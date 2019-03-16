#include "TTF_HeadTable.h"
using namespace TTFParser;

HeadTable::HeadTable(const TableRecordEntry & entry , char * source, int & offset)
{
	offset = entry.GetOffset();
	TRead(source, &m_Version, offset);
	TRead(source, &m_FontRevision, offset);
	TRead(source, &m_ChecksumAdjustment, offset);
	TRead(source, &m_MagicNumber, offset);
	TRead(source, &m_Flags, offset);
	TRead(source, &m_UnitsPerEm, offset);
	TRead(source, &m_Created, offset);
	TRead(source, &m_Modified, offset);
	TRead(source, &m_xMin, offset);
	TRead(source, &m_yMin, offset);
	TRead(source, &m_xMax, offset);
	TRead(source, &m_yMax, offset);
	TRead(source, &m_macType, offset);
	TRead(source, &m_lowestRecPPEM, offset);
	TRead(source, &m_fondDirectionHint, offset);
	TRead(source, &m_indexToLocFormat, offset);
	TRead(source, &m_glyphDataFormat, offset);
}
