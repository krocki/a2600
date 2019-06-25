#include <stdio.h>
#include "pia.h"

u16 increments[4] = {1, 8, 64, 1024};

pia_regs_t pia_regs;

u16 incr=0;
u16 cnt=0;

void pia_w8(u8 a, u8 v) {
  u8 select = a-0x94;
  //printf("pia write : a %x, v %x\n", a, v);
  switch (a) {
    case 0x94 ... 0x97:
      incr=increments[select]; cnt=0;
      ((u8 *)&pia_regs)[0] = v;
      ((u8 *)&pia_regs)[select+2]=0;
      break;
    default:
      break;
  }
}

u8 pia_r8(u8 a) {
  switch (a) {
    case 0x84: return ((u8 *)&pia_regs)[0];
    default: return 0;
  }
}

void pia_step(u32 ticks) {
  for (u32 i=0; i<ticks; i++) {
    if (cnt==(incr-1)) {
      cnt=0;
      (pia_regs.INTIM)--;
    }
    cnt++;
  }
  //printf("pia : cnt %d, inc %d, intim %d\n", cnt, incr, pia_regs.INTIM);
}
