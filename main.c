/*
 * Basically just raylib for the classpad II.
 * Can't do much at the moment.
 *
 * If you would like to use this in another project,
 * just copy the cplib.*, cplib_os.s, raylib_wrapper.*, Makefile and linker.d
 * files to your project.
 *
 * Don't know how to get newlib working though.
 */


#include "cplib.h"

APP_NAME("CPLib test")
APP_DESCRIPTION("CPLib test program")
APP_AUTHOR("TheSkizzle")
APP_VERSION("0.0.0")


int main() {
    cpInit();

    // ReSharper disable once CppDFAEndlessLoop
    while (true) {
        // update
        if (cpIsKeyPressed(KEY_CLEAR)) {
            break;
        }

        // render
        cpBeginDrawing();
        cpClearBackground(BLACK);

        for (int j = 0; j < 100; j++)
            cpDrawPixel(j, j, RED);

        cpDrawRectangle(
            150,
            250,
            100,
            50,
            SKYBLUE
        );

        cpEndDrawing();
    }

    cpQuit();
    return 0;
}