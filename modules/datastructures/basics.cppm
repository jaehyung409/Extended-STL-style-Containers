/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <optional>
#include <type_traits>

export module basics;

namespace j
{
    export template <class T, class Alloc>
    struct Uses_allocator : std::false_type {};
}