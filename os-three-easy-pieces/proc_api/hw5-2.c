#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
  int out_desc = open("output.txt", O_WRONLY, S_IRWXU);
  int rc = fork();
  if (rc < 0) {
    fprintf(stdout, "fork error\n");
    exit(1);
  } else if (rc == 0) {
    dprintf(out_desc, "child print\n");
  } else {
    wait(NULL);
    dprintf(out_desc, "parent print\n");
  }
  close(out_desc);
  return 0;
}
