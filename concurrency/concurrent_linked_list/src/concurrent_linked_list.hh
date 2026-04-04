#pragma once

#include "node.hh"
#include <vector>
#include <atomic>

class ConcurrentLinkedList {

public:

    // Constructor 
    ConcurrentLinkedList() {
        sentinel = new Node();
        size = 0;
    }

    // Destructor
    ~ConcurrentLinkedList() {
        this->Clear();
        delete(this->sentinel);
    }

    // Copy Ctor
    ConcurrentLinkedList(const ConcurrentLinkedList& other);

    // Copy Assignment
    ConcurrentLinkedList& operator=(const ConcurrentLinkedList& other);

    // Move Ctor
    ConcurrentLinkedList(ConcurrentLinkedList&& other) noexcept;

    // Move Assignemnt
    ConcurrentLinkedList& operator=(ConcurrentLinkedList&&) noexcept;

    // GetSize
    size_t GetSize();

    // IsEmpty
    bool IsEmpty();

    // Add Function
    // Returns true upon success
    bool Add(int value);

    // Value is Contained in the List.
    bool Contains(int value);

    // Remove Value at index
    // Returns value removed
    bool Remove(int value);

    // Clear out the list so that it is
    // empty.
    void Clear();

private:

    Node* sentinel;
    std::atomic<size_t> size;
};
