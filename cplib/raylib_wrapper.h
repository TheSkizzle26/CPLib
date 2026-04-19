#ifndef CPLIB_RAYLIB_WRAPPER_H
#define CPLIB_RAYLIB_WRAPPER_H

#ifdef TARGET_PC

#include <stdint.h>

typedef enum {
    RLW_KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    RLW_KEY_APOSTROPHE      = 39,       // Key: '
    RLW_KEY_COMMA           = 44,       // Key: ,
    RLW_KEY_MINUS           = 45,       // Key: -
    RLW_KEY_PERIOD          = 46,       // Key: .
    RLW_KEY_SLASH           = 47,       // Key: /
    RLW_KEY_ZERO            = 48,       // Key: 0
    RLW_KEY_ONE             = 49,       // Key: 1
    RLW_KEY_TWO             = 50,       // Key: 2
    RLW_KEY_THREE           = 51,       // Key: 3
    RLW_KEY_FOUR            = 52,       // Key: 4
    RLW_KEY_FIVE            = 53,       // Key: 5
    RLW_KEY_SIX             = 54,       // Key: 6
    RLW_KEY_SEVEN           = 55,       // Key: 7
    RLW_KEY_EIGHT           = 56,       // Key: 8
    RLW_KEY_NINE            = 57,       // Key: 9
    RLW_KEY_SEMICOLON       = 59,       // Key: ;
    RLW_KEY_EQUAL           = 61,       // Key: =
    RLW_KEY_A               = 65,       // Key: A | a
    RLW_KEY_B               = 66,       // Key: B | b
    RLW_KEY_C               = 67,       // Key: C | c
    RLW_KEY_D               = 68,       // Key: D | d
    RLW_KEY_E               = 69,       // Key: E | e
    RLW_KEY_F               = 70,       // Key: F | f
    RLW_KEY_G               = 71,       // Key: G | g
    RLW_KEY_H               = 72,       // Key: H | h
    RLW_KEY_I               = 73,       // Key: I | i
    RLW_KEY_J               = 74,       // Key: J | j
    RLW_KEY_K               = 75,       // Key: K | k
    RLW_KEY_L               = 76,       // Key: L | l
    RLW_KEY_M               = 77,       // Key: M | m
    RLW_KEY_N               = 78,       // Key: N | n
    RLW_KEY_O               = 79,       // Key: O | o
    RLW_KEY_P               = 80,       // Key: P | p
    RLW_KEY_Q               = 81,       // Key: Q | q
    RLW_KEY_R               = 82,       // Key: R | r
    RLW_KEY_S               = 83,       // Key: S | s
    RLW_KEY_T               = 84,       // Key: T | t
    RLW_KEY_U               = 85,       // Key: U | u
    RLW_KEY_V               = 86,       // Key: V | v
    RLW_KEY_W               = 87,       // Key: W | w
    RLW_KEY_X               = 88,       // Key: X | x
    RLW_KEY_Y               = 89,       // Key: Y | y
    RLW_KEY_Z               = 90,       // Key: Z | z
    RLW_KEY_LEFT_BRACKET    = 91,       // Key: [
    RLW_KEY_BACKSLASH       = 92,       // Key: '\'
    RLW_KEY_RIGHT_BRACKET   = 93,       // Key: ]
    RLW_KEY_GRAVE           = 96,       // Key: `
    // Function keys
    RLW_KEY_SPACE           = 32,       // Key: Space
    RLW_KEY_ESCAPE          = 256,      // Key: Esc
    RLW_KEY_ENTER           = 257,      // Key: Enter
    RLW_KEY_TAB             = 258,      // Key: Tab
    RLW_KEY_BACKSPACE       = 259,      // Key: Backspace
    RLW_KEY_INSERT          = 260,      // Key: Ins
    RLW_KEY_DELETE          = 261,      // Key: Del
    RLW_KEY_RIGHT           = 262,      // Key: Cursor right
    RLW_KEY_LEFT            = 263,      // Key: Cursor left
    RLW_KEY_DOWN            = 264,      // Key: Cursor down
    RLW_KEY_UP              = 265,      // Key: Cursor up
    RLW_KEY_PAGE_UP         = 266,      // Key: Page up
    RLW_KEY_PAGE_DOWN       = 267,      // Key: Page down
    RLW_KEY_HOME            = 268,      // Key: Home
    RLW_KEY_END             = 269,      // Key: End
    RLW_KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    RLW_KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    RLW_KEY_NUM_LOCK        = 282,      // Key: Num lock
    RLW_KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    RLW_KEY_PAUSE           = 284,      // Key: Pause
    RLW_KEY_F1              = 290,      // Key: F1
    RLW_KEY_F2              = 291,      // Key: F2
    RLW_KEY_F3              = 292,      // Key: F3
    RLW_KEY_F4              = 293,      // Key: F4
    RLW_KEY_F5              = 294,      // Key: F5
    RLW_KEY_F6              = 295,      // Key: F6
    RLW_KEY_F7              = 296,      // Key: F7
    RLW_KEY_F8              = 297,      // Key: F8
    RLW_KEY_F9              = 298,      // Key: F9
    RLW_KEY_F10             = 299,      // Key: F10
    RLW_KEY_F11             = 300,      // Key: F11
    RLW_KEY_F12             = 301,      // Key: F12
    RLW_KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    RLW_KEY_LEFT_CONTROL    = 341,      // Key: Control left
    RLW_KEY_LEFT_ALT        = 342,      // Key: Alt left
    RLW_KEY_LEFT_SUPER      = 343,      // Key: Super left
    RLW_KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    RLW_KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    RLW_KEY_RIGHT_ALT       = 346,      // Key: Alt right
    RLW_KEY_RIGHT_SUPER     = 347,      // Key: Super right
    RLW_KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    RLW_KEY_KP_0            = 320,      // Key: Keypad 0
    RLW_KEY_KP_1            = 321,      // Key: Keypad 1
    RLW_KEY_KP_2            = 322,      // Key: Keypad 2
    RLW_KEY_KP_3            = 323,      // Key: Keypad 3
    RLW_KEY_KP_4            = 324,      // Key: Keypad 4
    RLW_KEY_KP_5            = 325,      // Key: Keypad 5
    RLW_KEY_KP_6            = 326,      // Key: Keypad 6
    RLW_KEY_KP_7            = 327,      // Key: Keypad 7
    RLW_KEY_KP_8            = 328,      // Key: Keypad 8
    RLW_KEY_KP_9            = 329,      // Key: Keypad 9
    RLW_KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    RLW_KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    RLW_KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    RLW_KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    RLW_KEY_KP_ADD          = 334,      // Key: Keypad +
    RLW_KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    RLW_KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    RLW_KEY_BACK            = 4,        // Key: Android back button
    RLW_KEY_MENU            = 5,        // Key: Android menu button
    RLW_KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    RLW_KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} rlwKeyboardKey;


void rlwInitWindow(int width, int height, char* title);
void rlwCloseWindow();
void rlwSetWindowTitle(char* title);
int rlwGetFPS();
void rlwSetTargetFPS(int value);
void rlwBeginDrawing();
void rlwEndDrawing();
int rlwGetTextureWidth(void* texture);
int rlwGetTextureHeight(void* texture);
void* rlwCreateTexture(int width, int height);
void rlwUpdateTexture(void* texture, uint8_t* data);
void rlwDrawTexture(void* texture);
bool rlwIsKeyDown(uint32_t keycode);
int rlwGetMouseX();
int rlwGetMouseY();
bool rlwIsMouseButtonDown();


#endif

#endif