#include "TTF_LocaTable.h"
using namespace TTFParser;

LocaTable::LocaTable(const TableRecordEntry & entry, char * source, int & offset, int glyph_number , bool isInt16 )
{
	m_GlyphLocation.resize(glyph_number + 1);
	offset = entry.GetOffset();
	int c = 0;
	if (isInt16)
	{
		for (int i = 0; i <= glyph_number; i++)
		{
			uint16_t off;
			TRead(source , &off, offset);
			off <<= 1;
			if (off != 0)
			{
				c++;
			}
			m_GlyphLocation[i] = off;
		}
	}
	else {
		for (int i = 0; i <= glyph_number; i++)
		{
			TRead(source, &m_GlyphLocation[i], offset);
		}
	}
}
