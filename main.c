#include <stdio.h>
#include "6502/6502.h"
#include "tia.h"
#include "lcd.h"
#include "a26.h"

#define USE_LCD 1
#if USE_LCD
#include <pthread.h>
#endif

int main(int argc, char **argv) {
#if USE_LCD

  pthread_t a26_thr;

  if(pthread_create(&a26_thr, NULL, a26, argv[1])) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  init_lcd();

  if(pthread_join(a26_thr, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return 2;
  }

#else

  (*a26)(argv[1]);

#endif

  return 0;
}
