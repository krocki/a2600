#include <stdlib.h>
#include <stdio.h>
#include "tia.h"

u8 col=0;
u16 line=0;
u8 hblank;
u8 vblank;
u8 overscan;
u8 vsync;
u8 draw;

u64 tia_cycles = 0;

//u32 pal[2] = {0x0000ff, 0xffffff}; //b/w palette
u32 pal[] = {
  0x000000, 0x404040, 0x6c6c6c, 0x909090, 0xb0b0b0, 0xc8c8c8, 0xdcdcdc, 0xf4f4f4,
  0x444400, 0x646410, 0x848424, 0xa0a034, 0xb8b840, 0xd0d050, 0xe8e85c, 0xfcfc68,
  0x702800, 0x844414, 0x985c28, 0xac783c, 0xbc8c4c, 0xcca05c, 0xdcb468, 0xecc878,
  0x841800, 0x983418, 0xac5030, 0xc06848, 0xd0805c, 0xe09470, 0xeca880, 0xfcbc94,
  0x880000, 0x9c2020, 0xb03c3c, 0xc05858, 0xd07070, 0xe08888, 0xeca0a0, 0xfcb4b4,
  0x78005c, 0x8c2074, 0xa03c88, 0xb0589c, 0xc070b0, 0xd084c0, 0xdc9cd0, 0xecb0e0,
  0x480078, 0x602090, 0x783ca4, 0x8c58b8, 0xa070cc, 0xb484dc, 0xc49cec, 0xd4b0fc,
  0x140084, 0x302098, 0x4c3cac, 0x6858c0, 0x7c70d0, 0x9488e0, 0xa8a0ec, 0xbcb4fc,
  0x000088, 0x1c209c, 0x3840b0, 0x505cc0, 0x6874d0, 0x7c8ce0, 0x90a4ec, 0xa4b8fc,
  0x00187c, 0x1c3890, 0x3854a8, 0x5070bc, 0x6888cc, 0x7c9cdc, 0x90b4ec, 0xa4c8fc,
  0x002c5c, 0x1c4c78, 0x386890, 0x5084ac, 0x689cc0, 0x7cb4d4, 0x90cce8, 0xa4e0fc,
  0x003c2c, 0x1c5c48, 0x387c64, 0x509c80, 0x68b494, 0x7cd0ac, 0x90e4c0, 0xa4fcd4,
  0x003c00, 0x205c20, 0x407c40, 0x5c9c5c, 0x74b474, 0x8cd08c, 0xa4e4a4, 0xb8fcb8,
  0x143800, 0x345c1c, 0x507c38, 0x6c9850, 0x84b468, 0x9ccc7c, 0xb4e490, 0xc8fca4,
  0x2c3000, 0x4c501c, 0x687034, 0x848c4c, 0x9ca864, 0xb4c078, 0xccd488, 0xe0ec9c,
  0x442800, 0x644818, 0x846830, 0xa08444, 0xb89c58, 0xd0b46c, 0xe8cc7c, 0xfce08c
};

u8 pix[IM_W * IM_H * 3];
tia_regs_t tia_regs;

void tia_reset() {
  tia_regs.WSYNC = 0xff;
}

void tia_step(u32 ticks) {

  for (u32 i=0; i<ticks; i++) {

    vsync=line>=0 && line<3;
    vblank=line>=3 && line<40;
    s16 x=col-68; s16 y=line-40;
    hblank=y>=0 && y<IM_H && x<0;
    overscan=y>=IM_H;
    draw=col > 67 && line > 39 && col < 232;

    if (tia_regs.VSYNC) { line=0; col=0; tia_regs.VSYNC=0;}

    if (draw) {
      u32 rgb_color;
      u8 color;
#if TEST_SCREEN
      rgb_color=pal[rand() % 128];
#else
      int offset = x/4;
      u8 pf = 0;
      if ((tia_regs.CTRLPF & 0x1) && offset > 19)
           { offset = 19 - (offset % 20); }
      else { offset %= 20; }
      if      (offset < 4 ) { pf = (tia_regs.PF0 >> (offset + 4))  & 0x1; } // read playfield 0
      else if (offset < 12) { pf = (tia_regs.PF1 >> (11 - offset)) & 0x1; } // read playfield 1
      else                  { pf = (tia_regs.PF2 >> (offset - 12)) & 0x1; } // read playfield 2
      if (pf) {
        if ((tia_regs.CTRLPF & 0x2) == 0x2) {
          color=(offset < 20) ? tia_regs.COLUP0 : tia_regs.COLUP1; //return P0 or P1
        } else { color=tia_regs.COLUPF; } // playfield
      }
      else {
        color=tia_regs.COLUBK;
        } // not playfield

      // convert
      rgb_color=pal[color/2];
#endif
      if (x>=0 && x<IM_W && y>=0 && y<IM_H) {
        pix[3*(x+y*IM_W)+0] = (rgb_color >> 16);
        pix[3*(x+y*IM_W)+1] = (rgb_color >> 8) & 0xff;
        pix[3*(x+y*IM_W)+2] = (rgb_color) & 0xff;
      }
    }

    //printf("col %d line %d, x %d y %d\n", col, line, x, y);
    //printf("h %d v %d o %d vs %d d %d\n", hblank, vblank, overscan, vsync, draw);

    col++;
    if (col==COLS) { line++; col=0; tia_regs.WSYNC = 0xff; }
    if (line==ROWS) { line=0; }
  }

  tia_cycles++;
}

void tia_w8(u8 a, u8 v) {
  //printf("tia writing %0x <- %0x\n", a, v);

  switch (a) {
    case 0x00: tia_regs.VSYNC  = v; break;
    case 0x01: tia_regs.VBLANK = v; break;
    case 0x02: tia_regs.WSYNC  = v; break;
    case 0x03: tia_regs.RSYNC  = v; break;
    case 0x06: tia_regs.COLUP0 = v; break;
    case 0x07: tia_regs.COLUP1 = v; break;
    case 0x08: tia_regs.COLUPF = v; break;
    case 0x09: tia_regs.COLUBK = v; break;
    case 0x0a: tia_regs.CTRLPF = v; break;
    case 0x0d: tia_regs.PF0 = v;  break;
    case 0x0e: tia_regs.PF1 = v;  break;
    case 0x0f: tia_regs.PF2 = v;  break;
    case 0x10: /*printf("RESP0\n");*/ break;
    case 0x11: /*printf("RESP1\n");*/ break;
    case 0x12: /*printf("RESM0\n");*/ break;
    case 0x13: /*printf("RESM1\n");*/ break;
    case 0x14: /*printf("RESBL\n");*/ break;
    case 0x2a: /*printf("HMOVE\n");*/ break;
    case 0x2b: /*printf("HMCLR\n");*/ break;
    case 0x2c: /*printf("CXCLR\n");*/ break;

    default:
      //tia_regs[a & 0x3f] = v;
      //printf("tia writing %0x <- %0x\n", a, v);
      break;
  }
}

u8 tia_r8(u8 a) {
  u8 v = 0; // = tia_regs[0x30 | (a & 0x0f)];
  //printf("tia reading %0x -> %0x\n", a,v);
  //return v;
  return v;
}

