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
    ConcurrentLinkedList(const ConcurrentLinkedList& other) : ConcurrentLinkedList() {
        // Snapshot other's values under its lock
        std::vector<int> values;
        {
            std::unique_lock<std::shared_mutex> other_lock(other.sentinel->mtx);
            Node<T>* ptr = other.sentinel->next;
            while (ptr) {
                std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
                values.push_back(ptr->value);
                other_lock = std::move(curr_lock);
                ptr = ptr->next;
            }
        }
        // Now add without holding any of other's locks
        for (int v : values) this->Add(v);
    }

    // Copy Assignment
    ConcurrentLinkedList& operator=(const ConcurrentLinkedList& other) {
        // Self assignment guard
        if (this == &other) return *this;

        // Clear out all nodes and set size to 0.
        this->Clear();

        // Snapshot other's values under its lock
        std::vector<int> values;
        {
            std::unique_lock<std::shared_mutex> other_lock(other.sentinel->mtx);
            Node<T>* ptr = other.sentinel->next;
            while (ptr) {
                std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
                values.push_back(ptr->value);
                other_lock = std::move(curr_lock);
                ptr = ptr->next;
            }
        }
        // Now add without holding any of other's locks
        for (int v : values) this->Add(v);

        return *this;
    }

    // Move Ctor
    ConcurrentLinkedList(ConcurrentLinkedList&& other) noexcept
        :
            sentinel(other.sentinel),
            size(other.size.load())
    {
        other.sentinel = nullptr;
        other.size = 0;
    }

    // Move Assignemnt
    ConcurrentLinkedList& operator=(ConcurrentLinkedList&& other) noexcept {
        // Self assignment guard
        if (this == &other) return *this;

        // Clear out this list
        this->Clear();

        // Delete old sentinel
        delete(this->sentinel);

        this->sentinel = other.sentinel;
        this->size = other.size.load();

        // Null out other's sentinel
        other.sentinel = nullptr;
        other.size = 0;

        return *this;
    }

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
