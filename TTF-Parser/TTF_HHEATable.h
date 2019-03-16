#ifndef _HHEATABLE_H_
#define _HHEATABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"

namespace TTFParser
{
	class HHEATable
	{
	public:
		HHEATable(const TableRecordEntry & entry, char * source, int & offset);

	private:
		uint16_t								m_MajorVersion;
		uint16_t								m_MinorVersion;
		FWORD								m_Ascender;
		FWORD								m_Descender;
		FWORD								m_LineGap;
		UFWORD								m_AdvanceWidthMax;
		FWORD								m_MinLeftSideBearing;
		FWORD								m_MinRightSideBearing;
		FWORD								m_XMaxExtent;
		int16_t									m_CaretSlopeRise;
		int16_t									m_CaretSlopeRun;
		int16_t									m_CaretOffset;
		int16_t									m_MetricDataFormat;
		uint16_t								m_NumberOfHMetrics;
	};

}

#endif