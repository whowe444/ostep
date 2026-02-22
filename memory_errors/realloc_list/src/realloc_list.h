#ifndef REALLOC_LIST_H
#define REALLOC_LIST_H

typedef struct {
    size_t size;
    int* data;
} ReallocList;

size_t ReallocList_size(ReallocList *list);
int* ReallocList_remove(ReallocList *list, size_t index);
int ReallocList_add(ReallocList *list, int value);
int* ReallocList_get(ReallocList *list, size_t index);

#endif // REALLOC_LIST_H