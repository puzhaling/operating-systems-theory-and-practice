#include <stdio.h>
#include <stdlib.h>

//#define NULL_DEREFERENCING
//#define FORGET_TO_FREE 
#define ALL_S_GREAT

int main() {
  #ifdef NULL_DEREFERENCING
  int *iptr = NULL;
  printf("%d\n", *iptr);
  #endif

  #ifdef FORGET_TO_FREE
  int *iptr = (int *) malloc(sizeof(int));
  *iptr = 1;
  printf("%d\n", *iptr);
  #endif

  #ifdef ALL_S_GREAT
  int *iptr = (int *) malloc(sizeof(int));
  *iptr = 1;
  printf("%d\n", *iptr);
  free(iptr);
  #endif

  return 0;
}
