#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "q2.h"

/**
 * Fork function with an integer local variable.
 */
void fork_function(void) {
  int tFD = open("file.txt", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  // Data to write to the file
  char tText[50] = "";
  sprintf(tText, "Hello, this is a test message from, %d!\n", getpid());

  // Write data to the file
  ssize_t tBytes = write(tFD, tText, sizeof(tText) - 1);

  // Fork the current process.
  fork();

  // Data to write to the file
  sprintf(tText, "Hello, this is a test message from, %d!\n", getpid());

  // Write data to the file
  tBytes = write(tFD, tText, sizeof(tText) - 1);

  // Close the file descriptor.
  close(tFD);
}

/**
 * Crucial takeway from q2: the forked process indeed has access to the 
 * file and is able to write to it along with the parent process.
 */
int main() {
    // Call the function to fork your process
    fork_function();
    return 0;
}
