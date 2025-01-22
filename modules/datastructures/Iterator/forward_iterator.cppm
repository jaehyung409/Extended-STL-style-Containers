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
    export template<class Node>
    class Forward_iterator {
    public:
        using value_type = typename Node::value_type;
        using pointer = Node*;
        using const_pointer = const Node*;
        using reference = value_type&;
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