#include "TTF_Rasterizer.h"
#include "svpng.h"
#include <algorithm>
#include <queue>
using namespace std;

#define WIDTH 2048
#define HEIGHT 2048

using namespace TTFParser;

enum PointStateEnum {
	NotProcess = 0,
	IsContour,
	InContour,
	OutContour
}flag[HEIGHT][WIDTH];;

void TTFRaster::Rasterize()
{
	if (buffer == NULL)
	{
		buffer = new unsigned char[WIDTH * HEIGHT * 3];
	}
	memset(flag, NotProcess, sizeof(flag));
	int countour_num = m_Glyph.countour_num;
	int width = (m_Glyph.bounding_box[2] - m_Glyph.bounding_box[0]) ;
	int height = (m_Glyph.bounding_box[3] - m_Glyph.bounding_box[1]);
	int x_delta = (WIDTH - width) / 2;
	int y_delta = (HEIGHT - height) / 2;

	for (int i = 0; i < countour_num; i++)
	{
		int curve_num = m_Glyph.countour_list[i].curves.size();
		for (int j = 0; j < curve_num; j++)
		{
			m_Glyph.countour_list[i].curves[j].p0.x += ( -m_Glyph.bounding_box[0] + x_delta);
			m_Glyph.countour_list[i].curves[j].p1.x += (-m_Glyph.bounding_box[0] + x_delta);
			m_Glyph.countour_list[i].curves[j].p2.x += (-m_Glyph.bounding_box[0] + x_delta );
			m_Glyph.countour_list[i].curves[j].p0.y += (-m_Glyph.bounding_box[1] + y_delta);
			m_Glyph.countour_list[i].curves[j].p1.y += (-m_Glyph.bounding_box[1] + y_delta);
			m_Glyph.countour_list[i].curves[j].p2.y += (-m_Glyph.bounding_box[1] + y_delta);
			m_Glyph.countour_list[i].curves[j].p0.y = HEIGHT - m_Glyph.countour_list[i].curves[j].p0.y;
			m_Glyph.countour_list[i].curves[j].p1.y = HEIGHT - m_Glyph.countour_list[i].curves[j].p1.y;
			m_Glyph.countour_list[i].curves[j].p2.y = HEIGHT - m_Glyph.countour_list[i].curves[j].p2.y;

		}
	}

	for (int i = 0; i < countour_num; i++)
	{
		int curve_num = m_Glyph.countour_list[i].curves.size();
		for (int j = 0; j < curve_num; j++)
		{
			if (m_Glyph.countour_list[i].curves[j].is_curve == true)
			{
				RasterBesierContour(m_Glyph.countour_list[i].curves[j]);
			}
			else {
				RasterLine(m_Glyph.countour_list[i].curves[j]);
			}
		}
	}

	Fill();
}

void TTFRaster::RasterBesierContour(const TTFParser::Curve & curve)
{
	const float delta = 0.0005f;

	auto lerp_point = [&](Point_f p1 , Point_f p2 , float t)->Point_f {
		Point_f tmp;
		tmp.x = p1.x * t + p2.x * (1 - t);
		tmp.y = p1.y * t + p2.y * (1 - t);
		return tmp;
	};

	for (float i = 0.0f; i <= 1.0f; i += delta)
	{
		Point_f t1 = lerp_point(curve.p0, curve.p1 , i);
		Point_f t2 = lerp_point(curve.p1, curve.p2, i);
		Point_f p = lerp_point(t1, t2, i);

		int px = p.x ;
		int py = p.y;
		DrawBoundaryPoint(px, py);
	}
}

