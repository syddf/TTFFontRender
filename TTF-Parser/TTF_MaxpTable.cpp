#include "TTF_MaxpTable.h"

TTFParser::MaxpTable::MaxpTable(const TableRecordEntry & entry, char * source, int & offset)
{
	offset = entry.GetOffset();
	TRead(source, &m_Version, offset);
	TRead(source, &m_NumGlyphs, offset);
	TRead(source, &m_MaxPoints, offset);
	TRead(source, &m_MaxContours, offset);
	TRead(source, &m_MaxCompositePoints, offset);
	TRead(source, &m_MaxCompositeContours, offset);
	TRead(source, &m_MaxZones, offset);
	TRead(source, &m_MaxTwilightPoints, offset);
	TRead(source, &m_MaxStorage, offset);
	TRead(source, &m_MaxFunctionDefs, offset);
	TRead(source, &m_MaxStackElements, offset);
	TRead(source, &m_MaxSizeOfInstructions, offset);
	TRead(source, &m_MaxComponentElements, offset);
	TRead(source, &m_MaxComponentDepth, offset);
}
