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
        this->Clear(this->root);
    }

    // GetSize
    int GetSize() {
        return this->size;
    }

    // IsEmpty
    bool IsEmpty() {
        return this->GetSize() == 0;
    }

    void Clear(Node<std::pair<K, V>>* node) {
        if (!node) return;
        if (node->left) Clear(node->left);
        if (node->right) Clear(node->right);
        delete node;
        this->size--;
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
                    ptr->left = new Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Red, {key, value});
                    this->size++;

                    this->rebalanceInsertion(ptr->left);
                    return true;

                }
            } else {
                if (ptr->right) {
                    ptr = ptr->right;
                    continue;
                } else {
                    // add new node
                    ptr->right = new Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Red, {key, value});
                    this->size++;
                    
                    this->rebalanceInsertion(ptr->right);
                    return true;
                }
            }
        }

        return false;
    }

    // Get
    std::optional<V> Get(const K& key) {
        if (this->IsEmpty()) return std::nullopt;

        auto ptr = this->root;
        while (ptr) {
            if (ptr->value.first == key) return ptr->value.second;
            ptr = key < ptr->value.first ? ptr->left : ptr->right;
        }

        return std::nullopt;
    }

    // Delete
    std::optional<V> Delete(const K& key) {
        if (this->IsEmpty()) return std::nullopt;

        // special case for root node
        auto ptr = this->root;
        if (ptr->value.first == key) {
            // we have found the node to delete
            auto return_value = ptr->value.second;
            auto return_color = ptr->color;
            
            if (!ptr->left) {
                // replace root with the right child
                this->root = ptr->right;
                if (this->root) this->root->parent = nullptr;
                delete ptr;
                this->size--;
            } else if (!ptr->right) {
                // replace root with left child
                this->root = ptr->left;
                if (this->root) this->root->parent = nullptr;
                delete ptr;
                this->size--;
            } else {
                // replace with the successor when both children are present
                this->replaceWithSuccessorAndDeleteSuccessor(ptr);
            }

            // TODO: rebalancing
            return return_value;
        }

        while (ptr) {
            if (key < ptr->value.first) {
                // check left
                if (ptr->left) {
                    if (ptr->left->value.first == key) {
                        // we found node to delete

                        auto delete_node = ptr->left;
                        auto return_value = delete_node->value.second;
                        auto return_color = delete_node->color;

                        // now delete the node
                        if (!delete_node->left) {
                            // replace with right child
                            ptr->left  = delete_node->right;
                            if (ptr->left) ptr->left->parent = ptr;
                            delete delete_node;
                            this->size--;
                        } else if(!delete_node->right) {
                            // replace with left child
                            ptr->left = delete_node->left;
                            if (ptr->left) ptr->left->parent = ptr;
                            delete delete_node;
                            this->size--;
                        } else {
                            // logic for the successor
                            this->replaceWithSuccessorAndDeleteSuccessor(delete_node);
                        }

                        if (return_color == Color::Red) {
                            // no rebalancing needed
                            return return_value;
                        } else {
                            // TODO: rebalancing
                            return return_value;
                        }

                    } else {
                        ptr = ptr->left;
                    }
                } else {
                    return std::nullopt;
                }
            } else {
                // check right
                if (ptr->right) {
                    if (ptr->right->value.first ==  key) {
                        // we found the node to delete

                        auto delete_node = ptr->right;
                        auto return_value = delete_node->value.second;
                        auto return_color = delete_node->color;

                        // now delete the node
                        if (!delete_node->left) {
                            // replace with the right child
                            ptr->right = delete_node->right;
                            if (ptr->right) ptr->right->parent = ptr;
                            delete delete_node;
                            this->size--;
                        } else if(!delete_node->right) {
                            // replace with the left child
                            ptr->right = delete_node->left;
                            if (ptr->right) ptr->right->parent = ptr;
                            delete delete_node;
                            this->size--;
                        } else {
                            // logic for the successor
                            this->replaceWithSuccessorAndDeleteSuccessor(delete_node);
                        }

                       if (return_color == Color::Red) {
                            // no rebalancing needed
                            return return_value;
                        } else {
                            // TODO: rebalancing
                            return return_value;
                        }

                        return return_value;

                    } else {
                        ptr = ptr->right;
                    }
                } else {
                    return std::nullopt;
                }
            }
        }

        return std::nullopt;
    }

private:

    void replaceWithSuccessorAndDeleteSuccessor(Node<std::pair<K, V>>* delete_node) {
        //first find the successor
        auto successor = delete_node->right;
        while (successor->left) successor = successor->left;

        // found the successor
        delete_node->value = successor->value;
        if (successor != delete_node->right) {
            successor->parent->left = successor->right;
        } else {
            delete_node->right = successor->right;
        }

        if (successor->right) successor->right->parent = successor->parent;
        
        delete successor;
        this->size--;
    }

    void rebalanceInsertion(Node<std::pair<K, V>>* node) {
        // Check if I am the root node
        if (!node->parent) {
            node->color = Color::Black;
            return;
        }
    
        // Base Case (ends the rebalancing)
        if (node->parent->color == Color::Black) return;

        auto parent = node->parent;
        auto grandparent = parent->parent;
        auto grandparent_left = grandparent->left;
        auto grandparent_left_color = !grandparent_left || grandparent_left->color == Color::Black
            ? Color::Black : Color::Red;
        auto grandparent_right = grandparent->right;
        auto grandparent_right_color = !grandparent_right || grandparent_right->color == Color::Black
            ? Color::Black : Color::Red;
        if (grandparent_left_color == Color::Red && grandparent_right_color == Color::Red) {
            // Case 1: Parent and Uncle Red
            // Recolor parent and uncle to black, grandparent to red, then recurse on the grandparent.
            grandparent_left->color = Color::Black;
            grandparent_right->color = Color::Black;
            grandparent->color = Color::Red;
            this->rebalanceInsertion(grandparent);
        } else {
            // Case 2: Parent Red, Uncle Black
            bool parent_is_left = grandparent->left == node->parent;
            bool node_is_left = node->parent->left == node;
            bool is_triangle = parent_is_left != node_is_left;
            
            if (is_triangle) {
                // perform a rotation on the parent to produce a line
                if (parent_is_left) {
                    // perform a left rotation
                    this->leftRotation(node->parent);
                } else {
                    // perform a right rotation
                    this->rightRotation(node->parent);
                }
            }

            // perform a rotation on the grandparent and recolor
            if (parent_is_left) {
                // perform right rotation on grandparent
                this->rightRotation(grandparent);
            } else {
                // perform left rotation on grandparent
                this->leftRotation(grandparent);
            }

            // Recolor the grandparent to red, parent to black
            grandparent->color = Color::Red;
            parent->color = Color::Black;
            return;
        }
    }

    void leftRotation(Node<std::pair<K, V>>* node) {
        // TODO: unimplemented
    }

    void rightRotation(Node<std::pair<K, V>>* node) {
        // TODO: unimplemented
    }


    Node<std::pair<K, V>>* root;
    size_t size;

};