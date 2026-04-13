#pragma once

#include "node.hh"
#include <vector>
#include <atomic>

template<typename T>
class ConcurrentLinkedList {

public:

    // Constructor 
    ConcurrentLinkedList() {
        sentinel = new Node<T>();
        size = 0;
    }

    // Destructor
    ~ConcurrentLinkedList() {
        this->Clear();
        delete(this->sentinel);
    }

    // Copy Ctor
    ConcurrentLinkedList(const ConcurrentLinkedList& other) = delete;

    // Copy Assignment
    ConcurrentLinkedList& operator=(const ConcurrentLinkedList& other) = delete;

    // Move Ctor
    ConcurrentLinkedList(ConcurrentLinkedList&& other) = delete;

    // Move Assignemnt
    ConcurrentLinkedList& operator=(ConcurrentLinkedList&& other) = delete;

    // GetSize
    size_t GetSize() {
        return this->size.load();
    }

    // IsEmpty
    bool IsEmpty() {
        return this->GetSize() == 0;
    }

    // Add Function
    // Returns true upon success
    bool Add(int value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;

        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            prev_lock = std::move(curr_lock);
            prev = ptr;
            ptr = ptr->next;
        }

        prev->next = new Node<T>(nullptr, value);
        this->size++;
        return true;
    }

    // Value is Contained in the List.
    bool Contains(int value) {
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
            if (ptr->value == value) {
                return true;
            }

            // Release previous
            prev_lock = std::move(curr_lock);

            prev = ptr;
            ptr = ptr->next;
        }
        return false;
    }

    // Remove Value at index
    // Returns value removed
    bool Remove(int value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            if (ptr->value == value) {
                // Delete this node
                prev->next = ptr->next;
                this->size--;
                curr_lock.unlock();
                delete(ptr);
                return true;
            } else {
                prev_lock = std::move(curr_lock);

                ptr = ptr->next;
                prev = prev->next;
            }
        }
        return false;
    }

    // Clear out the list so that it is
    // empty.
    void Clear() {
        if (!sentinel) return;
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* ptr = this->sentinel->next;
        while (ptr) {
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            auto temp = ptr->next;
            curr_lock.unlock();
            delete(ptr);
            ptr = temp;
        }

        this->sentinel->next = nullptr;
        this->size = 0;
    }

private:

    Node<T>* sentinel;
    std::atomic<size_t> size;
};
