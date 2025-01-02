#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "q8.h"

/**
 * Fork function with a pipe between the parent and child 
 * processes.
 */
void fork_function(void) {
  int tPipeFDs[2];
  pid_t tPid1, tPid2;
  char tMsg[] = "Hello from the first child!";
  char tReadMsg[100];

  // Create the pipe.
  pipe(tPipeFDs);

  tPid1 = fork();
  if (tPid1 == 0)
  {
    // Close the read end of the pipe.
    close(tPipeFDs[0]);

    // Redirect stdout to the pipe write end.
    if (dup2(tPipeFDs[1], STDOUT_FILENO) == -1) {
      perror("dup2 failed");
      exit(-1);
    }

    // Close original pipe end since it is duplicated.
    close(tPipeFDs[1]);

    printf("%s\n", tMsg);
    exit(0);

  }

  tPid2 = fork();
  if (tPid2 == 0)
  {
    // Close the read end of the pipe
    close(tPipeFDs[1]);

    // Redirect standard-in to the pipe reader.
    if (dup2(tPipeFDs[0], STDIN_FILENO) == -1) {
      perror("dup2 failed");
      exit(-1);
    }

    // Close the original pipe read end since it's now duplicated.
    close(tPipeFDs[0]);

    // Read from stdin (which is now the pipe) and print.
    if (fgets(tReadMsg, sizeof(tReadMsg), stdin) != NULL) {
      printf("Child 2 received: %s", tReadMsg);
    }

    exit(0);
  }

  // Close the pipe.
  close(tPipeFDs[0]);
  close(tPipeFDs[1]);

  // Wait for the child processes to finish.
  waitpid(tPid1, NULL, 0);
  waitpid(tPid2, NULL, 0);
  printf("Parent process exits.\n");
}

/**
 * The crucial takeaway of this problem is that the pipe can 
 * be duplicated using dup2 that will cause the child processes
 * to utilize stdin/stdout instead of the file descriptors 
 * provided by the pipe.
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
