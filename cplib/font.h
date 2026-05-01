#ifndef CPLIB_FONT_H
#define CPLIB_FONT_H

#ifdef CPLIB_ENABLE_FONT

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


void cpDefaultFontInit(); // Called inside cpInit().
void cpLoadDefaultFont(); // Load the CPLib's default font.
void cpDefaultFontFree(); // Free the default font clearing up a bit of RAM.
void cpLoadFont(cpFont font); // Load a specific font.
void cpDrawText(const char* text, int startX, int startY, int fontSize, cpColor tint); // Draw some text.

#endif

#endif