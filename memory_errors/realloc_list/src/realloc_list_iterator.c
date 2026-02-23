#include "realloc_list.h"
#include "realloc_list_iterator.h"

ReallocListIterator ReallocList_iterator(ReallocList *list) {
    ReallocListIterator it;
    it.current = list->data;
    it.end = list->data + list->size;
    return it;
}

int ReallocListIterator_hasNext(ReallocListIterator *it) {
    return ((it->current) != it->end);
}

int ReallocListIterator_next(ReallocListIterator *it) {
    int return_value = *(it->current);
    it->current+=1;
    return return_value;
}

