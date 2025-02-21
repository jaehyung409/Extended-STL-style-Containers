/*
 * @ Created by jaehyung409 on 25. 2. 5.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <iterator>

export module j.heap_algo;

namespace j {
    template <std::random_access_iterator Iter, class Compare>
    void heapify(Iter first, Iter parent, std::ptrdiff_t distance, Compare comp) {
        while (parent < distance / 2) {
            auto left = 2 * parent + 1;
            auto right = 2 * parent + 2;
            auto largest = parent;
            if (comp(first[largest], first[left])) {
                largest = left;
            }
            if (right < distance && comp(first[largest], first[right])) {
                largest = right;
            }
            if (largest == parent) break;
            std::swap(first[parent], first[largest]);
            parent = largest;
        }
    }

    export template <std::random_access_iterator Iter, class Compare>
    void make_heap(Iter first, Iter last, Compare comp) {
        if (first == last) return;
        auto distance = std::distance(first, last);
        for (auto i = distance / 2 - 1; i >= 0; --i) {
            auto parent = i;
            heapify(first, parent, distance, comp);
        }
    }

    export template <std::random_access_iterator Iter>
    void make_heap(Iter first, Iter last) {
        make_heap(first, last, std::less<>());
    }

    export template <std::random_access_iterator Iter, class Compare>
    void push_heap(Iter first, Iter last, Compare comp) {
        if (first == last) return;
        auto distance = std::distance(first, last);
        auto child = distance - 1;
        while (child > 0) {
            auto parent = (child - 1) / 2;
            if (comp(first[parent], first[child])) {
                std::swap(first[parent], first[child]);
                child = parent;
            } else {
                break;
            }
        }
    }

    export template <std::random_access_iterator Iter>
    void push_heap(Iter first, Iter last) {
        push_heap(first, last, std::less<>());
    }

    export template <std::random_access_iterator Iter, class Compare>
    void pop_heap(Iter first, Iter last, Compare comp) {
        if (first == last) return;
        auto distance = std::distance(first, last);
        std::swap(first[0], first[distance - 1]);
        auto parent = 0;
        heapify(first, parent, distance, comp);
    }

    export template <std::random_access_iterator Iter>
    void pop_heap(Iter first, Iter last) {
        pop_heap(first, last, std::less<>());
    }
}