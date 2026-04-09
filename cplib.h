//
// Created by timo on 09.04.26.
//

#ifndef CPLIB_CPLIB_H
#define CPLIB_CPLIB_H

#include <stdint.h>
#include <stdbool.h>


#ifdef TARGET_PC
#define HOLLYHOCK_SECTION_STRING(name, str)
#define APP_NAME(app_name)
#define APP_DESCRIPTION(app_description)
#define APP_AUTHOR(app_author)
#define APP_VERSION(app_version)
#else
#define HOLLYHOCK_SECTION_STRING(name, str) \
__attribute__ ((section(".hollyhock_" #name))) \
volatile char hollyhock_ ## name [] = str;
#define APP_NAME(app_name) \
HOLLYHOCK_SECTION_STRING(name, app_name)
#define APP_DESCRIPTION(app_description) \
HOLLYHOCK_SECTION_STRING(description, app_description)
#define APP_AUTHOR(app_author) \
HOLLYHOCK_SECTION_STRING(author, app_author)
#define APP_VERSION(app_version) \
HOLLYHOCK_SECTION_STRING(version, app_version)
#endif


typedef uint32_t uint;
typedef uint16_t cpColor;

// literally just raylib's colors
#define LIGHTGRAY 52825
#define GRAY 33808
#define DARKGRAY 21130
#define YELLOW 65472
#define GOLD 65088
#define ORANGE 64768
#define PINK 64376
#define RED 57670
#define MAROON 47366
#define GREEN 1830
#define LIME 1253
#define DARKGREEN 933
#define SKYBLUE 26111
#define BLUE 990
#define DARKBLUE 661
#define PURPLE 52191
#define VIOLET 33271
#define DARKPURPLE 28911
#define BEIGE 54672
#define BROWN 31561
#define DARKBROWN 18917
#define WHITE 65535
#define BLACK 0
#define BLANK 0
#define MAGENTA 63519
#define RAYWHITE 63422


enum KeyIndices {
    KEY_SHIFT		= 0,
    KEY_CLEAR		= 1, //The Power key
    KEY_BACKSPACE	= 2,
    KEY_LEFT		= 3,
    KEY_RIGHT		= 4,
    KEY_Z			= 5,
    KEY_POWER		= 6, //The exponent key
    KEY_DIVIDE		= 7,
    KEY_MULTIPLY	= 8,
    KEY_SUBTRACT	= 9,
    KEY_ADD			= 10,
    KEY_EXE			= 11,
    KEY_EXP			= 12,
    KEY_3			= 13,
    KEY_6			= 14,
    KEY_9			= 15,

    KEY_KEYBOARD	= 16,
    KEY_UP			= 17,
    KEY_DOWN		= 18,
    KEY_EQUALS		= 19,
    KEY_X			= 20,
    KEY_Y			= 21,
    KEY_LEFT_BRACKET= 22,
    KEY_RIGHT_BRACKET= 23,
    KEY_COMMA		= 24,
    KEY_NEGATIVE	= 25,
    KEY_0			= 26,
    KEY_DOT			= 27,
    KEY_1			= 28,
    KEY_2			= 29,
    KEY_4			= 30,
    KEY_5			= 31,
    KEY_7			= 32,
    KEY_8			= 33,
};


void cpInit();
void cpQuit();

cpColor cpRGBtoColor(uint8_t r, uint8_t g, uint8_t b);

uint cpGetScreenWidth();
uint cpGetScreenHeight();
uint16_t* cpGetFramebuffer();

void cpBeginDrawing();
void cpEndDrawing(); // also fetch inputs

void cpClearBackground(cpColor tint);
void cpDrawPixel(uint x, uint y, cpColor tint);
void cpDrawLine(uint x1, uint y1, uint x2, uint y2, cpColor tint);
void cpDrawRectangle(uint x, uint y, uint w, uint h, cpColor tint);

bool cpIsKeyDown(uint keyIdx);
bool cpIsKeyPressed(uint keyIdx);
bool cpIsKeyUp(uint keyIdx);
bool cpIsKeyReleased(uint keyIdx);


#endif //CPLIB_CPLIB_H
