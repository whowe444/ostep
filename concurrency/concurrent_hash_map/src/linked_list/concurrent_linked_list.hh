#pragma once

#include "./node.hh"
#include <vector>
#include <atomic>

template<typename K, typename V>
class ConcurrentLinkedList {

public:

    // Constructor 
    ConcurrentLinkedList() {
        sentinel = new linked_list::Node<std::pair<K, V>>();
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
    bool Add(const K& key, const V& value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        linked_list::Node<std::pair<K, V>>* prev = this->sentinel;
        linked_list::Node<std::pair<K, V>>* ptr = prev->next;

        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            prev_lock = std::move(curr_lock);

            // Updating an existing key
            if (ptr->value.first == key) {
                ptr->value = {key, value};
                return true;
            }

            prev = ptr;
            ptr = ptr->next;
        }

        prev->next = new linked_list::Node<std::pair<K, V>>(nullptr, {key, value});
        this->size++;
        return true;
    }

    // Value is Contained in the List.
    bool Contains(const K& key) {
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        linked_list::Node<std::pair<K, V>>* prev = this->sentinel;
        linked_list::Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
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
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        linked_list::Node<std::pair<K, V>>* prev = this->sentinel;
        linked_list::Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
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
    std::optional<V> Remove(const K& key) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        linked_list::Node<std::pair<K, V>>* prev = this->sentinel;
        linked_list::Node<std::pair<K, V>>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            if (ptr->value.first == key) {
                // Delete this node
                auto return_value = ptr->value.second;
                prev->next = ptr->next;
                this->size--;
                curr_lock.unlock();
                delete(ptr);
                return return_value;
            } else {
                prev_lock = std::move(curr_lock);

                ptr = ptr->next;
                prev = prev->next;
            }
        }
        return std::nullopt;
    }

    // Clear out the list so that it is
    // empty.
    void Clear() {
        if (!sentinel) return;
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        linked_list::Node<std::pair<K, V>>* ptr = this->sentinel->next;
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

    linked_list::Node<std::pair<K, V>>* sentinel;
    std::atomic<size_t> size;
};
