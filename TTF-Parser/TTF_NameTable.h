#ifndef _TTF_NAME_TABLE_H_
#define _TTF_NAME_TABLE_H_

#include "TTF_Type.h"
#include "TTF_RecordEntry.h"

#include <vector>

namespace TTFParser
{
	class NameValue
	{
	public:
		NameValue(char * source, int & offset);
		NameValue() {};
		friend class NameTable;
	private:
		uint16_t							m_PlatformID;
		uint16_t							m_EncodingID;
		uint16_t							m_LanguageID;
		uint16_t							m_NameID;
		uint16_t							m_Length;
		uint16_t							m_Offset;

	};

	class NameTable
	{
	public:
		NameTable(const TableRecordEntry & entry, char * source, int & offset);

	private:
		uint16_t								m_Format;
		uint16_t								m_Count;
		uint16_t								m_StringOffset;
		std::vector<NameValue>	m_NameValues;
		std::vector<std::string>		m_Names;
	};
}

#endif