#ifndef _TTF_RECORD_ENTRY_H_
#define _TTF_RECORD_ENTRY_H_

#include "TTF_Type.h"
#include "TTF_Utility.h"

namespace TTFParser
{
	class TableRecordEntry {
	public:
		void						LoadEntry(char * source , int & offset );
		friend class			OffsetTable;
		#ifdef DEBUG
		void						OutputInfo();
		#endif // DEBUG

	public:
		ULONG				GetOffset() const { return m_Offset;  }

	private:
		Tag						m_Tag;
		ULONG				m_Checksum;
		ULONG				m_Offset;
		ULONG				m_Length;
	};
}

#endif