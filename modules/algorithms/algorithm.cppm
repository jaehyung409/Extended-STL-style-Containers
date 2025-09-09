/*
 * @ Created by jaehyung409 on 25. 8. 28..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>

export module j:algorithm;

import :heap_algo;

namespace j {
    export template <std::random_access_iterator Iter, class Compare>
    void make_heap(Iter first, Iter last, Compare comp) {
        _make_heap(first, last, comp);
    }

    export template <std::random_access_iterator Iter>
    void make_heap(Iter first, Iter last) {
        _make_heap(first, last, std::less<>());
    }

    export template <std::random_access_iterator Iter, class Compare>
    void push_heap(Iter first, Iter last, Compare comp) {
        _push_heap(first, last, comp);
    }

    export template <std::random_access_iterator Iter>
    void push_heap(Iter first, Iter last) {
        _push_heap(first, last, std::less<>());
    }

    export template <std::random_access_iterator Iter, class Compare>
    void pop_heap(Iter first, Iter last, Compare comp) {
        _pop_heap(first, last, comp);
    }

    export template <std::random_access_iterator Iter>
    void pop_heap(Iter first, Iter last) {
        _pop_heap(first, last, std::less<>());
    }
}