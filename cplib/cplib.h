//
// Created by timo on 09.04.26.
//

#ifndef CPLIB_CPLIB_H
#define CPLIB_CPLIB_H

#include "common.h"
#include "rect.h"
#include "vector.h"
#include "matrix.h"
#include "rand.h"
#include "file.h"
#include "font.h"


#ifdef TARGET_PC
#include <stdlib.h>
#include <string.h>
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


// literally just raylib's colors
#define CP_LIGHTGRAY 52825
#define CP_GRAY 33808
#define CP_DARKGRAY 21130
#define CP_YELLOW 65472
#define CP_GOLD 65088
#define CP_ORANGE 64768
#define CP_PINK 64376
#define CP_RED 57670
#define CP_MAROON 47366
#define CP_GREEN 1830
#define CP_LIME 1253
#define CP_DARKGREEN 933
#define CP_SKYBLUE 26111
#define CP_BLUE 990
#define CP_DARKBLUE 661
#define CP_PURPLE 52191
#define CP_VIOLET 33271
#define CP_DARKPURPLE 28911
#define CP_BEIGE 54672
#define CP_BROWN 31561
#define CP_DARKBROWN 18917
#define CP_WHITE 65535
#define CP_BLACK 0
#define CP_BLANK 0
#define CP_MAGENTA 63519
#define CP_RAYWHITE 63422

typedef enum {
    CP_OC_MUL_12 = 0x05,
    CP_OC_MUL_16 = 0x07,
    CP_OC_MUL_24 = 0x0B,
    CP_OC_MUL_30 = 0x0E,
    CP_OC_MUL_32 = 0x0F,
    CP_OC_MUL_36 = 0x11,
    CP_OC_MUL_48 = 0x17,

    CP_OC_MUL_DEFAULT = 0x0F, // same as OC_MUL_32
} cpOverclockMultipliers;

typedef enum {
    CP_KEY_SHIFT		= 0,
    CP_KEY_CLEAR		= 1, //The Power key
    CP_KEY_BACKSPACE	= 2,
    CP_KEY_LEFT		= 3,
    CP_KEY_RIGHT		= 4,
    CP_KEY_Z			= 5,
    CP_KEY_POWER		= 6, //The exponent key
    CP_KEY_DIVIDE		= 7,
    CP_KEY_MULTIPLY	= 8,
    CP_KEY_SUBTRACT	= 9,
    CP_KEY_ADD			= 10,
    CP_KEY_EXE			= 11,
    CP_KEY_EXP			= 12,
    CP_KEY_3			= 13,
    CP_KEY_6			= 14,
    CP_KEY_9			= 15,

    CP_KEY_KEYBOARD	= 16,
    CP_KEY_UP			= 17,
    CP_KEY_DOWN		= 18,
    CP_KEY_EQUALS		= 19,
    CP_KEY_X			= 20,
    CP_KEY_Y			= 21,
    CP_KEY_LEFT_BRACKET= 22,
    CP_KEY_RIGHT_BRACKET= 23,
    CP_KEY_COMMA		= 24,
    CP_KEY_NEGATIVE	= 25,
    CP_KEY_0			= 26,
    CP_KEY_DOT			= 27,
    CP_KEY_1			= 28,
    CP_KEY_2			= 29,
    CP_KEY_4			= 30,
    CP_KEY_5			= 31,
    CP_KEY_7			= 32,
    CP_KEY_8			= 33,
} cpKeyIndices;


typedef enum {
    CP_PIXEL_FORMAT_RGB565 = 0, // RGB565 -> 2 bytes
    CP_PIXEL_FORMAT_RGB565_A8 = 1 // RGB565 + 1 byte alpha (bool) -> 3 bytes
} cpPixelFormat;

typedef struct {
    int width, height;
    cpPixelFormat pixelFormat;
    void* data;
} cpTexture;

#ifdef CPLIB_ENABLE_3D

typedef struct {
    cpVector3 position;
    cpVector3 target; // global (e.g. position + rotation)
    fix16_t fovY;
} cpCamera3d;

typedef struct {
    size_t a, b;
} cpMeshEdge;

typedef struct {
    size_t vertexCount; // vertices
    cpVector3* vertices;
    size_t edgeCount; // edges
    cpMeshEdge* edges;
} cpMesh;

#endif


// In the header so some things can be inlined.

extern int cpInternalScreenWidth, cpInternalScreenHeight;
extern cpColor* cpInternalPixelBuf __attribute__((aligned(32))); // alignment for faster memcpy


// general

void cpInit(); // Initialize CPLib.
void cpQuit(); // Finish CPLib.

void cpSetTargetFPS(int value); // Set the target FPS.
void cpSetOverclock(cpOverclockMultipliers mul); // Set the overclock multiplier.

