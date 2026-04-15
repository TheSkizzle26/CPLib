# CPLib
A library for drawing, input, etc. for the ClassPad II calculator.\
Runs on both the calculator and your PC!

# Use
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
- vector arithmetic
- also includes libfixmath for float vectors

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
    return true;
}
```

# Additional Features
Some features are disabled by default. Enable them by adding the appropriate definition
before you include CPLib.

**CPLIB_IMPLEMENT_VECTOR3I:** Implement cpVector3i struct and arithmetic functions.\
**CPLIB_IMPLEMENT_VECTOR4:** Implement cpVector4 struct and arithmetic functions.\
**CPLIB_IMPLEMENT_MATRIX4:** Implement cpMatrix4 struct and arithmetic functions. *Automatically enables vector4 as well*.

# TODOs:
- **use vectors for rendering instead of yaw and pitch**
- add CALC_* prefix to cmt, power, etc.
- use dmac for rendering
- add clipping for some rendering functions