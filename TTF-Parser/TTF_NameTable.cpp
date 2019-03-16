#include "TTF_NameTable.h"

using namespace TTFParser;

NameValue::NameValue(char * source, int & offset)
{
	TRead(source, &m_PlatformID , offset);
	TRead(source, &m_EncodingID , offset );
	TRead(source, &m_LanguageID , offset);
	TRead(source, &m_NameID , offset);
	TRead(source, &m_Length , offset );
	TRead(source, &m_Offset , offset);

}

NameTable::NameTable(const TableRecordEntry & entry, char * source, int & offset)
{
	offset = entry.GetOffset();

	TRead(source, &m_Format , offset);
	TRead(source, &m_Count , offset );
	TRead(source, &m_StringOffset , offset );
	m_NameValues.resize(m_Count);
	m_Names.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		m_NameValues[i] = NameValue(source , offset );
	}
	for (int i = 0 ; i < m_Count ; i ++ )
	{
		if (m_NameValues[i].m_NameID >= m_Count) continue;
		char * name_string = new char[m_NameValues[i].m_Length];
		offset = m_StringOffset + m_NameValues[i].m_Offset + entry.GetOffset();
		memcpy(name_string, source + offset , m_NameValues[i].m_Length);
		m_Names[m_NameValues[i].m_NameID].assign(name_string , m_NameValues[i].m_Length);

		//消除每个字符之间可能有的间隔'\0'
		
		if (name_string[0] == 0)
		{
			int string_length = m_NameValues[i].m_Length >> 1;
			for (int j = 0; j < string_length; j++)
			{
				name_string[j] = name_string[2 * j + 1];
			}
		}
		delete[] name_string;
	}
}
