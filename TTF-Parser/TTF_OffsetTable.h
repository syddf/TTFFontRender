#ifndef _TTF_OFFSET_TABLE_H_
#define _TTF_OFFSET_TABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"
#include <fstream>
#include <unordered_map>

namespace TTFParser
{
	class OffsetTable {
	public:
		OffsetTable(char * source, int & offset);

#ifdef DEBUG
		void OutputInfo();
#endif // DEBUG

		TableRecordEntry	SearchEntry(const char * entry_tag);

	private:
		ULONG					m_sfntVersion;
		USHORT					m_numTables;
		USHORT					m_searchRange;
		USHORT					m_entrySelector;
		USHORT					m_rangeShift;
		std::unordered_map<Tag,TableRecordEntry>	m_RecordEntries;
		typedef std::unordered_map<Tag, TableRecordEntry> EntryMapType;
	};
}

#endif