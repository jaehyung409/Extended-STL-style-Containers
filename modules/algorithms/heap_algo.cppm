/*
 * @ Created by jaehyung409 on 25. 2. 5.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License.
 */

module;
#include <iterator>

#if defined(__clang__)
export module j:heap_algo;
#else
module j:heap_algo;
#endif

namespace j {
template <std::random_access_iterator Iter, class Compare>
void _heapify(Iter first, std::ptrdiff_t parent, std::ptrdiff_t distance, Compare comp) {
    auto value = std::move(first[parent]);
    auto hole = parent;

    while (true) {
        auto child = 2 * hole + 1; // left
        if (child >= distance)
            break;

        if (child + 1 < distance && comp(first[child], first[child + 1])) {
            ++child; // right
        }

        if (!comp(value, first[child]))
            break;

        first[hole] = std::move(first[child]);
        hole = child;
    }
    first[hole] = std::move(value);
}

template <std::random_access_iterator Iter, class Compare> void _make_heap(Iter first, Iter last, Compare comp) {
    const auto distance = std::distance(first, last);
    if (distance < 2)
        return;
    for (auto i = distance / 2 - 1; i >= 0; --i) {
        _heapify(first, i, distance, comp);
    }
}

template <std::random_access_iterator Iter, class Compare> void _push_heap(Iter first, Iter last, Compare comp) {
    auto distance = std::distance(first, last);
    if (distance < 2)
        return;

    auto hole = distance - 1;
    auto value = std::move(first[hole]);
    auto parent = (hole - 1) / 2;

    while (hole > 0 && comp(first[parent], value)) {
        first[hole] = std::move(first[parent]);
        hole = parent;
        if (hole == 0)
            break;
        parent = (hole - 1) / 2;
    }

    first[hole] = std::move(value);
}

template <std::random_access_iterator Iter, class Compare> void _pop_heap(Iter first, Iter last, Compare comp) {
    const auto distance = std::distance(first, last);
    if (distance < 2)
        return;

    auto value = std::move(first[distance - 1]);
    first[distance - 1] = std::move(first[0]);

    std::ptrdiff_t hole = 0;
    const auto limit = distance - 1;

    while (true) {
        auto child = 2 * hole + 1;
        if (child >= limit)
            break;

        if (child + 1 < limit && comp(first[child], first[child + 1])) {
            ++child;
        }

        if (!comp(value, first[child]))
            break;

        first[hole] = std::move(first[child]);
        hole = child;
    }

    first[hole] = std::move(value);
}
} // namespace j
