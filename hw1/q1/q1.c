#include <stdio.h>
#include <unistd.h>
#include "q1.h"

/**
 * Fork function with an integer local variable.
 */
void fork_function(void) {
  // Start value of x at 10.
  int x = 10;
  printf("Value of x is: %d in process: %d\n", x, getpid());

  // Fork the current process.
  fork();

  // Value of x in parent process should be 10. Value of x in 
  // child process is also 10.
  printf("Value of x is: %d in process: %d\n", x, getpid());

  // Now set the value of 'x' to 20.
  x = 20;

  // Observe that the value of x is changed to 20 in both the 
  // child and parent processes. 
  printf("Value of x is: %d in process: %d\n", x, getpid());
}

/**
 * Crucial takeaway: upon fork, the child process has its own
 * address space so while it may appear like the processes are
 * sharing the variable x, they each have their own "copy".
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
