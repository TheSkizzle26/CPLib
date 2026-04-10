// https://github.com/diddyholz/CPDoom
// slightly modified
#pragma once

#include <stdint.h>

#define CMT_TICKS_PER_SEC 3600000

typedef enum {
  CMT_SIZE_32_BIT = 0x0,
  CMT_SIZE_16_BIT = 0x1,
} cmt_cmcsr_cms;

typedef enum {
  CMT_MODE_ONE_SHOT     = 0x0, // Timer will only run once
  CMT_MODE_FREE_RUNNING = 0x1, // Timer will restart after clearing flags
} cmt_cmcsr_cmm;

typedef enum {
  CMT_REQUEST_DISABLE = 0x0, // Disable DMA transfer request and internal interrupt request
  CMT_REQUEST_DMA     = 0x1, // Enable DMA transfer request
  CMT_REQUEST_IIR     = 0x2, // Enable internal interrupt request
} cmt_cmcsr_cmr;

typedef enum {
  CMT_CLOCK_RCLK_DIV_8   = 0x4,
  CMT_CLOCK_RCLK_DIV_32  = 0x5,
  CMT_CLOCK_RCLK_DIV_128 = 0x6,
} cmt_cmcsr_cks;

// Compare match timer start register
typedef union {
  struct
  {
    uint16_t _reserved0 : 10;
    uint16_t STR5       : 1; // Count start
    uint16_t _reserved1 : 5;
  };
  
  uint16_t raw;
} cmt_cmstr;

// Compare match timer control/status register
typedef union {
  struct
  {
    uint16_t CMF        : 1; // Compare Match Flag
    uint16_t OVF        : 1; // Overflow Flag
    uint16_t WRFLG      : 1; // Write State Flag
    uint16_t _reserved0 : 3;
    cmt_cmcsr_cms CMS   : 1; // Compare Match Timer Counter Size
    cmt_cmcsr_cmm CMM   : 1; // Compare Match Mode
    uint16_t CMTOUT_IE  : 1;
    uint16_t _reserved1 : 1;
    cmt_cmcsr_cmr CMR   : 2; // Compare Match Request
    uint16_t _reserved2 : 1;
    cmt_cmcsr_cks CKS   : 3; // Clock Select
  };
  
  uint16_t raw;
} cmt_cmcsr;

#define CMT_CMSTR ((volatile cmt_cmstr *)0xA44A0000)
#define CMT_CMCSR ((volatile cmt_cmcsr *)0xA44A0060)
#define CMT_CMCNT ((volatile uint32_t *) 0xA44A0064)
#define CMT_CMCOR ((volatile uint32_t *) 0xA44A0068)

inline void cmt_set(uint32_t constant, cmt_cmcsr_cmm cmm, cmt_cmcsr_cmr cmr)
{
  CMT_CMSTR->STR5 = 0;
  *CMT_CMCOR = constant;
  *CMT_CMCNT = 0;

  cmt_cmcsr temp_cmcsr = { .raw = 0 };
  temp_cmcsr.CMS = CMT_SIZE_32_BIT;
  temp_cmcsr.CMM = cmm;
  temp_cmcsr.CMR = cmr;
  temp_cmcsr.CKS = CMT_CLOCK_RCLK_DIV_8;

  CMT_CMCSR->raw = temp_cmcsr.raw;
}

inline void cmt_start()
{
  CMT_CMCSR->CMF = 0;
  CMT_CMSTR->STR5 = 1;
}

inline void cmt_stop()
{
  CMT_CMSTR->STR5 = 0;
  *CMT_CMCNT = 0;
  CMT_CMCSR->raw = 0;
}

inline void cmt_wait()
{
  while (!CMT_CMCSR->CMF) { }
}
