/*
 * @ Created by jaehyung409 on 25. 1. 18.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <iterator>

export module forward_iterator;

import default_value;

namespace j {
    export template<typename T>
    class Forward_iterator {
    public:
        class Node {
        public:
            T value;
            Node* next;
            Node() : value(Default_value<T>().get().value_or(T())), next(nullptr) {}
            explicit Node(const T& value) : value(value), next(nullptr) {}
            explicit Node(T&& value) : value(std::move(value)), next(nullptr) {}
        };

        using value_type = T;
        using pointer = Node*;
        using const_pointer = const Node*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;

    public:
        Forward_iterator(pointer ptr = nullptr) : ptr(ptr) {}
        Forward_iterator(const_pointer ptr) : ptr(const_cast<pointer>(ptr)) {}
        reference operator*() const { return ptr->value; }
        pointer operator->() const { return ptr; }

        Forward_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        Forward_iterator operator++(int) {
            Forward_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Forward_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Forward_iterator& other) const { return ptr != other.ptr; }
    };
}