void TTFRaster::RasterLine(const TTFParser::Curve & curve)
{
	int x1 = curve.p0.x;
	int y1 = curve.p0.y;
	int x2 = curve.p1.x ;
	int y2 = curve.p1.y;


	if (x1 == x2 && y1 == y2)
	{
		DrawBoundaryPoint(x1, y1);
	}
	else if (x1 == x2)
	{
		int inc = y1 < y2 ? 1: -1 ;
		for (int y = y1; y != y2; y += inc)
		{
			DrawBoundaryPoint(x1, y);
		}
		DrawBoundaryPoint(x1, y2);
	}
	else if (y1 == y2)
	{
		int inc = x1 < x2 ? 1 : -1;
		for (int x = x1; x != x2; x += inc)
		{
			DrawBoundaryPoint(x, y1);
		}
		DrawBoundaryPoint(x2, y1);
	}
	else {
		int x, y, rem = 0;
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy)
		{
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++)
			{
				DrawBoundaryPoint(x, y);
				rem += dy;
				if (rem >= dx)
				{
					rem -= dx;
					y += (y2 > y1) ? 1 : -1;
					DrawBoundaryPoint(x, y);
				}
			}
			DrawBoundaryPoint(x2, y2);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++)
			{
				DrawBoundaryPoint(x, y);
				rem += dx;
				if (rem += dy)
				{
					rem -= dy;
					x += (x2 > x1) ? 1 : -1;
					DrawBoundaryPoint(x, y);
				}
			}
			DrawBoundaryPoint(x2, y2);
		}
	}
}

//Find the point which is not the contour's point, judge whether it's in the contour and fill its neighbors with the same state.
//Judge rule : find the number of the contour's points on the left of the target point , if the number is odd , the point is in the contour , otherwise it's not.
//Notice that if there are several contour's points which are in the same line and have consecutive x coordinates , we should only count one of them and ignore others.

void TTFRaster::Fill()
{
	struct tmp_node
	{
		int x;
		int y;
	};
	auto FillNeighbor = [&](int start_x , int start_y , PointStateEnum state )->void
	{
		queue<tmp_node> que;
		tmp_node s_n = {start_x , start_y };
		que.push(s_n);

		int dx[4] = { -1 , 1 , 0 , 0 };
		int dy[4] = { 0 , 0 , -1 , 1 };
		while (!que.empty())
		{
			tmp_node t = que.front();
			while (!que.empty())
			{
				tmp_node top_n = que.front();
				que.pop();
				for (int i = 0; i < 4; i++)
				{
					int new_x = top_n.x + dx[i];
					int new_y = top_n.y + dy[i];
					tmp_node new_node = { new_x , new_y };
					if (new_x < 0 || new_x >= WIDTH || new_y < 0 || new_y >= HEIGHT || flag[new_y][new_x] != NotProcess ) continue;
					que.push(new_node);
					flag[new_y][new_x] = state;
				}
			}
		}
	};
	auto IsInContour = [&](int x, int y)->bool
	{
		if (flag[y][x] != NotProcess) return false;
		int count = 0;
		for (int i = 0; i < x ; i++)
		{
			if (flag[y][i] == IsContour && flag[y][i+1] != IsContour )
			{
				count++;
			}
		}
		return count % 2;
	};

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (flag[i][j] == NotProcess)
			{
				if (IsInContour(j, i))
				{
					flag[i][j] = InContour;
					FillNeighbor(j, i, InContour);
				}
				else {
					flag[i][j] = OutContour;
					FillNeighbor(j, i, OutContour);
				}
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (flag[i][j] == InContour) DrawPoint(j, i);
		}
	}

}

void TTFRaster::DrawBoundaryPoint(int x, int y)
{
	buffer[y * WIDTH * 3 + x * 3] = 255;
	buffer[y * WIDTH * 3 + x * 3 + 1] = 255;
	buffer[y * WIDTH * 3 + x * 3 + 2] = 255;

	Point_i new_p;
	new_p.y = y;
	new_p.x = x;
	flag[y][x] = IsContour;
}

void TTFRaster::DrawPoint(int x, int y)
{
	buffer[y * WIDTH * 3 + x * 3] = 255;
	buffer[y * WIDTH * 3 + x * 3 + 1] = 255;
	buffer[y * WIDTH * 3 + x * 3 + 2] = 255;
}

void TTFRaster::OutputToPng(const char * file)
{
	assert(buffer != NULL);
	FILE * fp = fopen(file, "wb");
	svpng(fp, WIDTH, HEIGHT, buffer, 0);
	fclose(fp);

	delete buffer;
	buffer = NULL;
}
