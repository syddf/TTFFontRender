#include "TTF_CMapTable.h"

using namespace TTFParser;
#include <vector>

CMapTable::CMapTable(const TableRecordEntry & entry, char * source, int & Offset)
{
	uint16_t version;
	uint16_t numTables;
	Offset = entry.GetOffset();
	TRead(source, &version, Offset);
	TRead(source, &numTables , Offset);
	for (int i = 0; i < numTables; i++)
	{
		uint16_t platform_id, encoding_id;
		uint32_t offset;
		TRead(source, &platform_id , Offset);
		TRead(source, &encoding_id , Offset);
		TRead(source, &offset , Offset);
		if (!((platform_id == 0 && encoding_id == 3) || (platform_id == 3 && encoding_id == 1 )))
		{
			continue;
		}
		Offset = offset + entry.GetOffset();
		uint16_t format;
		TRead(source, &format, Offset);
		if (format != 4) continue;
		uint16_t length, language, segCountx2, searchRange, entrySelector, rangeShift;
		TRead(source, &length, Offset);
		TRead(source, &language, Offset);
		TRead(source, &segCountx2, Offset);
		TRead(source, &searchRange, Offset);
		TRead(source, &entrySelector, Offset);
		TRead(source, &rangeShift, Offset);

		uint16_t segCount = segCountx2 >> 1;
		std::vector<uint16_t> endCode, startCode, idRangeOffset ;
		std::vector<int16_t> idDelta;
		endCode.resize(segCount);
		startCode.resize(segCount);
		idRangeOffset.resize(segCount);
		idDelta.resize(segCount);

		uint16_t reservedPad;
		for (int j = 0; j < segCount; j++)
		{
			TRead(source, &endCode[j], Offset);
		}
		TRead(source, &reservedPad, Offset);
		for (int j = 0; j < segCount; j++)
		{
			TRead(source, &startCode[j], Offset);
		}
		for (int j = 0; j < segCount; j++)
		{
			TRead(source, &idDelta[j], Offset);
		}
		for (int j = 0; j < segCount; j++)
		{
			TRead(source, &idRangeOffset[j], Offset);
		}
		auto tmp_pos = Offset;

		for (int j = 0; j < segCount; j++)
		{
			if ( idRangeOffset[j] == 0) 
			{
				for (uint32_t k = startCode[j]; k <= endCode[j]; k++)
				{
					m_GlyphMap[k] = k + idDelta[j];
					m_GlyphReverseMap[k + idDelta[j]] = k;
				}
			}
			else {
				for (uint32_t k = startCode[j]; k <= endCode[j]; k++)
				{
					int ind = k - startCode[j];
					Offset = tmp_pos + idRangeOffset[j];
					uint32_t m;
					TRead(source, &m , Offset);
				}
			}
		}
	}

}