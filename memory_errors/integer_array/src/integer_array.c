#include <stdlib.h>
#include <stdio.h>
#include "integer_array.h"

static size_t array_size = 100;

void alloc_integer_array_no_free(void) {
    int* integer_array = malloc(array_size * sizeof(int));
}

void alloc_integer_array(void) {
    int* integer_array = malloc(array_size * sizeof(int));
    integer_array[100] = 0;
}

void alloc_integer_array_with_free(void) {
    int* integer_array = malloc(array_size * sizeof(int));
    free(integer_array);
    printf("Value of index 0 is, %d\n", integer_array[0]);
}

void alloc_integer_array_free_midpoint(void) {
    int* integer_array = malloc(array_size * sizeof(int));
    int* midpoint = integer_array + 49;
    free(midpoint);
}

int main() {
    alloc_integer_array_no_free();
    alloc_integer_array();
    alloc_integer_array_with_free();

}