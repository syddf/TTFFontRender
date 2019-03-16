#ifndef _TTF_GLYPH_H_
#define _TTF_GLYPH_H_

#include <vector>
#include <set>
#include "TTF_MaxpTable.h"
#include "TTF_CMapTable.h"
#include "TTF_LocaTable.h"
#include "TTF_HHEATable.h"

namespace TTFParser
{

	struct Point_f
	{
		float x; 
		float y;
	};
	
	struct Point_i
	{
		int x;
		int y;
	};

	struct PointLess
	{	
		bool operator()(const Point_i & p1, const Point_i & p2) const
		{
			if (p1.y == p2.y &&  p1.x == p2.x) return false;
			return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
		}
	};

	struct Curve
	{
		Point_f p0;
		Point_f p1;
		Point_f p2;
		bool is_curve;
	};

	struct Contour
	{
		std::vector<Curve> curves;
	};

	struct Glyph
	{
		int16_t glyph_index;
		int16_t countour_num;
		uint16_t advance_width;
		int16_t left_side_bearing;
		int16_t bounding_box[4];
		uint32_t num_triangles;
		std::vector<Contour> countour_list;
	};

	struct GlyphFlag
	{
		bool off_curve;
		bool xShort;
		bool yShort;
		bool repeat;
		bool xDual;
		bool yDual;
	};

	class TTFGlyph
	{
	public:
		TTFGlyph(const CMapTable & cmap_table , 
			const MaxpTable & maxp , 
			const LocaTable & loca , 
			const HHEATable & hhea , 
			int glyf_offset,
			int hmtx_offset,
			wchar_t w_c,
			char * data);

	private:
		void LoadGlyph(int i , char * data, int & offset) ;

	private:
		Glyph									m_Glyph;
	public:
		const Glyph&						GetGlyph() { return m_Glyph; };
	};

}

#endif