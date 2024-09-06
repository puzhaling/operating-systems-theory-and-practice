#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("code address: %p\n", (void *) main);
  printf("heap address: %p\n", (void *) malloc(1));
  int x = 3;
  printf("stack address: %p\n", (void *) &x);
  return x;
}
