#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "realloc_list.h"

size_t ReallocList_size(ReallocList *list) {
    return list->size;
}

int* ReallocList_remove(ReallocList *list, size_t index) {
    if (list->size <= index) return NULL;
    list->size--;
    return &list->data[index];
}

int ReallocList_add(ReallocList *list, int value) {
    // If the list is empty, allocate memory.
    int* tmp = (int*) realloc(list->data, sizeof(int));
    if (tmp == NULL) {
        fprintf(stderr, "realloc failed\n");
        free(list->data);
        return -1;
    }
    list->data = tmp;

    list->data[list->size] = value;
    list->size++;
    return 0;
}

int* ReallocList_get(ReallocList *list, size_t index) {
    if (list->size <= index) return NULL;
    return &list->data[index];
}


