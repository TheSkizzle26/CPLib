# CPLib
A library for drawing, input, etc. for the ClassPad II calculator.\
Also runs on both the calculator and your PC!

# Use
Just copy the "cplib" folder into your project.\
Also take a look at linker.ld, Makefile and of course example.c.\
The 3D rendering works a bit differently than raylib's. Just take a look at the 3d part in cplib/cplib.h. \
All the 3D functions use floats, the 2D ones ints.

# Features:
- very similar to raylib
- 2d and (soon) 3d rendering
- shape drawing
- vector arithmetic
- also includes libfixmath for float vectors
- try to use cpVector*i whenever possible, as floats are to be software-emulated

# TODOs:
- add CALC_* prefix to cmt, power, etc.
- use dmac for rendering
- add clipping for some rendering functions