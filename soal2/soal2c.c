#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int pid;
int fd1[2];
int fd2[2];

int main() {
  if (pipe(fd1) == -1) {
    perror("Pipe Failed");
    exit(1);
  }

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd1[1], 1);
    close(fd1[0]);
    close(fd1[1]);
    execlp("ps", "ps", "aux", NULL);
    _exit(1);
  }

  if (pipe(fd2) == -1) {
    perror("Pipe Failed");
    exit(1);
  }

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd1[0], 0);
    dup2(fd2[1], 1);
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    execlp("sort", "sort", "-nrk", "3,3", NULL);
    _exit(1);
  }

  close(fd1[0]);
  close(fd1[1]);

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd2[0], 0);
    close(fd2[0]);
    close(fd2[1]);
    execlp("head", "head", "-5", NULL);
    _exit(1);
  }
}