#ifndef CPLIB_RAYLIB_WRAPPER_H
#define CPLIB_RAYLIB_WRAPPER_H

#ifdef TARGET_PC

#include <stdint.h>


void rlInitWindow(int width, int height, char* title);
void rlCloseWindow();
void rlBeginDrawing();
void rlEndDrawing();
void* rlCreateTexture(int width, int height);
void rlUpdateTexture(void* texture, uint8_t* data);
void rlDrawTexture(void* texture);


#endif

#endif