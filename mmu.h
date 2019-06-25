#include "6502/typedefs.h"
#define CART_SIZE 0x1000

extern void w8(u16 a, u8 v);
extern u8 r8(u16 a);
extern u16 r16_ok(u16 a);
extern u16 r16(u16 a);

u8 _cart[CART_SIZE];
u8 _ram[0x80];
u8 _riot[0x100];

u8 *cart;
u8 *ram;
u8 *riot;
