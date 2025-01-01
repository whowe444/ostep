#include <stdio.h>
#include <unistd.h>
#include "q1.h"

// Function definition
void fork_function(void) {
  int x = 10;
  printf("Value of x is: %d in process: %d\n", x, getpid());
  fork();
  printf("Value of x is: %d in process: %d\n", x, getpid());
  x = 20;
  printf("Value of x is: %d in process: %d\n", x, getpid());
}

int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
