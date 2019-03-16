#ifndef _TTF_LOCATABLE_H_
#define _TTF_LOCATABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"
#include <vector>

namespace TTFParser
{
	class LocaTable
	{
	public:
		LocaTable(const TableRecordEntry & entry, char * source, int & offset, int glyph_number , bool isInt16);

	public:
		std::vector<uint32_t> GetGlyphLocation() const
		{
			return m_GlyphLocation;
		}
		int GetGlyphLocationByIndex(int index) const
		{
			return m_GlyphLocation[index];
		}

	private:
		std::vector<uint32_t>						m_GlyphLocation;

	};

}

#endif