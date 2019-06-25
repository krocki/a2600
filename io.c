#include "io.h"

int readfile(const char* fname, u8* mem, u32 memsize) {

  FILE * file = fopen(fname, "r+");
  if (file == NULL) return -1;
  fseek(file, 0, SEEK_END);
  long int size = ftell(file);

  printf("%s, size=%ld\n", fname, size);
  if (size > memsize) {
    printf("size %ld > memsize %u\n", size, memsize);
    fclose(file);
    return -1;
  }

  fseek(file, 0, SEEK_SET);
  int bytes_read = fread(mem, sizeof(u8), size, file);
  fclose(file);

  return bytes_read;
}
