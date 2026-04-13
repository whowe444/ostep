#pragma once

#include "node.hh"
#include <shared_mutex>
#include <optional>

using namespace red_black_tree;

template<typename K, typename V>
class ConcurrentRedBlackTree {

public:

    // Constructor
    ConcurrentRedBlackTree() {
        root = nullptr;
        size = 0;
    }

    // Destructor
    ~ConcurrentRedBlackTree() {
        this->Clear(this->root);
    }

    // Copy Ctor
    ConcurrentRedBlackTree(const ConcurrentRedBlackTree& other) = delete;

    // Copy Assignment
    ConcurrentRedBlackTree& operator=(const ConcurrentRedBlackTree& other) = delete;

    // Move Ctor
    ConcurrentRedBlackTree(ConcurrentRedBlackTree&& other) = delete;

    // Move Assignment
    ConcurrentRedBlackTree& operator=(ConcurrentRedBlackTree&& other) = delete;

    // GetSize
    int GetSize() {
        return this->size;
    }

    // Contains
    bool Contains(const K& key)
    {
        return this->Get(key).has_value();
    }

    // IsEmpty
    bool IsEmpty() {
        return this->GetSize() == 0;
    }

    void Clear(red_black_tree::Node<std::pair<K, V>>* node) {
        std::unique_lock<std::shared_mutex> write_lock(this->treeMutex);
        this->ClearHelper(node);
    }

    // Insert
    bool Add(const K& key, const V& value) {
        std::unique_lock<std::shared_mutex> write_lock(this->treeMutex);
        // If root is NULL, we need to 
        // allocate it and return.
        if (this->IsEmpty()) {
            // The root node is always black.
            root = new red_black_tree::Node<std::pair<K, V>>(nullptr, nullptr, nullptr, Color::Black, {key, value});
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
                    ptr->left = new red_black_tree::Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Red, {key, value});
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
                    ptr->right = new red_black_tree::Node<std::pair<K, V>>(nullptr, nullptr, ptr, Color::Red, {key, value});
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
        std::shared_lock<std::shared_mutex> read_lock(this->treeMutex);
        if (this->IsEmpty()) return std::nullopt;

        auto ptr = this->root;
        while (ptr) {
            if (ptr->value.first == key) return ptr->value.second;
            ptr = key < ptr->value.first ? ptr->left : ptr->right;
        }

        return std::nullopt;
    }

    // Delete
    std::optional<V> Remove(const K& key) {
        std::unique_lock<std::shared_mutex> write_lock(this->treeMutex);
        if (this->IsEmpty()) return std::nullopt;

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
            } else if (!ptr->right) {
                // replace root with left child
                this->root = ptr->left;
                if (this->root) this->root->parent = nullptr;
                delete ptr;
                this->size--;
            } else {
                // replace with the successor when both children are present
                this->replaceWithSuccessorAndDeleteSuccessor(ptr);
                return return_value;
            }

