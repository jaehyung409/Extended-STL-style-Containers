/*
 * @ Created by jaehyung409 on 25. 7. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <ranges>

export module j.range_basics;

export template <typename R, typename T>
concept container_compatible_range =
    std::ranges::input_range<R> &&
    std::convertible_to<std::ranges::range_reference_t<R>, T>;

namespace std::ranges {
    export struct from_range_t { explicit from_range_t() = default; };
    export inline constexpr from_range_t from_range{};
}
