//
// Created by timo on 09.04.26.
//

/*
 * TODO:
 * - add CALC_* prefix to cmt, power, etc.
 * - use dmac for rendering
 * - add clipping for rendering
 */

#include "cplib.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_PC
#include "raylib_wrapper.h"
#else
#include "cpg.h"
#include "cmt.h"
#include "power.h"
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

// keys
#define NUM_KEYS 34
void CALC_getKey(uint32_t *key1, uint32_t *key2);

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
    0x08000000,
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
static cpColor* pixelBuf __attribute__((aligned(32)));

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
    numPixels = screenWidth * screenHeight;
    rlwInitWindow(screenWidth, screenHeight, "CPLib emu");

    // create raylib stuff
    rlTexture = rlwCreateTexture(screenWidth, screenHeight);
    rlPixelBuf = (uint8_t*)malloc(sizeof(uint8_t) * 4*numPixels);
    memset(rlPixelBuf, 0, (int)sizeof(uint8_t) * 4*numPixels);
#else
    // init lcd
    CALC_LCD_GetSize(&screenWidth, &screenHeight);
    numPixels = screenWidth * screenHeight;
    calcVRAM = CALC_LCD_GetVRAMAddress();
    CALC_LCD_VRAMBackup();

    // init timers
    POWER_MSTPCR0->CMT = 0;
#endif

    pixelBuf = (cpColor*)malloc(sizeof(cpColor) * numPixels);
    memset(pixelBuf, 0, (int)sizeof(cpColor) * numPixels);

    cpSetTargetFPS(0);
}

void cpQuit() {
#ifdef TARGET_PC
    rlwCloseWindow();
#else
    // reset clock speed
    cpg_set_pll_mul(OC_MUL_DEFAULT);

    // restore lcd
    CALC_LCD_VRAMRestore();
    CALC_LCD_Refresh();

    // stop timers
    cmt_stop();
    POWER_MSTPCR0->CMT = 0;
#endif

    free(pixelBuf);
}

void cpSetTargetFPS(const int value) {
#ifdef TARGET_PC
    rlwSetTargetFPS(value);
#else
    if (value <= 0) {
        // infinite
        cmt_stop();
    } else {
        cmt_set(CMT_TICKS_PER_SEC / value, CMT_MODE_ONE_SHOT, CMT_REQUEST_DISABLE);
    }
#endif
}

void cpSetOverclock(cpOverclockMultipliers mul) {
#ifdef TARGET_PC
    mul *= 1; // don't throw unused warning
#else
    cpg_set_pll_mul(mul);
#endif
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
#else
    // start frame timer
    cmt_start();
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

    // render raylib texture
    rlwUpdateTexture(rlTexture, rlPixelBuf);
    rlwDrawTexture(rlTexture);
    rlwEndDrawing();

    // fetch key states
    for (int i = 0; i < NUM_KEYS; i++) {
        keyState[i] = rlwIsKeyDown(keyCodes[i]);
    }
#else
    // draw pixel buffer
    memcpy(calcVRAM, pixelBuf, (int)sizeof(uint16_t) * numPixels);
    CALC_LCD_Refresh();

    // fetch key states
    for (int i = 0; i < NUM_KEYS; i++) {
        const uint32_t keycode = keyCodes[i];

        uint32_t key1, key2;
        CALC_getKey(&key1, &key2);

        keyState[i] = !!((i < 10 ? key1 : key2) & keycode);
    }

    // wait till we reach our target fps
    cmt_wait();
#endif
}

inline void cpClearBackground(const cpColor tint) {
    memset(pixelBuf, tint, (int)sizeof(uint16_t) * numPixels);
}

// DOESN'T HAVE CLIPPING!!! (for performance reasons)
inline void cpDrawPixel(const int x, const int y, const cpColor tint) {
    pixelBuf[y*screenWidth + x] = tint;
}

inline void cpDrawLine(int x1, int y1, int x2, int y2, cpColor tint) {
    // TODO: add clipping
    // bresenham line algorithm, based on https://saturncloud.io/blog/bresenham-line-algorithm-a-powerful-tool-for-efficient-line-drawing/
    int dx = x2 - x1; if (dx < 0) dx *= -1;
    int dy = y2 - y1; if (dy < 0) dy *= -1;
    const bool slope = dy > dx;

    if (slope) {
        int temp = x1;
        x1 = y1;
        y1 = temp;

        temp = x2;
        x2 = y2;
        y2 = temp;
    }

    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;

        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    dx = x2 - x1; if (dx < 0) dx *= -1;
    dy = y2 - y1; if (dy < 0) dy *= -1;
    int error = dx / 2;
    int y = y1;
    int ystep = y1 < y2 ? 1 : -1;

    for (int x = x1; x < x2+1; x++) {
        int sx, sy;

        if (slope) {
            sx = y;
            sy = x;
        } else {
            sx = x;
            sy = y;
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }

        cpDrawPixel(sx, sy, tint);
    }
}

void cpDrawRectangle(int x, int y, int w, int h, cpColor tint) {
    // clipping
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x+w >= screenWidth) w = screenWidth-x;
    if (y+h >= screenHeight) h = screenHeight-y;

    for (int sy = y; sy < y+h; sy++) {
        for (int sx = x; sx < x+w; sx++) {
            cpDrawPixel(sx, sy, tint);
        }
    }
}

void cpDrawCircle(int centerX, int centerY, int radius, cpColor tint) {
    // TODO: optimize clipping maybe?
    // based off: https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y) {
        for (int i = centerX - x; i <= centerX + x; i++) {
            if (i < 0 || i >= screenWidth) continue;

            if (centerY + y >= 0 && centerY + y < screenHeight)
                cpDrawPixel(i, centerY + y, tint);
            if (centerY - y >= 0 && centerY - y < screenHeight)
                cpDrawPixel(i, centerY - y, tint);
        }

        for (int i = centerX - y; i <= centerX + y; i++) {
            if (i < 0 || i >= screenWidth) continue;

            if (centerY + x >= 0 && centerY + x < screenHeight)
                cpDrawPixel(i, centerY + x, tint);
            if (centerY - x >= 0 && centerY - x < screenHeight)
                cpDrawPixel(i, centerY - x, tint);
        }

        y++;
        radiusError += yChange;
        yChange += 2;

        if ((radiusError << 1) + xChange > 0) {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

inline bool cpIsKeyDown(const cpKeyIndices keyIdx) {
    return keyState[keyIdx];
}

inline bool cpIsKeyPressed(const cpKeyIndices keyIdx) {
    return keyState[keyIdx] && !lastKeyState[keyIdx];
}

inline bool cpIsKeyUp(const cpKeyIndices keyIdx) {
    return !keyState[keyIdx];
}

inline bool cpIsKeyReleased(const cpKeyIndices keyIdx) {
    return !keyState[keyIdx] && lastKeyState[keyIdx];
}
