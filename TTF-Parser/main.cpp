#include "TTF_OffsetTable.h"
#include "TTF_HeadTable.h"
#include "TTF_MaxpTable.h"
#include "TTF_NameTable.h"
#include "TTF_HHEATable.h" 
#include "TTF_CMapTable.h"
#include "TTF_LocaTable.h"
#include "TTF_GLYPH.h"
#include "TTF_Rasterizer.h"
#include <Windows.h>

#include <fstream>
using namespace std;

int main(int argc , char ** argv )
{
	if (argc != 3) return 0;
	ifstream in;
	in.open( argv[1] , ios::binary);

	in.seekg(0, ios::end);
	int size = in.tellg();
	in.seekg(0, ios::beg);
	char * buffer = new char[size];
	in.read(buffer, size);
	int offset = 0;
	TTFParser::OffsetTable of(buffer , offset);

	of.OutputInfo();
	TTFParser::HeadTable ht(of.SearchEntry("head") , buffer , offset);
	TTFParser::MaxpTable maxp(of.SearchEntry("maxp"), buffer, offset);
	TTFParser::NameTable name(of.SearchEntry("name"), buffer, offset); //maybe it's not used in rasterizing.
	TTFParser::HHEATable hhea(of.SearchEntry("hhea"), buffer, offset);
	TTFParser::CMapTable cmap(of.SearchEntry("cmap"), buffer, offset);
	TTFParser::LocaTable loca(of.SearchEntry("loca"), buffer, offset, maxp.GetNumGlyphs() , !ht.GetIdxToLocalFormat());

	wchar_t w_c;
	MultiByteToWideChar(CP_ACP, 0, argv[2], -1, &w_c, 1);

	TTFParser::TTFGlyph glyph(cmap, maxp, loca, hhea, of.SearchEntry("glyf").GetOffset(), of.SearchEntry("hmtx").GetOffset(),w_c,buffer);
	TTFRaster ras(glyph.GetGlyph());
	ras.Rasterize();

	ras.OutputToPng("sss.png");
	return 0;
}