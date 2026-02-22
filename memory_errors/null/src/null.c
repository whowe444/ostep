#include <stddef.h>
#include <stdio.h>
#include "null.h"

void dereference_null(void) {
    int* null_pointer = NULL;
    printf("Integer value is %d!\n", *null_pointer);
}

int main() {

    dereference_null();

}