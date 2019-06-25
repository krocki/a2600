#include "6502/typedefs.h"

typedef struct {
  u8 INTIM;     // 0x284 R
  u8 INSTAT;    // 0x285 R
  u8 TIM1T;     // 0x294 W
  u8 TIM8T;     // 0x295 W
  u8 TIM64T;    // 0x296 W
  u8 TIM1024T;  // 0x297 W
} pia_regs_t;

extern void pia_w8(u8, u8);
extern u8 pia_r8(u8);
extern void pia_step(u32 ticks);
