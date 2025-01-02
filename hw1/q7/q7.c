#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "q7.h"

/**
 * Fork function.
 */
void fork_function(void) {
  // Grab parent pid.
  pid_t tParent = getpid();

  // Fork the current process.
  fork();

  if (getpid() == tParent)
  {
    printf("goodbye\n");
  }
  else
  {
    close(STDOUT_FILENO);
    printf("hello\n");
  }
}

/**
 * After closing the STDOUT_FILENO the child process is not longer 
 * able to print to stdout, but the parent process is still able
 * to do so.
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
