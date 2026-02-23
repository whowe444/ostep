#ifndef REALLOC_LIST_ITERATOR_H
#define REALLOC_LIST_ITERATOR_H

typedef struct {
    int *current;
    int* end;
} ReallocListIterator;

ReallocListIterator ReallocList_iterator(ReallocList *list);
int ReallocListIterator_hasNext(ReallocListIterator *it);
int ReallocListIterator_next(ReallocListIterator *it);

#endif // REALLOC_LIST_ITERATOR_H