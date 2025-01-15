/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <type_traits>

export module default_value;

namespace j
{
    export template <typename T>
    struct DefaultValue {
        static constexpr T value =
                std::conditional_t<std::is_arithmetic<T>::value,
                                   std::integral_constant<T, 0>,
                                   std::integral_constant<T*, nullptr>>::value;
    };
}