            this->rebalanceDeletion(this->root, nullptr, Color::Black);
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
                            return return_value;
                        }

                        // Pass in the replacement node (left in this case)
                        this->rebalanceDeletion(ptr->left, ptr, return_color);
                        return return_value;

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
                            return return_value;
                        }

                        // Pass in the replacement node (left in this case)
                        this->rebalanceDeletion(ptr->right, ptr, return_color);
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

    friend class ConcurrentRedBlackTreeTest;

    void ClearHelper(red_black_tree::Node<std::pair<K, V>>* node) {
        if (!node) return;
        if (node->left) ClearHelper(node->left);
        if (node->right) ClearHelper(node->right);
        delete node;
        this->size--;
    }

    void Validate() {
        if (!this->root) return;
        assert(this->root->color == Color::Black);
        assert(this->root->parent == nullptr);
        ValidateNode(this->root);
    }

    int ValidateNode(red_black_tree::Node<std::pair<K, V>>* node) {
        if (!node) return 1;

        // Property 3: Red node must not have red children
        if (node->color == Color::Red) {
            assert(!node->left || node->left->color == Color::Black);
            assert(!node->right || node->right->color == Color::Black);
        }

        // Property 4: Both subtrees must have the same black height
        int left_black_height = this->ValidateNode(node->left);
        int right_black_height = this->ValidateNode(node->right);
        assert(left_black_height == right_black_height);

        // Property 5: Verify parent pointers are consistent
        if (node->left) assert(node->left->parent == node);
        if (node->right) assert(node->right->parent == node);

        return left_black_height + (node->color == Color::Black ? 1 : 0);
    }

    void replaceWithSuccessorAndDeleteSuccessor(red_black_tree::Node<std::pair<K, V>>* delete_node) {
        //first find the successor
        auto successor = delete_node->right;
        while (successor->left) successor = successor->left;

        // found the successor
        delete_node->value = successor->value;
        auto deleted_node_color = successor->color;

        auto parent_node = successor->parent;
        auto replacement_node = successor->right;
        if (successor != delete_node->right) {
            // the right subtree had a left component
            parent_node->left = replacement_node;
        } else {
            // the right subtree had no left children
            parent_node->right = replacement_node;
            
        }

        if (replacement_node) replacement_node->parent = parent_node;
        
        delete successor;
        this->size--;
        
        // Now we need to call the rebalance function
        this->rebalanceDeletion(replacement_node, parent_node, deleted_node_color);
    }

    /**
     * Rebalance the ConcurrentRedBlackTree after a node has been deleted.
     * 
     * @param replacement_node a pointer to the replacement node of the node that was deleted
     * @param deleted_node_color the color of the initial deleted node
     */
    void rebalanceDeletion(red_black_tree::Node<std::pair<K, V>>* replacement_node, red_black_tree::Node<std::pair<K, V>>* parent_node,
            Color deleted_node_color) {
        // There is no rebalancing to do
        if (!parent_node) {
            // If you are root, recolor yourself black and return
            if (this->root) this->root->color = Color::Black;
            return;
        }

        // If you deleted a red, there is no rebalancing
        if (deleted_node_color == Color::Red) return;

        // If you are a red node who replaced a black node simply color yourself 
        // black to restore the proper number of black nodes in the chain
        if (replacement_node && replacement_node->color == Color::Red) {
            replacement_node->color = Color::Black;
            return;
        }

        bool node_is_left = parent_node->left == replacement_node;
        auto sibling = node_is_left ? parent_node->right : parent_node->left;

        // If sibling is red, rotate parent towards double black and recolor, then re-evaluate
        if (sibling && sibling->color == Color::Red) {
            if (node_is_left) {
                this->leftRotation(parent_node);
            } else {
                this->rightRotation(parent_node);
            }
            // recolor
            parent_node->color = Color::Red;
            sibling->color = Color::Black;

            // re-evaluate
            this->rebalanceDeletion(replacement_node, parent_node, deleted_node_color);
        } else if (sibling && sibling->color == Color::Black) {
            // sibling is black, check if the sibling's children are black
            auto sibling_left_color = !sibling->left || sibling->left->color == Color::Black ? Color::Black : Color::Red;
            auto sibling_right_color = !sibling->right || sibling->right->color == Color::Black ? Color::Black : Color::Red;
            if (sibling_left_color == Color::Black && sibling_right_color == Color::Black) {
                // Case 2, recolor sibling red and propagate up to parent
                sibling->color = Color::Red;

                // propagate up to parent, in order to do this, parent becomes our new replacement node
                // and parent->parent becomes the new parent
                this->rebalanceDeletion(parent_node, parent_node->parent, deleted_node_color);
            } else {
                // one of the sibling's children is red

                auto near_child = node_is_left ? sibling->left : sibling->right;

                // if the near one is red, rotate the sibling away from the double black, recolor
                // and fall through to case 4
                if (near_child && near_child->color == Color::Red) {
                    if (node_is_left) {
                        this->rightRotation(sibling);
                    } else {
                        this->leftRotation(sibling);
                    }
                    // recolor
                    sibling->color = Color::Red;
                    near_child->color = Color::Black;
                    sibling = near_child;
                }

                auto far_child = node_is_left ? sibling->right : sibling->left;

                /// if the far one is red, rotate the parent towards the double black, recolor
                // and we are done
                if (far_child && far_child->color == Color::Red) {
                    auto parent_color = parent_node->color;
                    if (node_is_left) {
                        this->leftRotation(parent_node);
                    } else {
                        this->rightRotation(parent_node);
                    }
                    parent_node->color = Color::Black;
                    sibling->color = parent_color;
                    far_child->color = Color::Black;
                    return;
                }
            }
        } else {
            // the sibling was null
            this->rebalanceDeletion(parent_node, parent_node->parent, deleted_node_color);
        }
    }

    void rebalanceInsertion(red_black_tree::Node<std::pair<K, V>>* node) {
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
            bool parent_is_left = grandparent->left == parent;
            bool node_is_left = parent->left == node;
            bool is_triangle = parent_is_left != node_is_left;
            
            if (is_triangle) {
                // perform a rotation on the parent to produce a line
                if (parent_is_left) {
                    // perform a left rotation
                    this->leftRotation(parent);
                } else {
                    // perform a right rotation
                    this->rightRotation(parent);
                }
                parent = node;
            }

            // perform a rotation on the grandparent and recolor
            if (parent_is_left) {
                // perform right rotation on grandparent
                this->rightRotation(grandparent);
            } else {
                // perform left rotation on grandparent
                this->leftRotation(grandparent);
            }

            // Recolor the grandparent to red since it is now a child
            grandparent->color = Color::Red;

            // Recolor parent to black since it is now the root of the subtree
            parent->color = Color::Black;
            return;
        }
    }

    void leftRotation(red_black_tree::Node<std::pair<K, V>>* node) {
        // node must have a right child for left rotations
        assert(node->right != nullptr);
        auto right_child = node->right;
        auto parent = node->parent;
        auto new_right_child = right_child->left;

        // Reattach original node to its new parent
        right_child->left = node;
        node->parent = right_child;

        node->right = new_right_child;
        if (new_right_child) new_right_child->parent = node;

        if (!parent) {
            // we are the root node
            // right child becomes the new root node
            this->root = right_child;
            right_child->parent = nullptr;
        } else {
            auto right_side = parent->right == node;
            right_child->parent = parent;
            if (right_side) {
                parent->right = right_child;
            } else {
                parent->left = right_child;
            }

        }

    }

    void rightRotation(red_black_tree::Node<std::pair<K, V>>* node) {
        // node must have a left child for a right rotations
        assert(node->left != nullptr);
        auto left_child = node->left;
        auto parent = node->parent;
        auto new_left_child = left_child->right;

        // Reattach original node to its new parent
        left_child->right = node;
        node->parent = left_child;

        node->left = new_left_child;
        if (new_left_child) new_left_child->parent = node;

        if (!parent) {
            // we are the root node
            // left child becomes the new root node
            this->root = left_child;
            left_child->parent = nullptr;
        } else {
            auto right_side = parent->right == node;
            left_child->parent = parent;
            if (right_side) {
                parent->right = left_child;
            } else {
                parent->left = left_child;
            }
        }
    }

    red_black_tree::Node<std::pair<K, V>>* root;
    std::atomic<size_t> size;
    std::shared_mutex treeMutex;

};