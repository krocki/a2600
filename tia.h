#include "6502/typedefs.h"
#define COLS 228
#define ROWS 262
#define IM_W 160
#define IM_H 192

extern u32 pal[];
extern u8 pix[IM_W * IM_H * 3];
extern void tia_step(u32 ticks);
