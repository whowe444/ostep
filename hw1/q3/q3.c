#include <stdio.h>
#include <unistd.h>
#include "q3.h"

/**
 * Fork function with an integer local variable.
 */
void fork_function(void) {
  // Create a pipe.
  int tPipeFDs[2];
  pipe(tPipeFDs);

  // Grab parent pid.
  pid_t tParent = getpid();

  // Fork the current process.
  fork();

  if (getpid() == tParent)
  {
    // Close the pipe writer.
    close(tPipeFDs[1]);
    
    // Wait for the child to write to the pipe.
    char tBuffer[4];
    read(tPipeFDs[0], tBuffer, 4);

    printf("goodbye\n");

    // Close the pipe reader.
    close(tPipeFDs[0]);
  }
  else
  {
    printf("hello\n");

    // Close the pipe reader.
    close(tPipeFDs[0]);

    write(tPipeFDs[1], "done", 4);

    // Close the writer after writing
    close(tPipeFDs[1]);
  }
}

/**
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
