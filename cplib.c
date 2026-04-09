//
// Created by timo on 09.04.26.
//

#include "cplib.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_PC
#include "raylib_wrapper.h"
#endif


void CALC_LCD_ClearScreen();
uint16_t CALC_LCD_GetPixel(int x, int y);
void CALC_LCD_GetSize(int *width, int *height);
uint16_t* CALC_LCD_GetVRAMAddress();
void CALC_LCD_Refresh();
void CALC_LCD_SetPixel(int x, int y, uint16_t color);
void CALC_LCD_SetPixelFromPalette(int x, int y, uint8_t index);
void CALC_LCD_VRAMBackup();
void CALC_LCD_VRAMRestore();

#ifdef TARGET_PC
#include <stdlib.h>
#include <string.h>
#else
void free(void *ptr);
void* malloc(uint32_t size);
void* memcpy(void *destination, const void *source, int num);
void* memset(void *ptr, int value, int num);
#endif


static uint16_t* calcVRAM = (uint16_t*)0xB4000000;
static cpColor* pixelBuf;
static uint8_t rlPixelBuf;
static int screenWidth, screenHeight;


void cpInit() {
#ifdef TARGET_PC
    screenWidth = 320;
    screenHeight = 528;

    rlInitWindow(screenWidth, screenHeight, "CPLib emu");
    rlPixelBuf = (uint8_t)malloc(sizeof(uint8_t) * 4*screenWidth*screenHeight);
#else
    CALC_LCD_GetSize(&screenWidth, &screenHeight);
    CALC_LCD_VRAMBackup();
#endif

    pixelBuf = (cpColor*)malloc(sizeof(cpColor) * screenWidth*screenHeight);
}

void cpQuit() {
#ifdef TARGET_PC
    rlCloseWindow();
#else
    CALC_LCD_VRAMRestore();
    CALC_LCD_Refresh();
#endif

    free(pixelBuf);
}

inline cpColor cpRGBtoColor(uint8_t r, uint8_t g, uint8_t b) {
    return (r<<8 & 0b1111100000000000) |
           (g<<3 & 0b0000011111100000) |
           (b>>3 & 0b0000000000011111);
}

uint cpGetScreenWidth() {
    return screenWidth;
}

uint cpGetScreenHeight() {
    return screenHeight;
}

uint16_t* cpGetFramebuffer() {
    return pixelBuf;
}

void cpBeginDrawing() {
#ifdef TARGET_PC
    rlBeginDrawing();
#else
    // don't think we'll need to do anything here
#endif
}

void cpEndDrawing() {
    // TODO: fetch inputs

#ifdef TARGET_PC
    rlEndDrawing();
#else
#endif
}

void cpClearBackground(cpColor tint) {
    memset(pixelBuf, tint, screenWidth*screenHeight);
}