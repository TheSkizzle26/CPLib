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


typedef enum cpOverclockMultipliers cpOverclockMultipliers;
enum cpOverclockMultipliers {
    OC_MUL_12 = 0x05,
    OC_MUL_16 = 0x07,
    OC_MUL_24 = 0x0B,
    OC_MUL_30 = 0x0E,
    OC_MUL_32 = 0x0F,
    OC_MUL_36 = 0x11,
    OC_MUL_48 = 0x17,

    OC_MUL_DEFAULT = 0x0F, // same as OC_MUL_32
};

typedef enum cpKeyIndices cpKeyIndices;
enum cpKeyIndices {
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

void cpSetTargetFPS(int value);
void cpSetOverclock(cpOverclockMultipliers mul);

cpColor cpRGBtoColor(uint8_t r, uint8_t g, uint8_t b);

uint cpGetScreenWidth();
uint cpGetScreenHeight();
uint16_t* cpGetFramebuffer();

void cpBeginDrawing();
void cpEndDrawing();

void cpClearBackground(cpColor tint);
void cpDrawPixel(int x, int y, cpColor tint); // no clipping
void cpDrawLine(int x1, int y1, int x2, int y2, cpColor tint); // no clipping (for now)
void cpDrawRectangle(int x, int y, int w, int h, cpColor tint);
void cpDrawCircle(int centerX, int centerY, int radius, cpColor tint);

bool cpIsKeyDown(cpKeyIndices keyIdx);
bool cpIsKeyPressed(cpKeyIndices keyIdx);
bool cpIsKeyUp(cpKeyIndices keyIdx);
bool cpIsKeyReleased(cpKeyIndices keyIdx);


#endif //CPLIB_CPLIB_H
