#include "6502/typedefs.h"
#define COLS 228
#define ROWS 262
#define IM_W 160
#define IM_H 192

extern u32 pal[];
extern u8 pix[IM_W * IM_H * 3];
extern void tia_step(u32 ticks);
// read / write
extern void tia_w8(u8 a, u8 v);
extern u8 tia_r8(u8 a);

typedef struct {
  u8 VSYNC  ;
  u8 VBLANK ;
  u8 WSYNC  ;
  u8 RSYNC  ;
  u8 COLUP0;    // 0x06
  u8 COLUP1;    // 0x07
  u8 COLUPF;    // 0x08
  u8 COLUBK;    // 0x09
  u8 CTRLPF;    // 0x0a
  u8 PF0;
  u8 PF1;
  u8 PF2;
} tia_regs_t;

extern tia_regs_t tia_regs;
extern u64 tia_cycles;

//#define TEST_SCREEN 1
