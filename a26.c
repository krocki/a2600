#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "6502/6502.h"
#include "a26.h"
#include "mmu.h"
#include "tia.h"
#include "io.h"

extern int lcd_on;
extern u8 limitspeed;
extern u8 paused;
extern u8 *screen;
u64 cpu_cycles = 0;

void *a26(void *args) {
  printf("a26: args %s\n", (char*)args);

  printf("a26: waiting for lcd\n");
  while (!lcd_on) usleep(100);
  screen = pix;
  printf("a26: synced with lcd\n");

  u32 steps = 1;

  if (args) readfile(args, cart, CART_SIZE);
  else printf("a26: no rom specified\n");

  u16 rst_ip = r16(0xfffc);
  printf("rst ip %04x\n", rst_ip);

  reset(rst_ip, 0xff, 0x24);

  while (lcd_on) {
    if (paused) continue;
    if (limitspeed) usleep(1);

    if (0xff == tia_regs.WSYNC) {
      cpu_step(steps);
      cpu_cycles+=steps;
    }

    tia_step(steps*3);

  }

  printf("a26: terminate, cycles %llu\n", cpu_cycles);
  return 0;
}
