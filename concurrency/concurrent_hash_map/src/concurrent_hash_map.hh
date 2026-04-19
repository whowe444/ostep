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

        using map_type = std::conditional_t<IsConst, const ConcurrentHashMap, ConcurrentHashMap>;
    public:

        std::optional<range_type> make_range(int currentIndex) {
            if (currentIndex >= map->NUM_BUCKETS) return std::nullopt;
            if constexpr (IsConst) {
                return map->buckets[currentIndex].GetSharedRange();
            } else {
                return map->buckets[currentIndex].GetUniqueRange();
            }
        }

        // Constructor
        IteratorTemplate(map_type* map_, int currentIndex_) 
            :
                map(map_),
                currentIndex(currentIndex_),
                range(make_range(currentIndex)),
                current(range.has_value() ? std::optional<iterator_type>(range->begin()) : std::nullopt) //std::optional<iterator_type>(iterator_type{nullptr, nullptr}))
        {
        }

        // Define the dereference operator.
        reference operator*() const { 
            assert(current.has_value() && "Dereferencing end() iterator is undefined behavior.");
            return **current;
        }

        // Define the pointer access operator.
        pointer operator->() const { 
            assert(current.has_value() && "Dereferencing end() iterator is undefined behavior.");
            return &(**current);
        }

        // Define the pre-increment operator.
        IteratorTemplate<IsConst>& operator++() {
            // First move the iterator forward.
            assert(current.has_value() && "Incrementing end() iterator is undefined behavior.");
            ++*current;

            while(*current == range->end()) {
                currentIndex++;
                // If the current index is out of bounds
                // break out of the loop and return the
                // current value of the iterator (which should be end()).
                if (currentIndex >= map->NUM_BUCKETS) {
                    range = std::nullopt;
                    current = std::nullopt;
                    break;
                }

                // Move to the next bucket
                if constexpr (IsConst) {
                    range = map->buckets[currentIndex].GetSharedRange();
                } else {
                    range = map->buckets[currentIndex].GetUniqueRange();
                }

                // Set current
                current.emplace(range->begin());
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
            assert(current.has_value() && "Decrementing end() iterator is undefined behavior.");
            if (*current != range->begin()) {
                --*current;
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
                    *current = range->end();
                    --*current;
                } else {
                    // Set current to nullopt to indicate that we are before the beginning of the map.
                    range = std::nullopt;
                    current = std::nullopt;
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
            if (currentIndex != other.currentIndex) return false;
            if (!current.has_value() && !other.current.has_value()) return true;
            if (!current.has_value() || !other.current.has_value()) return false;
            return *current == *other.current;
        }

        // Define the not equals operator.
        bool operator!=(const IteratorTemplate<IsConst>& other) const {
            return !(*this == other);
        }

    private:
        
        // Pointer to the map which this iterator belongs to.
        map_type* map;

        // Bucket of the current iterator.
        int currentIndex;

        // Range Type which holds the lock.
        std::optional<range_type> range;

        // Current Iterator
        std::optional<iterator_type> current;

    };

    using Iterator = IteratorTemplate<false>;
    using ConstIterator = IteratorTemplate<true>;

    // Iterator Begin Function.
    // Use: Read/Write based iteration through the map.
    Iterator begin() {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            if (!buckets[i].IsEmpty()) {
                return Iterator(this, i);
            }
        }
        return end();
    }

    // Iterator End Function.
    // Use: Read/Write based iteration through the map.
    Iterator end() {
        return Iterator(this, NUM_BUCKETS);
    }

    // Const Iterator Begin Function.
    // Use: Read-only iteration through the map.
    ConstIterator cbegin() const {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            if (!buckets[i].IsEmpty()) {
                return ConstIterator(this, i);
            }
        }
        return cend();
    }

    // Const Iterator End Function.
    // Use: Read-only iteration through the map.
    ConstIterator cend() const {
        return ConstIterator(this, NUM_BUCKETS);
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
    bool IsEmpty() const {
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