#include "TTF_RecordEntry.h"
using namespace TTFParser;
using namespace std;

void TableRecordEntry::LoadEntry(char * source , int & offset)
{
	memcpy((char*)&m_Tag, source + offset, sizeof(Tag));
	offset += sizeof(Tag);
	TRead(source, &m_Checksum , offset);
	TRead(source, &m_Offset , offset);
	TRead(source, &m_Length , offset );
}

#ifdef DEBUG
void	TableRecordEntry::OutputInfo()
{
	char * t = (char*)&m_Tag;
	cout << "Tag : " << (char)t[0] << (char)t[1] << (char)t[2] << (char)t[3] << endl;
	cout << "Checksum : " << m_Checksum << endl;
	cout << "Offset : " << m_Offset << endl;
	cout << "Length : " << m_Length << endl;
	cout << endl;
}
#endif // DEBUG