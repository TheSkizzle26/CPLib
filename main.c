/*
 * Basically just raylib for the classpad II.
 * Can't do much at the moment.
 * Edit: can actually do quite a bit now.
 *
 * Also doesn't require the hollyhock-2 SDK, as it is included in this lib.
 *
 * If you would like to use this in another project,
 * just copy the cplib folder to your project and take a look
 * at this example script. You may also want to copy linker.ld and Makefile.
 */


#include "cplib/cplib.h"

APP_NAME("CPLib test")
APP_DESCRIPTION("CPLib test program")
APP_AUTHOR("TheSkizzle")
APP_VERSION("0.0.0")


static bool isRunning;
static int tick;


void update() {
    if (cpIsKeyPressed(KEY_CLEAR)) {
        isRunning = false;
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

    isRunning = true;

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (isRunning) {
        update();
        render();

        tick++;
    }

    cpQuit();
    return 0;
}
