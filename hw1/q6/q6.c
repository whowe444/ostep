#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "q6.h"

/**
 * Fork function with a wait call.
 */
void fork_function(void) {
  // Grab parent pid.
  pid_t tParent = getpid();

  // Fork the current process.
  pid_t tChild = fork();

  if (getpid() == tParent)
  {
    int tStatus;
    pid_t tChildPid = waitpid(tChild, &tStatus, 0);
    if (tChildPid == -1) {
      perror("waitpid() failed in parent");
    }
    printf("goodbye\n");
  }
  else
  {
    // Simulate work in the child process.
    sleep(1);
    printf("hello\n");

    int tStatus;
    pid_t tPid = waitpid(tChild, &tStatus, 0);
    if (tPid == -1) {
      perror("waitpid() failed in the child");
    }
  }
}

/**
 * The waitpid syscall has more flexibility than the wait() syscall
 * since it allows you to control which pids you are waiting on which
 * the normal wait() syscall does not allow.
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
