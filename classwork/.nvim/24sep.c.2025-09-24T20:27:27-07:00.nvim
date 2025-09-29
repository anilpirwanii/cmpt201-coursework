#define _POSIX_C_SOURCE 200809

#include <signal.h>
#include <stdio.h>

void handler(int signum) {
  write(STDOUT_FILENO, message, strlen(message));
