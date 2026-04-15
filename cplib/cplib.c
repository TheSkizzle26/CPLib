//
// Created by timo on 09.04.26.
//


#include "cplib.h"
#include "common.h"
#include "vector.h"
#include "matrix.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef TARGET_PC
#include <stdlib.h>
#include <string.h>
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

typedef struct {
    cpVector3 position; // use this instead of storing pos in viewMatrix, would require mat4x4
    cpMatrix3 rotationMatrix;
    fix16_t focalLength;
} cpInternalCamera3d;

static int screenWidth, screenHeight;
static int numPixels;
static cpColor* pixelBuf __attribute__((aligned(32))); // faster memcpy

static bool keyState[NUM_KEYS];
static bool lastKeyState[NUM_KEYS];

static cpInternalCamera3d internalCamera3d;

// some constants for 3d rendering
static fix16_t fixHalfScreenWidth;
static fix16_t fixHalfScreenHeight;
static fix16_t fixNegHalfScreenHeight;
static fix16_t fixEps;

#ifdef TARGET_PC
static void* rlTexture;
static uint8_t* rlPixelBuf;
#else
static bool fpsUnlocked;
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

#endif

    pixelBuf = (cpColor*)malloc(sizeof(cpColor) * numPixels);
    memset(pixelBuf, 0, (int)sizeof(cpColor) * numPixels);
    cpSetTargetFPS(0);

    internalCamera3d = (cpInternalCamera3d) {0};

    fixHalfScreenWidth = fix16_from_int(screenWidth / 2);
    fixHalfScreenHeight = fix16_from_int(screenHeight / 2);
    fixNegHalfScreenHeight = fix16_from_int(-screenHeight / 2);
    fixEps = fix16_div(
        fix16_from_int(1),
        fix16_from_int(100)
    );
}

void cpQuit() {
#ifdef TARGET_PC
    rlwCloseWindow();
#else
    // reset clock speed
    cpg_set_pll_mul(CP_OC_MUL_DEFAULT);

    // restore lcd
    CALC_LCD_VRAMRestore();
    CALC_LCD_Refresh();

    // stop timers
    cmt_stop();
    POWER_MSTPCR0->CMT = 1;
#endif

    free(pixelBuf);
}

void cpSetTargetFPS(const int value) {
#ifdef TARGET_PC
    rlwSetTargetFPS(value);
#else
    if (value <= 0) {
        // fps unlocked
        POWER_MSTPCR0->CMT = 1; // disable timer
        fpsUnlocked = true;
    } else {
        POWER_MSTPCR0->CMT = 0; // enable timer
        cmt_set(CMT_TICKS_PER_SEC / value, CMT_MODE_ONE_SHOT, CMT_REQUEST_DISABLE);
        fpsUnlocked = false;
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

inline cpColor cpRGBToColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    // https://stackoverflow.com/a/11471397
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

// expects normalized vector
inline void cpVector3ToAngles(const cpVector3 v, fix16_t* yaw, fix16_t* pitch) {
    *yaw = fix16_atan2(v.z, v.x);
    *pitch = fix16_mul(fix16_asin(v.y), fix16_from_int(-1));
}

inline cpVector3 cpAnglesToVector3(const fix16_t yaw, const fix16_t pitch) {
    return (cpVector3) {
        fix16_mul(fix16_cos(pitch), fix16_cos(yaw)),
        fix16_sin(pitch),
        fix16_mul(fix16_cos(pitch), fix16_sin(yaw)),
    };
}

inline int cpGetScreenWidth() {
    return screenWidth;
}

inline int cpGetScreenHeight() {
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
    if (!fpsUnlocked)
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

    // set window title (no stdlib)
    char title[32] = "CPLib emu | FPS: ";

    size_t numDigits = 0;
    int fps = rlwGetFPS();
    int temp = fps;
    while (temp) {
        numDigits++;
        temp /= 10;
    }

    temp = fps;
    size_t endIdx = 17 + numDigits;
    size_t charIdx = endIdx - 1;
    while (temp) {
        title[charIdx--] = '0' + (temp % 10);
        temp /= 10;
    }

    title[endIdx] = 0;
    rlwSetWindowTitle(title);
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
    if (!fpsUnlocked)
        cmt_wait();
#endif
}

inline void cpClearBackground(const cpColor tint) {
    for (int i = 0; i < numPixels; i++)
        pixelBuf[i] = tint;
}

// no clipping
inline void cpDrawPixelUnsafe(const int x, const int y, const cpColor tint) {
    pixelBuf[y*screenWidth + x] = tint;
}

inline void cpDrawPixel(const int x, const int y, const cpColor tint) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
        pixelBuf[y*screenWidth + x] = tint;
}

static void cpDrawLineClippedSub(int x1, int y1, int x2, int y2, const cpColor tint) {
    // just a copy of the function below but with clipping
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

        // this is literally the only difference
        if (sx >= 0 && sx < screenWidth && sy >= 0 && sy < screenHeight)
            cpDrawPixelUnsafe(sx, sy, tint);
    }
}

