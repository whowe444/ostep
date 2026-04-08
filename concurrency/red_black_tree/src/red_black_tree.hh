#pragma once

#include "node.hh"

template<typename T>
class RedBlackTree {

public:

    // Constructor
    RedBlackTree() {
        root = nullptr;
        size = 0;
    }

    // Destructor
    ~RedBlackTree() {
        delete root;
    }

    // GetSize
    int GetSize() {
        return this->size;
    }

    // IsEmpty
    bool IsEmpty() {
        return this->GetSize() == 0;
    }

    // Insert
    bool Insert(const T& value) {
        // If root is NULL, we need to 
        // allocate it and return.
        if (!root) {
            // The root node is always black.
            root = new Node(nullptr, nullptr, nullptr, Color::Black, value);
            this->size++;
            return true;
        }
    }

private:

    Node<T>* root;
    size_t size;

};