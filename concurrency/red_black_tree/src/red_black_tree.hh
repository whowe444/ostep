#pragma once

#include "node.hh"

template<typename K, typename V>
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
    bool Insert(const K& key, const V& value) {
        // If root is NULL, we need to 
        // allocate it and return.
        if (!root) {
            // The root node is always black.
            root = new Node<std::pair<K, V>>(nullptr, nullptr, nullptr, Color::Black, {key, value});
            this->size++;
            return true;
        }

        // Insert into the tree
        auto ptr = this->root;
        while (ptr) {
            // update the value
            if (ptr->value.first == key) {
                ptr->value = {key, value};
                return true;
            }

            // Now check the value
            if (key < ptr->value.first) {
                if (ptr->left) {
                    ptr = ptr->left;
                    continue;
                } else {
                    // add new node
                    ptr->left = new Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Black, {key, value});
                    this->size++;
                    return true;
                }
            } else {
                if (ptr->right) {
                    ptr = ptr->right;
                    continue;
                } else {
                    // add new node
                    ptr->right = new Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Black, {key, value});
                    this->size++;
                    return true;
                }
            }
        }

        return false;
    }

    // Get
    const V& Get(const K& key) {
        if (this->IsEmpty()) throw std::runtime_error("Get: Tree is empty!");

        auto ptr = this->root;
        while (ptr) {
            if (ptr->value.first == key) return ptr->value.second;
            ptr = key < ptr->value.first ? ptr->left : ptr->right;
        }

        std::logic_error("Get: unreachable!");
    }

private:

    Node<std::pair<K, V>>* root;
    size_t size;

};