void cpDrawLine(int x1, int y1, int x2, int y2, const cpColor tint) {
    if (x1 < 0 || x1 >= screenWidth || x2 < 0 || x2 >= screenWidth ||
        y1 < 0 || y1 >= screenHeight || y2 < 0 || y2 >= screenHeight) {
        cpDrawLineClippedSub(x1, y1, x2, y2, tint);
        return;
    }

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

        cpDrawPixelUnsafe(sx, sy, tint);
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
            cpDrawPixelUnsafe(sx, sy, tint);
        }
    }
}

void cpDrawCircle(const int centerX, const int centerY, const int radius, const cpColor tint) {
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
                cpDrawPixelUnsafe(i, centerY + y, tint);
            if (centerY - y >= 0 && centerY - y < screenHeight)
                cpDrawPixelUnsafe(i, centerY - y, tint);
        }

        for (int i = centerX - y; i <= centerX + y; i++) {
            if (i < 0 || i >= screenWidth) continue;

            if (centerY + x >= 0 && centerY + x < screenHeight)
                cpDrawPixelUnsafe(i, centerY + x, tint);
            if (centerY - x >= 0 && centerY - x < screenHeight)
                cpDrawPixelUnsafe(i, centerY - x, tint);
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

static void cpDrawTexture_RGB565(const cpTexture texture, const int x, const int y) {
    const int tw = x + texture.width >= screenWidth ? screenWidth - x : texture.width;
    const int th = y + texture.height >= screenHeight ? screenHeight - y : texture.height;

    for (int ty = y < 0 ? -y : 0; ty < th; ty++) {
        for (int tx = x < 0 ? -x : 0; tx < tw; tx++) {
            const uint32_t i = (ty * texture.width) + tx;
            const uint16_t color = ((uint16_t*)texture.data)[i];
            cpDrawPixelUnsafe(x + tx, y + ty, color);
        }
    }
}

static void cpDrawTexture_RGB565_A8(const cpTexture texture, const int x, const int y) {
    const int tw = x + texture.width >= screenWidth ? screenWidth - x : texture.width;
    const int th = y + texture.height >= screenHeight ? screenHeight - y : texture.height;

    for (int ty = y < 0 ? -y : 0; ty < th; ty++) {
        for (int tx = x < 0 ? -x : 0; tx < tw; tx++) {
            const uint32_t i = (ty * texture.width) + tx;
            const uint16_t color = *(uint16_t*)(texture.data + i*3);
            const bool alpha = *(bool*)(texture.data + i*3 + 2);
            if (alpha) cpDrawPixelUnsafe(x + tx, y + ty, color);
        }
    }
}

void cpDrawTexture(const cpTexture texture, const int x, const int y) {
    switch (texture.pixelFormat) {
        case CP_PIXEL_FORMAT_RGB565:
            cpDrawTexture_RGB565(texture, x, y);
            break;
        case CP_PIXEL_FORMAT_RGB565_A8:
            cpDrawTexture_RGB565_A8(texture, x, y);
            break;
    }
}

void cpRegisterCamera3d(const cpCamera3d camera) {
    internalCamera3d.position = camera.position;

    const cpVector3 cameraDirection = cpGetCamera3dDirection(camera);
    internalCamera3d.rotationMatrix = cpMatrix3Transpose(cpMatrix3CreateRotation(cameraDirection));

    internalCamera3d.focalLength = fix16_div(
        fixHalfScreenHeight,
        fix16_tan(fix16_mul(fix16_deg_to_rad(camera.fovY), fix16_from_str("0.5")))
    );
}

cpVector3 cpGetCamera3dDirection(const cpCamera3d camera) {
    return cpVector3Normalize(cpVector3Subtract(camera.target, camera.position));
}

void cpSetCamera3dDirection(cpCamera3d* camera, const cpVector3 rotation) {
    camera->target = cpVector3Add(camera->position, cpVector3Normalize(rotation));
}

cpVector3 cpWorldToCameraSpace(const cpVector3 pos) {
    const cpVector3 localPos = cpVector3Subtract(pos, internalCamera3d.position);
    return cpMatrix3MultiplyVector(internalCamera3d.rotationMatrix, localPos);
}

cpVector2 cpCameraToScreenSpace(const cpVector3 pos) {
    return (cpVector2) {
        fix16_add(fix16_mul(fix16_div(pos.x, pos.z), internalCamera3d.focalLength), fixHalfScreenWidth),
        fix16_sub(fixHalfScreenHeight, fix16_mul(fix16_div(pos.y, pos.z), internalCamera3d.focalLength))
    };
}

static void clipBehindCamera(cpVector3* start, const cpVector3 end) {
    const fix16_t zero = fix16_from_int(0);
    const fix16_t one = fix16_from_int(1);

    const fix16_t da = start->z;
    const fix16_t db = end.z;
    fix16_t d = fix16_sub(da, db); if (d == zero) d = one;
    const fix16_t s = fix16_div(da, d);
    start->x = fix16_add(start->x, fix16_mul(s, fix16_sub(end.x, start->x)));
    start->y = fix16_add(start->y, fix16_mul(s, fix16_sub(end.y, start->y)));
    start->z = fix16_add(start->z, fix16_mul(s, fix16_sub(end.z, start->z))); if (start->z == zero) start->z = one;
}

void cpDrawMesh(const cpMesh mesh, const cpVector3 offset, const cpMatrix3 transform, const cpColor tint) {
    cpVector3* coords = malloc(sizeof(cpVector3) * mesh.vertexCount);

    // apply mesh transform
    for (size_t i = 0; i < mesh.vertexCount; i++) {
        coords[i] = cpMatrix3MultiplyVector(transform, mesh.vertices[i]);
    }

    // transform to camera space
    for (size_t i = 0; i < mesh.vertexCount; i++) {
        coords[i] = cpWorldToCameraSpace(
            cpVector3Add(coords[i], offset)
        );
    }

    for (size_t i = 0; i < mesh.edgeCount; i++) {
        const cpMeshEdge edge = mesh.edges[i];
        cpVector3 start = coords[edge.a];
        cpVector3 end = coords[edge.b];

        // clip
        if (start.z < fixEps && end.z < fixEps)
            continue;

        if (start.z < fixEps) {
            clipBehindCamera(&start, end);
            start.z = fixEps;
        }
        else if (end.z < fixEps) {
            clipBehindCamera(&end, start);
            end.z = fixEps;
        }

        // screen space
        const cpVector2 screenCoordStart = cpCameraToScreenSpace(start);
        const cpVector2 screenCoordEnd = cpCameraToScreenSpace(end);

        cpDrawLine(
            fix16_to_int(screenCoordStart.x),
            fix16_to_int(screenCoordStart.y),
            fix16_to_int(screenCoordEnd.x),
            fix16_to_int(screenCoordEnd.y),
            tint
        );
    }

    free(coords);
}

void cpDrawPixel3d(const cpVector3 pos, const cpColor tint) {
    const cpVector3 camCoord = cpWorldToCameraSpace(pos);
    if (camCoord.z < fixEps)
        return;

    const cpVector2 screenCoordFix = cpCameraToScreenSpace(camCoord);
    const cpVector2 screenCoord = {
        fix16_to_int(screenCoordFix.x),
        fix16_to_int(screenCoordFix.y),
    };

    if (screenCoord.x < 0 || screenCoord.x >= screenWidth || screenCoord.y < 0 || screenCoord.y >= screenHeight)
        return;

    cpDrawPixelUnsafe(
        screenCoord.x,
        screenCoord.y,
        tint
    );
}

void cpDrawLine3d(const cpVector3 start, const cpVector3 end, const cpColor tint) {
    cpVector3 camStart = cpWorldToCameraSpace(start);
    cpVector3 camEnd = cpWorldToCameraSpace(end);

    // clip
    if (camStart.z < fixEps && camEnd.z < fixEps)
        return;

    if (camStart.z < fixEps) {
        clipBehindCamera(&camStart, camEnd);
        camStart.z = fixEps;
    }
    else if (camEnd.z < fixEps) {
        clipBehindCamera(&camEnd, camStart);
        camEnd.z = fixEps;
    }

    // screen space
    const cpVector2 screenCoordStart = cpCameraToScreenSpace(camStart);
    const cpVector2 screenCoordEnd = cpCameraToScreenSpace(camEnd);

    cpDrawLine(
        fix16_to_int(screenCoordStart.x),
        fix16_to_int(screenCoordStart.y),
        fix16_to_int(screenCoordEnd.x),
        fix16_to_int(screenCoordEnd.y),
        tint
    );
}

static int pointOrientation(cpVector2i a, cpVector2i b, cpVector2i c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static int pointOnSegment(cpVector2i a, cpVector2i b, cpVector2i p) {
    return p.x >= (a.x < b.x ? a.x : b.x) &&
           p.x <= (a.x > b.x ? a.x : b.x) &&
           p.y >= (a.y < b.y ? a.y : b.y) &&
           p.y <= (a.y > b.y ? a.y : b.y);
}

// probably not very optimized
bool cpCheckCollisionLines(const cpVector2i start1, const cpVector2i end1, const cpVector2i start2, const cpVector2i end2, cpVector2i* collisionPoint) {
    bool collision = false;

    const int o1 = pointOrientation(start1, end1, start2);
    const int o2 = pointOrientation(start1, end1, end2);
    const int o3 = pointOrientation(start2, end2, start1);
    const int o4 = pointOrientation(start2, end2, end1);

    if (o1 * o2 < 0 && o3 * o4 < 0) {
        collision = true;
    }

    if (o1 == 0 && pointOnSegment(start1, end1, start2)) collision = true;
    if (o2 == 0 && pointOnSegment(start1, end1, end2)) collision = true;
    if (o3 == 0 && pointOnSegment(start2, end2, start1)) collision = true;
    if (o4 == 0 && pointOnSegment(start2, end2, end1)) collision = true;

    if (!collision) {
        *collisionPoint = (cpVector2i) {0, 0};
        return false;
    }

    const int denom = (start1.x - end1.x) * (start2.y - end2.y) - (start1.y - end1.y) * (start2.x - end2.x);
    const int det1 = start1.x*end1.y - start1.y*end1.x;
    const int det2 = start2.x*end2.y - start2.y*end2.x;
    const int ix_num = det1*(start2.x - end2.x) - det2*(start1.x - end1.x);
    const int iy_num = det1*(start2.y - end2.y) - det2*(start1.y - end1.y);

    *collisionPoint = (cpVector2i) {
        fix16_to_int(fix16_div(fix16_from_int(ix_num), fix16_from_int(denom))),
        fix16_to_int(fix16_div(fix16_from_int(iy_num), fix16_from_int(denom))),
    };

    return true;
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
