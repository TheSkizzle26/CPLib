/*
 * Basically just raylib for the classpad II.
 * Can't do much at the moment.
 * Edit: can actually do quite a bit now.
 *
 * Also doesn't require the hollyhock-2 SDK, as it is included in this lib.
 *
 * If you would like to use this in another project,
 * just copy the cplib.*, cplib_os.s, raylib_wrapper.*, Makefile and linker.d
 * files to your project.
 *
 * Don't know how to get newlib working though.
 */


#include "cplib/cplib.h"

APP_NAME("CPLib test")
APP_DESCRIPTION("CPLib test program")
APP_AUTHOR("TheSkizzle")
APP_VERSION("0.0.0")


static bool is_running;
static int tick;


void update() {
    if (cpIsKeyPressed(KEY_CLEAR)) {
        is_running = false;
    }
}

void render() {
    cpBeginDrawing();
    cpClearBackground(BLACK);

    cpDrawLine(0, 0, 100, 100, RED);
    cpDrawRectangle(tick, 250, 100, 50, SKYBLUE);
    cpDrawLine(0, 0, tick, 0, WHITE);
    cpDrawCircle(150, 100, 30, WHITE);

    cpEndDrawing();
}

int main() {
    cpInit();
    cpSetTargetFPS(60);
    cpSetOverclock(OC_MUL_48);

    is_running = true;

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (is_running) {
        update();
        render();

        tick++;
    }

    cpQuit();
    return 0;
}
