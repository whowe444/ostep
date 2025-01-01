#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "q5.h"

/**
 * Fork function with a wait call.
 */
void fork_function(void) {
  // Grab parent pid.
  pid_t tParent = getpid();

  // Fork the current process.
  fork();

  if (getpid() == tParent)
  {
    int tStatus;
    pid_t tChildPid = wait(&tStatus);
    if (tChildPid == -1) {
      perror("wait() failed in parent");
    }
    printf("goodbye\n");
  }
  else
  {
    // Simulate work in the child process.
    sleep(1);
    printf("hello\n");

    int tStatus;
    pid_t tPid = wait(&tStatus);
    if (tPid == -1) {
      perror("wait() failed in the child");
    }
  }
}

/**
 * Output:
 * 
 * willhowe@MacBook-Pro q5 % make run
 * gcc -Wall -c q5.c -o q5.o
 * gcc q5.o -o q5.bin
 * ./q5.bin
 * hello
 * wait() failed in the child: No child processes
 * goodbye
 * 
 * The takeaway here is that the wait syscall in the child fails since the 
 * child has no child processes. But the wait call succeeds in the parent since
 * the parent has spawned off a child process.
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
