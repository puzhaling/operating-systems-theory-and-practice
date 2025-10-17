#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *data = (int *) malloc(sizeof(int) * 10);
  free(data + 5);
  return 0;
}
