.macro DEFINE_OS_FUNC name, addr
.globl _\name
.set _\name, \addr
.endm


DEFINE_OS_FUNC CALC_LCD_ClearScreen 0x800394C0
DEFINE_OS_FUNC CALC_LCD_GetPixel 0x80039328
DEFINE_OS_FUNC CALC_LCD_GetSize 0x8002E140
DEFINE_OS_FUNC CALC_LCD_GetVRAMAddress 0x8002E154
DEFINE_OS_FUNC CALC_LCD_Refresh 0x8003733E
DEFINE_OS_FUNC CALC_LCD_SetPixel 0x80039302
DEFINE_OS_FUNC CALC_LCD_SetPixelFromPalette 0x8003931A
DEFINE_OS_FUNC CALC_LCD_VRAMBackup 0x8002D3FA
DEFINE_OS_FUNC CALC_LCD_VRAMRestore 0x8002D41A

DEFINE_OS_FUNC free 0x800A76FC
DEFINE_OS_FUNC malloc 0x800CFB00
DEFINE_OS_FUNC memcpy 0x800A78AC
DEFINE_OS_FUNC memset 0x800A7FC0

.global _CALC_getKey
_CALC_getKey:
	mov.l CALC_GetKey_keyAddr, r2 !Load the keyboard addr
	mov.l CALC_GetKey_and, r3
	mov.w @r2, r1   !Load first word in r1
	shll16 r1
	add #2, r2
	mov.w @r2, r0   !Load 2nd word in r0
	and r3, r0  !Just use the lower 16 bits
	add r0, r1  !Add both in r1
	mov.l r1, @r4   !Store in first argument
	add #2, r2
	mov.w @r2, r1   !Load 3rd word in r1
	shll16 r1
	add #2, r2
	mov.w @r2, r0   !Load 4th word in r0
	and r3, r0  !Just use the lower 16 bits
	add r0, r1  !Add both in r1
	mov.l r1, @r5   !Store in 2nd argument
	add #2, r2
	mov.w @r2, r1   !Load 3rd because otherwise the hardware doesn't work
	add #2, r2
	rts
	mov.w @r2, r1   !Load 4th because of the hardware.

.align 2
CALC_GetKey_keyAddr:
	.long 0xa44B0000
CALC_GetKey_and:
	.long 0x0000FFFF


