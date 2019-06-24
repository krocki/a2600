#include "6502/typedefs.h"

extern void w8(u16 a, u8 v);
extern u8 r8(u16 a);
extern u16 r16_ok(u16 a);
extern u16 r16(u16 a);

u8 _mem[0x40000]; // 256kB
u8 *mem; // points to _mem
