#ifdef TARGET_PC

#include "raylib_wrapper.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

void rlInitWindow(int width, int height, char* title) {
    InitWindow(width, height, title);
    SetWindowMonitor(0);
}

void rlCloseWindow() {
    CloseWindow();
}

void rlBeginDrawing() {
    BeginDrawing();
}

void rlEndDrawing() {
    EndDrawing();
}

void* rlCreateTexture(int width, int height) {
    Texture t = LoadTextureFromImage(GenImageColor(width, height, BLACK));

    void* ret = malloc(sizeof(Texture));
    memcpy(ret, &t, sizeof(Texture));

    return ret;
}

void rlUpdateTexture(void* texture, uint8_t* data) {
    UpdateTexture(*((Texture*)texture), data);
}

void rlDrawTexture(void* texture) {
    DrawTexture(
        *((Texture*)texture),
        0, 0,
        WHITE
    );
}

#endif