#pragma once

#include "concurrent_linked_list.hh"

template<typename K, typename V>
class ConcurrentHashMap {

public:

    // Constructor
    ConcurrentHashMap() = default;

    // GetSize
    size_t GetSize() {
        return this->size.load();
    }

    // IsEmpty
    bool IsEmpty() {
        return this->size.load() == 0;
    }

    // Get
    std::optional<V> Get(const K& key) {
        return this->buckets[this->Hash(key)].Get(key);
    }

    // Insert
    bool Insert(const K& key, const V& value) {
        // Add {K, V} pair.
        bool tReturn = false;
        if (this->buckets[this->Hash(key)].Add(key, value)) {
            this->size++;
            tReturn = true;
        }
        return tReturn;
    }

    // Contains
    bool Contains(const K& key) {
        bool tReturn = false;
        if (this->buckets[this->Hash(key)].Contains(key)) {
            tReturn = true;
        }
        return tReturn;
    }

    // Remove
    bool Remove(const K& key) {
        bool tReturn = false;
        if (this->buckets[this->Hash(key)].Remove(key)) {
            this->size--;
            tReturn = true;
        }
        return tReturn;
    }

private:

    size_t Hash(const K& key) {
        return std::hash<K>{}(key) % NUM_BUCKETS;
    }

    std::atomic<int> size;
    static constexpr int NUM_BUCKETS = 10000;
    ConcurrentLinkedList<K, V> buckets[NUM_BUCKETS];

};