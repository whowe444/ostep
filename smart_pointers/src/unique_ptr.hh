#pragma once

template<typename T>
class UniquePtr {

public:

    // Constructor
    UniquePtr() = delete;

    // All Args Constructor
    UniquePtr(T* raw_ptr)
        : 
        raw_ptr(raw_ptr)
    {
    }

    // Destructor
    ~UniquePtr() {
        delete this->raw_ptr;
    }

    // Move Assignment
    UniquePtr& operator=(UniquePtr<T>&& other) {
        // Prevent self assignment by comparing pointers.
        // You want to compare the ptrs for self-assignment
        // rather than the objects' eqs method.
        if (this != &other) {
            delete raw_ptr;
            this->raw_ptr = other.raw_ptr;
            other.raw_ptr = nullptr;
        }
        return *this;
    }

    // Move Constructor
    UniquePtr(UniquePtr&& other) noexcept
        :
            raw_ptr(other.raw_ptr)
    {
        other.raw_ptr = nullptr;
    }

    // Equals Operator
    bool operator==(const UniquePtr<T>& other) {
        return other.raw_ptr == this->raw_ptr;
    }

    bool operator!=(const UniquePtr<T>& other) {
        return *this == other;
    }

    // Delete the Copy Ctor
    UniquePtr(const UniquePtr& other) = delete;

    // Delete the Copy Assignment
    UniquePtr& operator=(const UniquePtr& other) = delete;

    T* raw_ptr;
};