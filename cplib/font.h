#ifndef CPLIB_FONT_H
#define CPLIB_FONT_H

#include "vector.h"
#include "common.h"
#include "rect.h"


typedef struct {
    int width;
    int height;
    uint8_t* data; // every bit is one pixel
} cpFontGlyph;

typedef struct {
    int baseSize;
    int glyphSpacing;
    int glyphStart;
    int glyphCount;
    cpFontGlyph* glyphs;
} cpFont;

extern cpFont cpDefaultFont; // Raylib's default font, about 8 kb


void cpDefaultFontInit();
void cpDefaultFontUnload();
void cpLoadFont(cpFont font);
void cpDrawText(const char* text, int startX, int startY, cpColor tint);


#endif