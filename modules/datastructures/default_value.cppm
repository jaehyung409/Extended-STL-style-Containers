/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <optional>
#include <type_traits>

export module default_value;

namespace j
{
    export template <typename T>
    class Default_value {
    public:
        Default_value() {
            if constexpr (std::is_default_constructible_v<T>) {
                value_ = T();
            } else {
                value_ = std::nullopt;
            }
        }

        std::optional<T> get() const {
            return value_;
        }

    private:
        std::optional<T> value_;
    };
}