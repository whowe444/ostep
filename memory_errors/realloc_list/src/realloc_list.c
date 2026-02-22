#include <stddef.h>
#include "realloc_list.h"

size_t ReallocList_size(ReallocList *list) {
    return list->size;
}

int* ReallocList_remove(ReallocList *list, size_t index) {
    if (list->size <= index) return NULL;
    return NULL;
}
