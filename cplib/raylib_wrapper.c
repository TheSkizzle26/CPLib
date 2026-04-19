#ifdef TARGET_PC

#include "raylib_wrapper.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>


inline void rlwInitWindow(int width, int height, char* title) {
    InitWindow(width, height, title);
    SetWindowMonitor(0);
}

inline void rlwCloseWindow() {
    CloseWindow();
}

inline void rlwSetWindowTitle(char* title) {
    SetWindowTitle(title);
}

inline int rlwGetFPS() {
    return GetFPS();
}

inline void rlwSetTargetFPS(int value) {
    SetTargetFPS(value);
}

inline void rlwBeginDrawing() {
    BeginDrawing();
}

inline void rlwEndDrawing() {
    EndDrawing();
}

inline int rlwGetTextureWidth(void* texture) {
    return ((Texture*)texture)->width;
}

inline int rlwGetTextureHeight(void* texture) {
    return ((Texture*)texture)->height;
}

void* rlwCreateTexture(int width, int height) {
    Image img = GenImageColor(width, height, WHITE);
    Texture* t = malloc(sizeof(Texture));
    *t = LoadTextureFromImage(img);
    UnloadImage(img);
    return t;
}

inline void rlwUpdateTexture(void* texture, uint8_t* data) {
    Texture* t = (Texture*)texture;
    UpdateTexture(*t, data);
}

inline void rlwDrawTexture(void* texture) {
    DrawTexture(
        *(Texture*)texture,
        0, 0,
        WHITE
    );
}

inline bool rlwIsKeyDown(uint32_t keycode) {
    return IsKeyDown(keycode);
}

inline int rlwGetMouseX() {
    return GetMouseX();
}

inline int rlwGetMouseY() {
    return GetMouseY();
}

inline bool rlwIsMouseButtonDown() {
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}


#endif