#ifndef _TTF_CMAPTABLE_H_
#define _TTF_CMAPTABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"
#include <map>

namespace TTFParser
{
	class CMapTable
	{
	public:
		CMapTable(const TableRecordEntry & entry, char * source, int & Offset);

	private:
		std::map<uint32_t, uint32_t>				m_GlyphReverseMap;
		std::map<uint32_t, uint32_t>				m_GlyphMap;
		friend class TTFGlyph;
	};

}

#endif