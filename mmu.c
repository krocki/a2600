#include <stdio.h>

#include "mmu.h"
#include "tia.h"
#include "pia.h"

u8 *cart = _cart;
u8 *ram = _ram;
u8 *riot = _riot;

void w8(u16 a, u8 v) {
  a &= 0x1fff; // we only have 13 bits
  //printf("w8 %04x %02x\n", a, v);
  switch (a) {
    case 0x0000 ... 0x002c: // tia
      tia_w8(a & 0x3f, v);
      break;
    case 0x0200 ... 0x02ff:
      pia_w8(a & 0xff, v);
      break;
    case 0x0080 ... 0x00ff: // ram
      ram[a & 0x7f] = v;
      break;
    default: break;
  }
}

u8 r8(u16 a) {
  a &= 0x1fff; // we only have 13 bits
  switch (a) {
    case 0x0000 ... 0x003d: // tia
      return tia_r8(a & 0x0f);
    case 0x0080 ... 0x00ff: // ram
      return ram[a & 0x7f];
    case 0x0200 ... 0x02ff: // io
      return pia_r8(a & 0xff);
    case 0x1000 ... 0x1fff: // cart
      return cart[a & 0xfff];
    default: return 0x0;
  }
}

u16 r16_ok(u16 a) { return (r8(a) | (r8(a+1) << 8)); }

//version with the bug
u16 r16(u16 a) { u16 base=a & 0xff00; return (r8(a) | (r8(base|((u8)(a+1))) << 8)); }

