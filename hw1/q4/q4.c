#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "q4.h"

/**
 * Fork function with an execl() call.
 */
void fork_function_with_execl(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  execl(tProgram, "ls", "-l");
}

/**
 * Fork function with an execlp() call.
 */
void fork_function_with_execlp(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  execlp(tProgram, "ls", "-l");
}

/**
 * Fork function with an execle() call.
 */
void fork_function_with_execle(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  execle(tProgram, "ls", "-l");
}

/**
 * Fork function with an execv() call.
 */
void fork_function_with_execv(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  char* tArgs[2] = {"ls", "-l"}; 
  execv(tProgram, tArgs);
}

/**
 * Fork function with an execvp() call.
 */
void fork_function_with_execvp(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  char* tArgs[2] = {"ls", "-l"}; 
  execvp(tProgram, tArgs);
}

/**
 * Fork function with an execvpe() call.
 */
void fork_function_with_execvpe(void) {
  fork();
  char tProgram[7] = "/bin/ls";
  char* tArgs[] = {"ls", "-l", (char *)NULL};
  char* tEnvp[] = {"PATH=/bin", (char *)NULL}; 
  
  // execvpe not portable to macOS
  // execvpe(tProgram, tArgs, tEnvp);
}

/**
 * Call each exec function.
 */
int main() {
    // Call the function to fork your process
    fork_function_with_execl();
    fork_function_with_execlp();
    fork_function_with_execle();
    fork_function_with_execv();
    fork_function_with_execvp();
    fork_function_with_execvpe();
    return 0;
}
