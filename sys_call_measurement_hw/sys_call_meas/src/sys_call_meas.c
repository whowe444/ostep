#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include "sys_call_meas.h"

static int fd;

/**
 * Call a system call repeatedly and measure the time it
 * takes on average.
 */
void sys_call_meas(void) {
    struct timeval start, end;

    // Get initial time
    gettimeofday(&start, NULL);

    int ITERATIONS = 100000;
    for (int i = 0; i < ITERATIONS; i++) {
        zero_byte_read();
    }

    // Get end time
    gettimeofday(&end, NULL);
    
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    long elapsed = seconds * 1000000L + useconds;

    printf("Sys call took %ld us per call!\n", elapsed / ITERATIONS);
}

/**
 * Perform a zero byte read (read sys call)
 */
int zero_byte_read(void) {
    char buf[1];
    ssize_t n = read(fd, buf, 0);

    printf("Bytes read: %zd\n", n);
    return 0;
}

/**
 *
 */
int main() {
    int fd = open("test.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
	return 1;
    }

    // Call the system call
    sys_call_meas();
    close(fd);
    return 0;
}
