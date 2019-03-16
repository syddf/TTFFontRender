#ifndef _MAXP_TABLE_H_
#define _MAXP_TABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"

namespace TTFParser
{
	class MaxpTable
	{
	public:
		MaxpTable(const TableRecordEntry & entry, char * source, int & offset);

	public:
		uint16_t				GetNumGlyphs() const { return m_NumGlyphs;  }
	private:
		Fixed					m_Version;
		uint16_t				m_NumGlyphs;
		uint16_t				m_MaxPoints;
		uint16_t				m_MaxContours;
		uint16_t				m_MaxCompositePoints;
		uint16_t				m_MaxCompositeContours;
		uint16_t				m_MaxZones;
		uint16_t				m_MaxTwilightPoints;
		uint16_t				m_MaxStorage;
		uint16_t				m_MaxFunctionDefs;
		uint16_t				m_MaxInstructionDefs;
		uint16_t				m_MaxStackElements;
		uint16_t				m_MaxSizeOfInstructions;
		uint16_t				m_MaxComponentElements;
		uint16_t				m_MaxComponentDepth;
	};
}

#endif