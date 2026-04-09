//
// Created by timo on 09.04.26.
//

#ifndef CPLIB_CPLIB_H
#define CPLIB_CPLIB_H

#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_PC
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
#else
#define HOLLYHOCK_SECTION_STRING(name, str)
#define APP_NAME(app_name)
#define APP_DESCRIPTION(app_description)
#define APP_AUTHOR(app_author)
#define APP_VERSION(app_version)
#endif


typedef uint32_t uint;
typedef uint16_t cpColor;

#define BLACK 0
#define WHITE (-1)


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

bool cpIsKeyDown(uint keycode);
bool cpIsKeyPressed(uint keycode);
bool cpIsKeyUp(uint keycode);
bool cpIsKeyReleased(uint keycode);


#endif //CPLIB_CPLIB_H
