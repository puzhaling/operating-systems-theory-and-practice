#include <stdio.h>
#include <stdlib.h>

int main() {
  int *data = (int *) malloc(sizeof(int) * 10);
  free(data);
  printf("%d\n", data[0]);
  return 0;
}
