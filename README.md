# CPLib
A library for drawing, input, etc. for the ClassPad II calculator.\
Runs on both the calculator and your PC!

# How to use
Just copy the "cplib" folder into your project.\
Also take a look at linker.ld, Makefile and of course example.c.\
The 3D rendering works a bit differently than raylib's. Just take a look at the 3d part in cplib/cplib.h. \
All the 3D functions use floats, the 2D ones ints.\
If you want to run this on your PC, you will need to have raylib installed.

# Features:
- easy to use
- very similar to raylib
- 2d and 3d rendering
- shape drawing
- vectors
- matrices
- full file system access (uses an emulated folder on the PC)
- simple font rendering
- also includes libfixmath for float vectors
- lots of features are toggleable for smaller binaries

# A simple example
```C
#include "cplib/cplib.h"


int main() {
    cpInit();
    cpSetTargetFPS(20);

    while (true) {
        if (cpIsKeyPressed(CP_KEY_CLEAR))
            break;
        
        cpBeginDrawing();
        cpClearBackground(SKYBLUE);
        cpEndDrawing();
    }

    cpQuit();
    return 0;
}
```

# Additional Features
Some features are disabled by default. Enable them by adding the appropriate definition
in your makefile.

**CPLIB_ENABLE_3D:** Enable all 3d-related functionality.\
**CPLIB_ENABLE_VECTOR3I:** Enable cpVector3i struct and arithmetic functions.\
**CPLIB_ENABLE_VECTOR4:** Enable cpVector4 struct and arithmetic functions.\
**CPLIB_ENABLE_MATRIX4:** Enable cpMatrix4 struct and arithmetic functions.\
**CPLIB_ENABLE_NOFRAMEBUF:** Write to VRAM instead of separate frame buffer. cpGetFramebuffer() will return the VRAM address. Might be a bit slower but will save a lot of RAM.\
**CPLIB_ENABLE_FILE:** Enable file system access functions.\
**CPLIB_ENABLE_FONT:** Enable font drawing functions.

You might need to define these before including cplib.h in order for your IDE to recognize it.

# TODOs:
- add CALC_* prefix to cmt, power, etc.
- Lower binary size.
- Add touchscreen stuff
- Add GUI functions
- implement sprintf since we don't have stdlib
- clean up some of the code, put all declarations in one header
