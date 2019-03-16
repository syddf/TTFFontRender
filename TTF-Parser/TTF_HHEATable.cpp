#include "TTF_HHEATable.h"

using namespace TTFParser;

HHEATable::HHEATable(const TableRecordEntry & entry, char * source, int & offset)
{
	offset = entry.GetOffset();

	TRead(source, &m_MajorVersion, offset);
	TRead(source, &m_MinorVersion, offset);
	TRead(source, &m_Ascender, offset);
	TRead(source, &m_Descender, offset);
	TRead(source, &m_LineGap, offset);
	TRead(source, &m_AdvanceWidthMax, offset);
	TRead(source, &m_MinLeftSideBearing, offset);
	TRead(source, &m_MinRightSideBearing, offset);
	TRead(source, &m_XMaxExtent, offset);
	TRead(source, &m_CaretSlopeRise, offset);
	TRead(source, &m_CaretSlopeRun, offset);
	TRead(source, &m_CaretOffset, offset);
	TRead(source, &m_MetricDataFormat, offset);
	TRead(source, &m_NumberOfHMetrics, offset);

}
