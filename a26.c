#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "6502/6502.h"
#include "a26.h"
#include "mmu.h"
#include "tia.h"
#include "io.h"

extern int lcd_on;
extern u8 *screen;
u64 cpu_cycles = 0;

void *a26(void *args) {
  printf("a26: args %s\n", (char*)args);

  printf("a26: waiting for lcd\n");
  while (!lcd_on) usleep(100);
  screen = pix;
  printf("a26: synced with lcd\n");

  u32 steps = 1;

  if (args) readfile(args, mem);
  else printf("a26: no rom specified\n");

  while (lcd_on) {
    cpu_step(steps);
    tia_step(steps*3);
    cpu_cycles+=steps;
  }

  printf("a26: terminate, cycles %llu\n", cpu_cycles);
  return 0;
}
