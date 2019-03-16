#ifndef _TTF_RASTER_H_
#define _TTF_RASTER_H_

#include "TTF_GLYPH.h"

class TTFRaster
{
public:
	TTFRaster(const TTFParser::Glyph & glyph) : m_Glyph(glyph) { buffer = NULL; };
	~TTFRaster() { if (buffer != NULL) { delete buffer; buffer = NULL; } };
	void Rasterize();
	void RasterBesierContour(const TTFParser::Curve & curve);
	void RasterLine(const TTFParser::Curve & curve);
	void Fill();

	void DrawBoundaryPoint(int x , int y );
	void DrawPoint(int x, int y);
	void OutputToPng(const char * file);

private:
	TTFParser::Glyph m_Glyph;
	unsigned char * buffer;
};

#endif