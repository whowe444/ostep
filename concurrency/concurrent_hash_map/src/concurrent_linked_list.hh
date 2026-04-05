#pragma once

#include "node.hh"
#include <vector>
#include <atomic>

template<typename K, typename V>
class ConcurrentLinkedList {

public:

    // Constructor 
    ConcurrentLinkedList() {
        sentinel = new Node<std::pair<K, V>>();
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
        std::vector<std::pair<K, V>> values;
        {
            std::unique_lock<std::mutex> other_lock(other.sentinel->mtx);
            Node<std::pair<K, V>>* ptr = other.sentinel->next;
            while (ptr) {
                std::unique_lock<std::mutex> curr_lock(ptr->mtx);
                values.push_back(ptr->value);
                other_lock = std::move(curr_lock);
                ptr = ptr->next;
            }
        }
        // Now add without holding any of other's locks
        for (auto value : values) this->Add(value.first, value.second);
    }

    // Copy Assignment
    ConcurrentLinkedList& operator=(const ConcurrentLinkedList& other) {
        // Self assignment guard
        if (this == &other) return *this;

        // Clear out all nodes and set size to 0.
        this->Clear();

        // Snapshot other's values under its lock
        std::vector<std::pair<K, V>> values;
        {
            std::unique_lock<std::mutex> other_lock(other.sentinel->mtx);
            Node<std::pair<K, V>>* ptr = other.sentinel->next;
            while (ptr) {
                std::unique_lock<std::mutex> curr_lock(ptr->mtx);
                values.push_back(ptr->value);
                other_lock = std::move(curr_lock);
                ptr = ptr->next;
            }
        }
        // Now add without holding any of other's locks
        for (auto value : values) this->Add(value.first, value.second);

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
    bool Add(const K& key, const V& value) {
        std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
        Node<std::pair<K, V>>* prev = this->sentinel;
        Node<std::pair<K, V>>* ptr = prev->next;

        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
            prev_lock = std::move(curr_lock);

            // Updating an existing key
            if (ptr->value.first == key) {
                ptr->value = {key, value};
                return true;
            }

            prev = ptr;
            ptr = ptr->next;
        }

        prev->next = new Node<std::pair<K, V>>(nullptr, {key, value});
        this->size++;
        return true;
    }

    // Value is Contained in the List.
    bool Contains(const K& key) {
        std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
        Node<std::pair<K, V>>* prev = this->sentinel;
        Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
        
            if (ptr->value.first == key) {
                return true;
            }

            // Release previous
            prev_lock = std::move(curr_lock);

            prev = ptr;
            ptr = ptr->next;
        }
        return false;
    }

    // Value stored at the Key.
    std::optional<V> Get(const K& key) {
        std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
        Node<std::pair<K, V>>* prev = this->sentinel;
        Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
        
            if (ptr->value.first == key) {
                return ptr->value.second;
            }

            // Release previous
            prev_lock = std::move(curr_lock);

            prev = ptr;
            ptr = ptr->next;
        }
        return std::nullopt;
    }

    // Remove Value at index
    // Returns value removed
    bool Remove(const K& key) {
        std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
        Node<std::pair<K, V>>* prev = this->sentinel;
        Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
            if (ptr->value.first == key) {
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
        std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
        Node<std::pair<K, V>>* ptr = this->sentinel->next;
        while (ptr) {
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
            auto temp = ptr->next;
            curr_lock.unlock();
            delete(ptr);
            ptr = temp;
        }

        this->sentinel->next = nullptr;
        this->size = 0;
    }

private:

    Node<std::pair<K, V>>* sentinel;
    std::atomic<size_t> size;
};
