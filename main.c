#include "cplib.h"

APP_NAME("CPLib test")
APP_DESCRIPTION("CPLib test program")
APP_AUTHOR("TheSkizzle")
APP_VERSION("0.0.0")


int main() {
    cpInit();

    for (int i = 0; i < 50000; i++) {
        cpBeginDrawing();
        cpClearBackground(BLACK);
        cpEndDrawing();
    }

    cpQuit();
    return 0;
}