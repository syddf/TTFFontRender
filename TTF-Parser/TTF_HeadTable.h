#ifndef _TTF_HEAD_TABLE_H_
#define _TTF_HEAD_TABLE_H_

#include "TTF_Type.h"
#include "TTF_Utility.h"
#include "TTF_RecordEntry.h"

namespace TTFParser
{
	class HeadTable {
	public:
		HeadTable(const TableRecordEntry & entry , char * source , int & offset );

	public:
		int16_t					GetIdxToLocalFormat() { return m_indexToLocFormat; }

	private:
		Fixed					m_Version;
		Fixed					m_FontRevision;
		uint32_t				m_ChecksumAdjustment;
		uint32_t				m_MagicNumber;
		uint16_t				m_Flags;
		uint16_t				m_UnitsPerEm;
		LONGDATETIME	m_Created;
		LONGDATETIME	m_Modified;
		int16_t					m_xMin;
		int16_t					m_yMin;
		int16_t					m_xMax;
		int16_t					m_yMax;
		uint16_t				m_macType;
		uint16_t				m_lowestRecPPEM;
		int16_t					m_fondDirectionHint;
		int16_t					m_indexToLocFormat;
		int16_t					m_glyphDataFormat;
	};
}

#endif