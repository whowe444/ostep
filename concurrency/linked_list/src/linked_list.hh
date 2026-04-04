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
        Node* ptr = sentinel;
        while (ptr) {
            auto temp = ptr->next;
            delete(ptr);
            ptr = temp;
        }
    }

    // Copy Ctor
    LinkedList(const LinkedList& other);

    // Copy Assignment
    LinkedList& operator=(const LinkedList& other);

    // Move Ctor
    LinkedList(LinkedList&& other) noexcept;

    // Move Assignemnt
    LinkedList& operator=(LinkedList&&) noexcept;

    // GetSize
    size_t GetSize();

    // IsEmpty
    bool IsEmpty();

    // Add Function
    // Returns true upon success
    bool Add(int value);

    // Get Value at index
    int Get(int index);

    // Remove Value at index
    // Returns value removed
    int Remove(int index);

    // Clear out the list so that it is
    // empty.
    void Clear();

private:

    Node* sentinel;
    Node* tail;
    size_t size;

};
