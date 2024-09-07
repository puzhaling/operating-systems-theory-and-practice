#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int* data = (int *) malloc(sizeof(int) * 100);
  // C guarantees that last past index is correct
  // in pointer arifmetic, but invalid in context of
  // writing and reading
  data[100] = 0;
  return 0;
}
