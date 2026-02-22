#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include "context_switch.h"

static int tPipeOneFDs[2];
static int tPipeTwoFDs[2];
static pid_t tPid1, tPid2;

static int ITERATIONS = 10000;
static int counter = 0;

/**
 * Force a process to be pin to one logical core of the CPU.
 */
void set_affinity(int cpu) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu, &mask);

  if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
    perror("sched_setaffinity");
    _exit(1);
  }
}

/**
 * Function with two pipes between process1 and process2 sending messages
 * back and forth in an circular manner.
 */
void circular_pipe_function(void) {
  if (tPid1 == 0)
  {
    // Set affinity
    set_affinity(0);

    // Close unused ends of pipe.
    close(tPipeOneFDs[0]);
    close(tPipeTwoFDs[1]);
    // Process one will write to pipe 1 and read from pipe2.
    while (counter < ITERATIONS) {
      // Write to pipe 1 write end
      write(tPipeOneFDs[1], "Hello, Process 2!", 18);

      // Read from pipe 2 read end
      char tBuffer[18];
      read(tPipeTwoFDs[0], tBuffer, 18);

      printf("%s\n", tBuffer);
      // Increment the counter
      counter = counter + 1;
    }

    // Close other ends of pipe
    close(tPipeOneFDs[1]);
    close(tPipeTwoFDs[0]);
    _exit(0);
  }

  if (tPid2 == 0)
  {
    // Set affinity
    set_affinity(0);

    // Closed unused ends of pipe.
    close(tPipeOneFDs[1]);
    close(tPipeTwoFDs[0]);

    // Process two will read from pip1 and write to pipe2.
    while (counter < ITERATIONS) {
      // Read from pipe 1 read end
      char tBuffer[18];
      read(tPipeOneFDs[0], tBuffer, 18);

      printf("%s\n", tBuffer);

      // Write to the pipe2 write end
      write(tPipeTwoFDs[1], "Hello, Process 1!", 18);
      // Increment the counter
      counter = counter + 1;
    }

    // Close unused ends of pipe.
    close(tPipeOneFDs[0]);
    close(tPipeTwoFDs[1]);
    _exit(0);
  }

  // Close the pipes.
  close(tPipeOneFDs[0]);
  close(tPipeOneFDs[1]);
  close(tPipeTwoFDs[0]);
  close(tPipeTwoFDs[1]);

  // Wait for the child processes to finish.
  waitpid(tPid1, NULL, 0);
  waitpid(tPid2, NULL, 0);

}

/**
 * The crucial takeaway of this problem is that the pipe can 
 * be duplicated using dup2 that will cause the child processes
 * to utilize stdin/stdout instead of the file descriptors 
 * provided by the pipe.
 */
int main() {
  // Create the pipes.
  pipe(tPipeOneFDs);
  pipe(tPipeTwoFDs);

  // Set up time variables
  struct timeval start, end;

  // Get initial time
  gettimeofday(&start, NULL);

  // Need to guard the second fork here, otherwise we will
  // fork the first fork (which we don't want to do).
  tPid1 = fork();
  if (tPid1 != 0) {
    tPid2 = fork();
  }

  // Call the function to fork your process
  circular_pipe_function();

  // Get end time
  gettimeofday(&end, NULL);

  long seconds = end.tv_sec - start.tv_sec;
  long useconds = end.tv_usec - start.tv_usec;
  long elapsed = seconds * 1000000L + useconds;

  printf("Context switching took %ld us per iteration!\n", elapsed / ITERATIONS);
  printf("Parent process exits.\n");

  return 0;
}
