#include "mmu.h"

void w8(u16 a, u8 v) {
  //switch (a) {
  //  case 0x0000 ... 0x1fff: ram[a & 0x7ff] = v; break;
  //  case 0x2000 ... 0x3fff: write_ppu_regs(a & 0x7,v); break;
  //  case 0x4014: dma_transfer(v); break; // dma
  //  case 0x4016: break; // joypad strobe
  //  case 0x6000 ... 0xffff: break;
  //}
}

u8 r8(u16 a) {
  //switch (a) {
  //  case 0x0000 ... 0x1fff: return ram[a & 0x7ff];
  //  case 0x2000 ... 0x3fff: return read_ppu_regs(a & 0x7);
  //  case 0x4014: break; // dma
  //  case 0x4016: break; // joypad 0
  //  case 0x4017: break; // joypad 1
  //  case 0x6000 ... 0x7fff: break;
  //  case 0x8000 ... 0xffff: return prg[a & 0x3fff];
  //}
}

u16 r16_ok(u16 a) { return (r8(a) | (r8(a+1) << 8)); }

//version with the bug
u16 r16(u16 a) { u16 base=a & 0xff00; return (r8(a) | (r8(base|((u8)(a+1))) << 8)); }