// utilities

// Convert RGB values to a color.
inline cpColor cpRGBToColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    // https://stackoverflow.com/a/11471397
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

void cpVector3ToAngles(cpVector3 v, fix16_t* yaw, fix16_t* pitch) [[unsequenced]]; // Convert a cpVector3 to individual angles.
cpVector3 cpAnglesToVector3(fix16_t yaw, fix16_t pitch) [[unsequenced]];// Convert individual angles to a cpVector3.

int cpGetScreenWidth() [[unsequenced]]; // Get the screen's width.
int cpGetScreenHeight() [[unsequenced]]; // Get the screen's height.
uint16_t* cpGetFramebuffer() [[unsequenced]]; // Get a pointer to the framebuffer.

// 2d drawing functions

void cpBeginDrawing(); // Initialize drawing.
void cpEndDrawing(); // Finish drawing.

void cpClearBackground(cpColor tint); // Clear the background using a given color.

// Draw a single pixel. No clipping.
// If you draw outside the screen, you might corrupt memory.
static inline void cpDrawPixelUnsafe(const int x, const int y, const cpColor tint) {
    cpInternalPixelBuf[y*cpInternalScreenWidth + x] = tint;
}

// Draw a single pixel.
static inline void cpDrawPixel(const int x, const int y, const cpColor tint) {
    if (x >= 0 && x < cpInternalScreenWidth && y >= 0 && y < cpInternalScreenHeight)
        cpInternalPixelBuf[y*cpInternalScreenWidth + x] = tint;
}

void cpDrawLine(int x1, int y1, int x2, int y2, cpColor tint); // Draw a line.
void cpDrawRectangle(int x, int y, int w, int h, cpColor tint); // Draw a rectangle.
void cpDrawCircle(int centerX, int centerY, int radius, cpColor tint); // Draw a circle.
void cpDrawTexture(cpTexture texture, int x, int y); // Draw a texture.

// 3d drawing functions
#ifdef CPLIB_ENABLE_3D

void cpRegisterCamera3d(cpCamera3d camera); // Register the camera to use for drawing. Must be called before 3d rendering every frame.
cpVector3 cpGetCamera3dDirection(cpCamera3d camera) [[unsequenced]]; // Get the direction a camera is facing in.
void cpSetCamera3dDirection(cpCamera3d* camera, cpVector3 rotation); // Set the direction a camera is facing in.

cpVector3 cpWorldToCameraSpace(cpVector3 pos) [[unsequenced]]; // Transform a point from world to camera space.
cpVector2 cpCameraToScreenSpace(cpVector3 pos) [[unsequenced]]; // Transform a point from camera to screen space.

void cpDrawMesh(cpMesh mesh, cpVector3 offset, cpMatrix3 transform, cpColor tint); // Draw a mesh using a given transformation matrix.
void cpDrawPixel3d(cpVector3 pos, cpColor tint); // Draw a 3d pixel.
void cpDrawLine3d(cpVector3 start, cpVector3 end, cpColor tint); // Draw a 3d line.
void cpDrawCircle3d(cpVector3 center, fix16_t radius, cpColor tint); // Draw a 3d circle.

#endif

// collision detection functions (very bad)

bool cpCheckCollisionLines(cpVector2i start1, cpVector2i end1, cpVector2i start2, cpVector2i end2, cpVector2i* collisionPoint); // Check if two lines collide. If yes, store the collision point.

// input functions

bool cpIsKeyDown(cpKeyIndices keyIdx) [[unsequenced]]; // Is a key down?
bool cpIsKeyPressed(cpKeyIndices keyIdx) [[unsequenced]]; // Has a key just been pressed?
bool cpIsKeyUp(cpKeyIndices keyIdx) [[unsequenced]]; // Is a key up?
bool cpIsKeyReleased(cpKeyIndices keyIdx) [[unsequenced]]; // Has a key just been released?

bool cpIsTouchDown() [[unsequenced]]; // Is the screen touched?
bool cpIsTouchPressed() [[unsequenced]]; // Has the screen just been touched?
bool cpIsTouchUp() [[unsequenced]]; // Is the screen not touched?
bool cpIsTouchReleased() [[unsequenced]]; // Has the screen just been untouched?
int cpGetTouchX() [[unsequenced]]; // Get the touch x position, 0 if not touched.
int cpGetTouchY() [[unsequenced]]; // Get the touch y position, 0 if not touched.
int cpGetTouchDeltaX() [[unsequenced]]; // Get the difference between the current and last touch x.
int cpGetTouchDeltaY() [[unsequenced]]; // Get the difference between the current and last touch y.


#endif //CPLIB_CPLIB_H
