#ifdef TARGET_PC

#include "raylib_wrapper.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>


void rlwInitWindow(int width, int height, char* title) {
    InitWindow(width, height, title);
    SetWindowMonitor(0);
}

void rlwCloseWindow() {
    CloseWindow();
}

void rlwBeginDrawing() {
    BeginDrawing();
}

void rlwEndDrawing() {
    EndDrawing();
}

int rlwGetTextureWidth(void* texture) {
    return ((Texture*)texture)->width;
}

int rlwGetTextureHeight(void* texture) {
    return ((Texture*)texture)->height;
}

void* rlwCreateTexture(int width, int height) {
    Image img = GenImageColor(width, height, WHITE);
    Texture* t = malloc(sizeof(Texture));
    *t = LoadTextureFromImage(img);
    UnloadImage(img);
    return t;
}

void rlwUpdateTexture(void* texture, uint8_t* data) {
    Texture* t = (Texture*)texture;
    UpdateTexture(*t, data);
}

void rlwDrawTexture(void* texture) {
    DrawTexture(
        *(Texture*)texture,
        0, 0,
        WHITE
    );
}

bool rlIsKeyDown(uint32_t keycode) {
    return IsKeyDown(keycode);
}

#endif