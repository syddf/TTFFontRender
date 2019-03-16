#include "TTF_OffsetTable.h"
#include "TTF_Utility.h"
using namespace TTFParser;
using namespace std;

void Read(const char * source, char * target, const int per_data_size, int & offset, const int data_num = 1)
{
	assert(target != NULL && source != NULL);
	for (int i = 0; i < data_num; i++)
	{
		for (int j = per_data_size - 1; j >= 0; j--)
		{
			target[j + i * per_data_size] = source[offset + per_data_size - 1 - j];
		}
		offset += per_data_size;
	}
	
}


OffsetTable::OffsetTable(char * source, int & offset)
{
	offset = 0;
	TRead(source, &m_sfntVersion , offset);

	TRead(source , &m_numTables , offset );
	TRead(source, &m_searchRange, offset);
	TRead(source, &m_entrySelector, offset);
	TRead(source, &m_rangeShift, offset);

	for (int i = 0; i < m_numTables; i++)
	{
		TableRecordEntry entry;
		entry.LoadEntry(source, offset);
		m_RecordEntries.insert(std::make_pair(entry.m_Tag,entry));
	}
}

#ifdef DEBUG
void OffsetTable::OutputInfo()
{
	cout << "Version : " << m_sfntVersion << endl;
	cout << "TableNum : " << m_numTables << endl;
	cout << "SearchRange : " << m_searchRange << endl;
	cout << "EntrySelector : " << m_entrySelector << endl;
	cout << "RangeShift : " << m_rangeShift << endl;
	cout << endl;
	for (EntryMapType::iterator it = m_RecordEntries.begin() ; it != m_RecordEntries.end() ; it ++ )
	{
		(*it).second.OutputInfo();
	}
}

TableRecordEntry OffsetTable::SearchEntry(const char * entry_tag)
{
	assert(entry_tag != NULL);
	Tag* t = (Tag*)entry_tag;
	EntryMapType::iterator it = m_RecordEntries.find(*t);

	assert(it != m_RecordEntries.end());
	return TableRecordEntry((*it).second);
}

#endif // DEBUG