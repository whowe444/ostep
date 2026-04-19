#pragma once

#include "linked_list/concurrent_linked_list.hh"

template<typename K, typename V, template<typename X, typename Z> class C>
class ConcurrentHashMap {

public:

    // Iterator Template Definition
    template<bool IsConst>
    struct IteratorTemplate {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<K, V>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const std::pair<K, V>*, std::pair<K, V>*>;
        using reference = std::conditional_t<IsConst, const std::pair<K, V>&, std::pair<K, V>&>;

        using range_type = std::conditional_t<IsConst, typename C<K, V>::SharedRange,
            typename C<K, V>::UniqueRange>;

        using iterator_type = std::conditional_t<IsConst, typename C<K, V>::ConstIterator,
            typename C<K, V>::Iterator>;
    public:

        range_type make_range(int currentIndex) {
            if constexpr (IsConst) {
                return (currentIndex < map->NUM_BUCKETS) ? 
                    map->buckets[currentIndex].GetSharedRange() :
                    map->buckets[NUM_BUCKETS-1].GetSharedRange();
            } else {
                return (currentIndex < map->NUM_BUCKETS) ? 
                    map->buckets[currentIndex].GetUniqueRange() :
                    map->buckets[NUM_BUCKETS-1].GetUniqueRange();
            }
        }

        // Constructor
        IteratorTemplate(ConcurrentHashMap* map_, int currentIndex_) 
            :
                map(map_),
                currentIndex(currentIndex_),
                range(make_range(currentIndex)),
                current(currentIndex < map->NUM_BUCKETS ? range.begin() : range.end())
        {
        }

        // Define the dereference operator.
        reference operator*() const { return *current;}

        // Define the pointer access operator.
        pointer operator->() const { return &(*current);}

        // Define the pre-increment operator.
        IteratorTemplate<IsConst>& operator++() {
            // First move the iterator forward.
            ++current;

            while(current == range.end()) {
                currentIndex++;
                // If the current index is out of bounds
                // break out of the loop and return the
                // current value of the iterator (which should be end()).
                if (currentIndex >= map->buckets.size()) break;

                // Move to the next bucket
                if constexpr (IsConst) {
                    range = map->buckets[currentIndex].GetSharedRange();
                } else {
                    range = map->buckets[currentIndex].GetUniqueRange();
                }

                // Set current
                current = range.begin();
            }

            return *this;
        }

        // Define the post-increment operator.
        IteratorTemplate<IsConst> operator++(int) {
            // Grab a copy of the current iterator.
            auto tmp = *this;

            // Dereference current iter and pre-increment it.
            ++(*this);

            // Return the iter before pre-incrementing.
            return tmp;
        }

        // Define the pre-decrement operator.
        IteratorTemplate<IsConst>& operator--() {
            // Move the iterator back.
            if (current != range.begin()) {
                --current;
            } else {
                --currentIndex;
                while (currentIndex >= 0 && map->buckets[currentIndex].IsEmpty()) {
                    --currentIndex;
                }

                if (currentIndex >= 0) {                    
                    // Move to the previous bucket
                    if constexpr (IsConst) {
                        range = map->buckets[currentIndex].GetSharedRange();
                    } else {
                        range = map->buckets[currentIndex].GetUniqueRange();
                    }

                    // Set current
                    current = range.end();
                    --current;
                }
            }

            return *this;
        }

        // Define the post-decrement operator.
        IteratorTemplate<IsConst> operator--(int) {
            // Grab a copy of the current iterator.
            auto tmp = *this;

            // Dereference current iter and pre-decrement it.
            --(*this);

            // Return the iter before pre-decrementing.
            return tmp;
        }

        // Define the equals operator.
        bool operator==(const IteratorTemplate<IsConst>& other) const { 
            return currentIndex == other.currentIndex && current == other.current;
        }

        // Define the not equals operator.
        bool operator!=(const IteratorTemplate<IsConst>& other) const {
            return !(*this == other);
        }

    private:
        
        // Pointer to the map which this iterator belongs to.
        ConcurrentHashMap* map;

        // Bucket of the current iterator.
        int currentIndex;

        // Range Type which holds the lock.
        range_type range;

        // Current Iterator
        iterator_type current;

    };

    using Iterator = IteratorTemplate<false>;
    using ConstIterator = IteratorTemplate<true>;

    Iterator begin() {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            if (!buckets[i].IsEmpty()) {
                return Iterator(this, i);
            }
        }
        return end();
    }

    Iterator end() {
        return Iterator(this, NUM_BUCKETS);
    }

    // Constructor
    ConcurrentHashMap() = default;

    // Copy Ctor
    ConcurrentHashMap(const ConcurrentHashMap& other) = delete;

    // Copy Assignment
    ConcurrentHashMap& operator=(const ConcurrentHashMap& other) = delete;

    // Move Ctor
    ConcurrentHashMap(ConcurrentHashMap&& other) = delete;

    // Move Assignment
    ConcurrentHashMap& operator=(ConcurrentHashMap&& other) = delete;

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
    C<K, V> buckets[NUM_BUCKETS];

};