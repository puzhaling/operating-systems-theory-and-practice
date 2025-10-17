#include <stdio.h>
#include <unistd.h>

int main(void) {
  int x = 100;
  ++x;
  int rc = fork();
  
  if (rc == 0) {
    printf("child x is:%d\n", x);
  } else {
    printf("parent x is:%d\n", x);
  }
  return 0;
}
