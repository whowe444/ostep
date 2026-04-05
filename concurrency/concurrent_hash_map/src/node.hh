#pragma once

#include <memory>
#include <mutex>

template<typename T>
class Node {

public:

    // Constructor
    Node() = default;

    // All Args Constructor
    Node(Node<T>* node, T value)
        : 
        next(node),
        value(value)
    {
    }

    Node<T>* next;
    T value;
    std::mutex mtx;


};