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
        if (this->IsEmpty()) {
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
    std::optional<V> Get(const K& key) {
        if (this->IsEmpty()) throw std::runtime_error("Get: Tree is empty!");

        auto ptr = this->root;
        while (ptr) {
            if (ptr->value.first == key) return ptr->value.second;
            ptr = key < ptr->value.first ? ptr->left : ptr->right;
        }

        return std::nullopt;
    }

    // Delete
    std::optional<V> Delete(const K& key) {
        if (this->IsEmpty()) throw std::runtime_error("Delete: Tree is empty!");

        // special case for root node
        auto ptr = this->root;
        if (ptr->value.first == key) {
            // we have found the node to delete
            auto return_value = ptr->value.second;
            
            if (!ptr->left) {
                // replace root with the right child
                this->root = ptr->right;
                if (this->root) this->root->parent = nullptr;
                delete ptr;
                this->size--;
                return return_value;
            } else if (!ptr->right) {
                // replace root with left child
                this->root = ptr->left;
                if (this->root) this->root->parent = nullptr;
                delete ptr;
                this->size--;
                return return_value;
            } else {
                // replace with the successor when both children are present
                // First find the successor
                auto successor = ptr->right;
                while (successor->left) {
                    successor = successor->left;
                }

                // found the successor
                ptr->value = successor->value;
                if (successor != ptr->right) {
                    successor->parent->left = successor->right;
                } else {
                    ptr->right = successor->right;
                }

                if (successor->right) successor->right->parent = successor->parent;

                delete successor;
                this->size--;
                return return_value;
            }

        }

        while (ptr) {
            if (key < ptr->value.first) {
                // check left
                if (ptr->left) {
                    if (ptr->left->value.first == key) {
                        // we found node to delete

                        // TODO
                    }
                } else {
                    return std::nullopt;
                }
            } else {
                // check right
                if (ptr->right) {
                    if (ptr->right->value.first ==  key) {
                        // we found the node to delete

                        // TODO
                    }

                } else {
                    return std::nullopt;
                }
            }
        }

        return std::nullopt;
    }


private:

    Node<std::pair<K, V>>* root;
    size_t size;

};