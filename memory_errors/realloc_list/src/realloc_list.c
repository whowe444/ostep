#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "realloc_list.h"

static void ReallocList_shiftdown(ReallocList *list, size_t index) {
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[index] = list->data[index+1];
    }
}

size_t ReallocList_size(ReallocList *list) {
    return list->size;
}

int ReallocList_remove(ReallocList *list, size_t index, int* return_value) {
    if (list->size <= index) return -1;
    *return_value = list->data[index];
    ReallocList_shiftdown(list, index);
    list->size--;
    return 0;
}

int ReallocList_add(ReallocList *list, int value) {
    // If the list is empty, double the capacity.
    if (list->size == list->capacity) {
        int* tmp = (int*) realloc(list->data, list->capacity * 2 * sizeof(int));
        if (tmp == NULL) {
            fprintf(stderr, "realloc failed\n");
            free(list->data);
            return -1;
        }
        list->data = tmp;
    }

    list->data[list->size] = value;
    list->capacity++;
    list->size++;
    return 0;
}

int* ReallocList_get(ReallocList *list, size_t index) {
    if (list->size <= index) return NULL;
    return &list->data[index];
}

ReallocList* ReallocList_init(void) {
    ReallocList *list = malloc(sizeof(ReallocList));
    if (list == NULL) return NULL;

    // Initialize
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;

    int* tmp = (int*) realloc(list->data, sizeof(int));
    if (tmp == NULL) {
        fprintf(stderr, "realloc failed\n");
        free(list->data);
        return NULL;
    }
    list->data = tmp;
    list->capacity++;

    return list;
}

void ReallocList_free(ReallocList *list) {
    free(list->data);
    free (list);
}
