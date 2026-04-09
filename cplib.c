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

void CALC_getKey(uint32_t *key1, uint32_t *key2);


// keys
#define NUM_KEYS 34

#ifdef TARGET_PC
static uint32_t keyCodes[NUM_KEYS] = {
    RL_KEY_LEFT_SHIFT,
    RL_KEY_DELETE,
    RL_KEY_BACKSPACE,
    RL_KEY_LEFT,
    RL_KEY_RIGHT,
    RL_KEY_Z,
    RL_KEY_U,
    RL_KEY_I,
    RL_KEY_O,
    RL_KEY_K,
    RL_KEY_L,
    RL_KEY_ENTER,
    RL_KEY_RIGHT_SHIFT,
    RL_KEY_THREE,
    RL_KEY_SIX,
    RL_KEY_NINE,
    RL_KEY_INSERT,
    RL_KEY_UP,
    RL_KEY_DOWN,
    RL_KEY_J,
    RL_KEY_X,
    RL_KEY_Y,
    RL_KEY_Z,
    RL_KEY_H,
    RL_KEY_COMMA,
    RL_KEY_MINUS,
    RL_KEY_ZERO,
    RL_KEY_PERIOD,
    RL_KEY_ONE,
    RL_KEY_TWO,
    RL_KEY_FOUR,
    RL_KEY_FIVE,
    RL_KEY_SEVEN,
    RL_KEY_EIGHT,
};
#else
static uint32_t keyCodes[NUM_KEYS] = {
    0x80000000,
    0x00020000,
    0x00000080,
    0x00004000,
    0x00008000,
    0x00002000,
    0x00000040,
    0x40000000,
    0x20000000,
    0x10000000,
    0x08000000, // 10
    0x04000000,
    0x00000004,
    0x00000008,
    0x00000010,
    0x00000020,
    0x80000000,
    0x00800000,
    0x00400000,
    0x00000080,
    0x00000040,
    0x40000000,
    0x00000020,
    0x00000010,
    0x00000008,
    0x00000004,
    0x04000000,
    0x00040000,
    0x08000000,
    0x00080000,
    0x10000000,
    0x00100000,
    0x20000000,
    0x00200000,
};
#endif


static int screenWidth, screenHeight;
static int numPixels;
static cpColor* pixelBuf;

static bool keyState[NUM_KEYS];
static bool lastKeyState[NUM_KEYS];

#ifdef TARGET_PC
static void* rlTexture;
static uint8_t* rlPixelBuf;
#else
static uint16_t* calcVRAM;
#endif


void cpInit() {
#ifdef TARGET_PC
    screenWidth = 320;
    screenHeight = 528;
    rlwInitWindow(screenWidth, screenHeight, "CPLib emu");

    rlTexture = rlwCreateTexture(screenWidth, screenHeight);
    rlPixelBuf = (uint8_t*)malloc(sizeof(uint8_t) * 4*screenWidth*screenHeight);
    memset(rlPixelBuf, 0, (int)sizeof(uint8_t) * 4*screenWidth*screenHeight);
#else
    CALC_LCD_GetSize(&screenWidth, &screenHeight);
    calcVRAM = CALC_LCD_GetVRAMAddress();
    CALC_LCD_VRAMBackup();
#endif

    numPixels = screenWidth * screenHeight;

    pixelBuf = (cpColor*)malloc(sizeof(cpColor) * numPixels);
    memset(pixelBuf, 0, (int)sizeof(cpColor) * numPixels);
}

void cpQuit() {
#ifdef TARGET_PC
    rlwCloseWindow();
#else
    CALC_LCD_VRAMRestore();
    CALC_LCD_Refresh();
#endif

    free(pixelBuf);
}

inline cpColor cpRGBtoColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    // https://stackoverflow.com/a/11471397
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

inline uint cpGetScreenWidth() {
    return screenWidth;
}

inline uint cpGetScreenHeight() {
    return screenHeight;
}

inline uint16_t* cpGetFramebuffer() {
    return pixelBuf;
}

inline void cpBeginDrawing() {
#ifdef TARGET_PC
    rlwBeginDrawing();
#endif
}

void cpEndDrawing() {
    memcpy(lastKeyState, keyState, sizeof(bool) * NUM_KEYS);

#ifdef TARGET_PC
    // convert pixels
    for (int i = 0; i < numPixels; i++) {
        // https://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888
        cpColor c = pixelBuf[i];
        uint8_t r = ( (c >> 11 & 0b11111) * 527 + 23 ) >> 6;
        uint8_t g = ( (c >> 5 & 0b111111) * 259 + 33 ) >> 6;
        uint8_t b = ( (c & 0b11111) * 527 + 23 ) >> 6;

        rlPixelBuf[i*4  ] = r;
        rlPixelBuf[i*4+1] = g;
        rlPixelBuf[i*4+2] = b;
        rlPixelBuf[i*4+3] = 255;
    }

    rlwUpdateTexture(rlTexture, rlPixelBuf);
    rlwDrawTexture(rlTexture);
    rlwEndDrawing();

    // fetch key states
    for (int i = 0; i < NUM_KEYS; i++) {
        keyState[i] = rlIsKeyDown(keyCodes[i]);
    }
#else
    memcpy(calcVRAM, pixelBuf, (int)sizeof(uint16_t) * numPixels);
    CALC_LCD_Refresh();

    // fetch key states
    for (int i = 0; i < NUM_KEYS; i++) {
        const uint32_t keycode = keyCodes[i];

        uint32_t key1, key2;
        CALC_getKey(&key1, &key2);

        keyState[i] = !!((i < 10 ? key1 : key2) & keycode);
    }
#endif
}

inline void cpClearBackground(const cpColor tint) {
    memset(pixelBuf, tint, (int)sizeof(uint16_t) * numPixels);
}

inline void cpDrawPixel(const uint x, const uint y, const cpColor tint) {
    pixelBuf[y*screenWidth + x] = tint;
}

inline void cpDrawLine(uint x1, uint y1, uint x2, uint y2, cpColor tint) {

}

void cpDrawRectangle(const uint x, const uint y, const uint w, const uint h, const cpColor tint) {
    for (uint sy = y; sy < y+h; sy++) {
        for (uint sx = x; sx < x+w; sx++) {
            cpDrawPixel(sx, sy, tint);
        }
    }
}

bool cpIsKeyDown(const uint keyIdx) {
    return keyState[keyIdx];
}

bool cpIsKeyPressed(const uint keyIdx) {
    return keyState[keyIdx] && !lastKeyState[keyIdx];
}

bool cpIsKeyUp(const uint keyIdx) {
    return !keyState[keyIdx];
}

bool cpIsKeyReleased(const uint keyIdx) {
    return !keyState[keyIdx] && lastKeyState[keyIdx];
}
