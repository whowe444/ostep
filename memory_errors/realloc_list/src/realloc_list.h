#ifndef REALLOC_LIST_H
#define REALLOC_LIST_H

#include <stddef.h>

typedef struct {
    size_t size;
    size_t capacity;
    int* data;
} ReallocList;

size_t ReallocList_size(ReallocList *list);
int ReallocList_remove(ReallocList *list, size_t index, int* return_value);
int ReallocList_add(ReallocList *list, int value);
int* ReallocList_get(ReallocList *list, size_t index);
ReallocList* ReallocList_init(void);
void ReallocList_free(ReallocList *list);

#endif // REALLOC_LIST_H