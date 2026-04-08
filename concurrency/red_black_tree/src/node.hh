#pragma once

enum class Color {
    Black,
    Red
};

template<typename T>
class Node {

public:

    // Constructor
    Node() = default;

    // All Args Constructor
    Node(Node<T>* left, Node<T>* right, Node<T>* parent,
            Color color, T value)
        : 
        left(left),
        right(right),
        color(color),
        value(value)
    {
    }

    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Color color;
    T value;
};