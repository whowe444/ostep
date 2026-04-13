#pragma once

#include <memory>
#include <shared_mutex>

template<typename T>
class Node {

public:

    // Constructor
    Node() = default;

    // Copy Ctor (non-copyable)
    Node(const Node& other) = delete;

    // Copy Assignment (non-copyable)
    Node& operator=(const Node& other) = delete;

    // Move Ctor (non-movable)
    Node(Node&& other) = delete;

    // Move Assignment
    Node& operator=(Node&& other) = delete;

    // All Args Constructor
    Node(Node<T>* node, T value)
        : 
        next(node),
        value(value)
    {
    }

    Node<T>* next;
    T value;
    std::shared_mutex mtx;

};