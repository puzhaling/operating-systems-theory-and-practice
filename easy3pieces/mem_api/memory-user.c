#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
  int mbs, bytes_to_alloc;
  size_t i;
  char *arr;

  if (argc != 2) {
    fprintf(stderr, "usage: <app-name> mbs to allocate\n");
    return 1;
  }

  mbs = atoi(argv[1]);
  bytes_to_alloc = mbs * 1024 * 1024;
  arr = (char *) malloc(bytes_to_alloc);
  if (arr == NULL) {
    fprintf(stderr, "not enough memory\n");
    return 1;
  }
  
  printf("allocation is succed\n");

  for (i = 0; i < bytes_to_alloc; ++i) {
    if (i + 1 == bytes_to_alloc)
      i = 0;
    arr[i] = 0;
  }

  return 0;
}
