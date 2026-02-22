#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#include "memory-user.h"

static double memory_in_mebibytes;

int use_memory(void) {
    size_t bytes = (size_t)(memory_in_mebibytes * 1024.0 * 1024.0);
    size_t count = bytes / sizeof(int);
    printf("Allocating %.4f MiB (%zu bytes, %zu ints)...\n",
        memory_in_mebibytes, bytes, count);

    int *array = malloc(count * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Error: Failed to allocate %zu bytes\n", bytes);
        return EXIT_FAILURE;
    }

    int iteration = 0;
    while (true) {
        fprintf(stdout, "Entering the %dth iteration!\n", iteration);
        for (int index = 0; index < count; index++) {
            array[index] = index + iteration;
        }
        iteration++;
        sleep(1);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <size_in_MiB>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    errno = 0;

    memory_in_mebibytes = strtod(argv[1], &endptr);

    if (errno != 0) {
        perror("strtod");
        return EXIT_FAILURE;
    }

    if (endptr == argv[1]) {
        fprintf(stderr, "Error: No valid number found in '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (*endptr != '\0') {
        fprintf(stderr, "Error: Unexpected characters after number: '%s'\n", endptr);
        return EXIT_FAILURE;
    }

    if (memory_in_mebibytes < 0) {
        fprintf(stderr, "Error: Size must be non-negative!\n");
        return EXIT_FAILURE;
    }

    use_memory();
    return 0;
}