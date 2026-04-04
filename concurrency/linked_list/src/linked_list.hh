#pragma once

#include "node.hh"
#include<vector>

class LinkedList {

public:

    // Constructor 
    LinkedList() {
        sentinel = new Node();
        tail = sentinel;
        size = 0;
    }

    // Destructor
    ~LinkedList() {
        delete(sentinel);
    }

    // GetSize
    size_t GetSize();

    // IsEmpty
    bool IsEmpty();

    // Add Function
    // Returns true upon success
    bool Add(int value);

    // Get Value at index
    int Get(int index);

private:

    Node* sentinel;
    Node* tail;
    size_t size;

};
