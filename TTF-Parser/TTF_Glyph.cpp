#include "TTF_GLYPH.h"
#include "TTF_Utility.h"

using namespace TTFParser;

TTFGlyph::TTFGlyph(const CMapTable & cmap_table, const LocaTable & loca, int glyf_offset , int hmtx_offset , wchar_t w_c, char * data )
{
		int offset;
		
		uint32_t ind = (uint32_t)w_c;
		if (cmap_table.m_GlyphMap.find(ind) == cmap_table.m_GlyphMap.end()) throw "can't find the character in cmap table .";
		uint32_t i = (*(cmap_table.m_GlyphMap.find(ind))).second;
		offset = hmtx_offset + i * sizeof(uint32_t);

		TRead(data, &m_Glyph.advance_width, offset);
		TRead(data, &m_Glyph.left_side_bearing, offset);
	
		offset = glyf_offset + loca.GetGlyphLocationByIndex(i);
		m_Glyph.glyph_index = i;

		LoadGlyph(i , data , offset );
	
}

void TTFGlyph::LoadGlyph(int i , char * data, int & offset)
{
	TRead(data, &m_Glyph.contour_num, offset);
	TRead(data, &m_Glyph.bounding_box[0], offset);
	TRead(data, &m_Glyph.bounding_box[1], offset);
	TRead(data, &m_Glyph.bounding_box[2], offset);
	TRead(data, &m_Glyph.bounding_box[3], offset);

	Point_f glyph_center;
	glyph_center.x = (m_Glyph.bounding_box[0] + m_Glyph.bounding_box[2]) / 2.0f;
	glyph_center.y = (m_Glyph.bounding_box[1] + m_Glyph.bounding_box[3]) / 2.0f;

	if (m_Glyph.contour_num > 0)
	{
		std::vector<uint16_t> countour_end(m_Glyph.contour_num);
		m_Glyph.countour_list.resize(m_Glyph.contour_num);
		for (int j = 0; j < m_Glyph.contour_num; j++)
		{
			TRead(data, &countour_end[j], offset);
		}

		int point_nums = countour_end[m_Glyph.contour_num - 1] + 1;

		uint16_t instruction_num;
		std::vector<std::vector<uint16_t>> point_index_in_countour;
		std::vector<uint16_t> countour_index_for_point(point_nums);

		TRead(data, &instruction_num, offset);
		offset += sizeof(uint8_t) * instruction_num;

		std::vector<GlyphFlag> glyph_flags(point_nums);

		int c_num = m_Glyph.contour_num;
		point_index_in_countour.resize( c_num , std::vector<uint16_t>());
		for (int i = 0; i < c_num; i++)
		{
			point_index_in_countour[i].resize( i > 0 ? countour_end[i] - countour_end[i - 1] : countour_end[0] + 1);
		}

		//flag 
		uint8_t repeat = 0;
		for (int i = 0 , p_ind = 0 , c_ind = 0; i < point_nums; i++ , p_ind++)
		{
			if (repeat == 0)
			{
				uint8_t flag;
				TRead(data, &flag, offset);
				if (flag & 0x8) {
					TRead(data, &repeat, offset);
				}
				glyph_flags[i].off_curve = (!(flag & 0b00000001)) != 0;
				glyph_flags[i].xShort = ((flag & 0b00000010)) != 0;
				glyph_flags[i].yShort = ((flag & 0b00000100)) != 0;
				glyph_flags[i].repeat = ((flag & 0b00001000)) != 0;
				glyph_flags[i].xDual = ((flag & 0b00010000)) != 0;
				glyph_flags[i].yDual = ((flag & 0b00100000)) != 0;
				if (p_ind >= (c_ind ? countour_end[c_ind] - countour_end[c_ind - 1] : countour_end[c_ind] + 1) )
				{
					p_ind = 0;
					c_ind++;
				}
				countour_index_for_point[i] = c_ind;
				point_index_in_countour[c_ind][p_ind] = i ;
			}
			else {
				glyph_flags[i] = glyph_flags[i - 1];
				repeat--;
			}
		}

		std::vector<Point_i> point_coordinates;
		point_coordinates.resize(point_nums);

		// x coordinate
		for (int i = 0; i < point_nums; i++)
		{
			if (glyph_flags[i].xDual && !glyph_flags[i].xShort)
			{
				point_coordinates[i].x = i > 0 ? point_coordinates[i - 1].x : 0 ;
			}
			else {
				if (glyph_flags[i].xShort)
				{
					uint8_t x;
					TRead(data, &x, offset);
					point_coordinates[i].x = x;
					if (!glyph_flags[i].xDual) {
						point_coordinates[i].x *= -1;
					}
				}
				else {
					int16_t x;
					TRead(data, &x, offset);
					point_coordinates[i].x = x;
				}
				if (i) point_coordinates[i].x += point_coordinates[i - 1].x;
			}
		}

		// y coordinate
		for (int i = 0; i < point_nums; i++)
		{
			if (glyph_flags[i].yDual && !glyph_flags[i].yShort)
			{
				point_coordinates[i].y = i > 0 ? point_coordinates[i - 1].y : 0;
			}
			else {
				if (glyph_flags[i].yShort)
				{
					uint8_t y;
					TRead(data, &y, offset);
					point_coordinates[i].y = y;
					if (!glyph_flags[i].yDual) {
						point_coordinates[i].y *= -1;
					}
				}
				else {
					int16_t y;
					TRead(data, &y, offset);
					point_coordinates[i].y = y;
				}
				if (i) point_coordinates[i].y += point_coordinates[i - 1].y;
			}
		}

		//curves
		//one point in the curve + one point off the curve + one point in the curve
		for (int j = 0; j < m_Glyph.contour_num; j++)
		{
			uint16_t point_num = j ?  countour_end[j] - countour_end[j - 1] : countour_end[j] + 1 ;
			uint16_t first_point_ind = point_index_in_countour[j][0];
			GlyphFlag first_point_flag = glyph_flags[first_point_ind];
			Point_f pre_point;
			//find the first point in the curve
			if (first_point_flag.off_curve)
			{
				uint16_t last_ind = point_index_in_countour[j][point_num - 1];
				GlyphFlag last_point_flag = glyph_flags[last_ind];
				if (last_point_flag.off_curve)
				{
					pre_point.x = (point_coordinates[first_point_ind].x + point_coordinates[last_ind].x) / 2.0f;
					pre_point.y = (point_coordinates[first_point_ind].y + point_coordinates[last_ind].y) / 2.0f;
				}
				else {
					pre_point.x = point_coordinates[last_ind].x;
					pre_point.y = point_coordinates[last_ind].y;
				}
			}
		
			for (int k = 0; k < point_num; k++)
			{
				uint16_t p0_ind = point_index_in_countour[j][k % point_num];
				uint16_t p1_ind = point_index_in_countour[j][(k + 1) % point_num];

				GlyphFlag p0_flag = glyph_flags[p0_ind];
				GlyphFlag p1_flag = glyph_flags[p1_ind];

				Point_i p0 = point_coordinates[p0_ind];
				Point_i p1 = point_coordinates[p1_ind];

				Curve curve;
				
				if (p0_flag.off_curve)
				{
					curve.p0.x = pre_point.x;
					curve.p0.y = pre_point.y;
					curve.p1.x = p0.x;
					curve.p1.y = p0.y;

					if (p1_flag.off_curve)
					{
						curve.p2.x = (p0.x + p1.x) / 2.0f;
						curve.p2.y = (p0.y + p1.y) / 2.0f;

						pre_point = curve.p2;
					}
					else {
						curve.p2.x = p1.x;
						curve.p2.y = p1.y;
					}
				}
				else if( !p1_flag.off_curve )
				{
					curve.p0.x = p0.x;
					curve.p0.y = p0.y;
					curve.p1.x = p1.x;
					curve.p1.y = p1.y;
					curve.p2.x = glyph_center.x + 0.5f;
					curve.p2.y = glyph_center.y + 0.5f;

					pre_point = curve.p0;
				}
				else {
					int p2_ind = point_index_in_countour[j][(k + 2) % point_num];
					GlyphFlag p2_flag = glyph_flags[p2_ind];
					Point_i p2 = point_coordinates[p2_ind];

					if (p2_flag.off_curve)
					{
						curve.p0.x = p0.x;
						curve.p0.y = p0.y;
						curve.p1.x = p1.x;
						curve.p1.y = p1.y;
						curve.p2.x = (p1.x + p2.x) / 2.0f;
						curve.p2.y = (p1.y + p2.y) / 2.0f;

						pre_point = curve.p2;
					}
					else {
						curve.p0.x = p0.x;
						curve.p0.y = p0.y;
						curve.p1.x = p1.x;
						curve.p1.y = p1.y;
						curve.p2.x = p2.x;
						curve.p2.y = p2.y;

						pre_point = curve.p0;
					}


				}

				if (p0_flag.off_curve || p1_flag.off_curve)
				{
					curve.is_curve = true;
					if (!p0_flag.off_curve) k++;
				}
				else {
					curve.is_curve = false;
				}

				m_Glyph.countour_list[j].curves.push_back(curve);

			}
			m_Glyph.num_triangles += m_Glyph.countour_list[j].curves.size();

		}
	}

}
