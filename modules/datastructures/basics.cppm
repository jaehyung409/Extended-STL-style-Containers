/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <algorithm>
#include <optional>
#include <limits>
#include <type_traits>

export module basics;

namespace j
{
    export template <class T, class Alloc>
    struct uses_allocator : std::false_type {};

    // Operation (Monoid)
    export template <class T>
    struct plus
    {
        constexpr T operator()(const T &lhs, const T &rhs) const { return lhs + rhs; }
        constexpr T identity() const { return T(); }
    };

    export template <class T>
    struct max
    {
        constexpr T operator()(const T &lhs, const T &rhs) const { return std::max(lhs, rhs); }
        constexpr T identity() const { return std::numeric_limits<T>::min(); }
    };
}