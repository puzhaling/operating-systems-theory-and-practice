#include <stdio.h>
#include <unistd.h>

int main(void) {
  close(STDOUT_FILENO);
  printf("hello\n");
}
