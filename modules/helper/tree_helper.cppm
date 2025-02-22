/*
 * @ Created by jaehyung409 on 25. 2. 22.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>

export module j.tree_helper;

namespace j {
    template <class T>
    struct is_std_pair : std::false_type {};

    template <class K, class V>
    struct is_std_pair<std::pair<K, V>> : std::true_type {};

    export template <class T>
    constexpr bool is_std_pair_v = is_std_pair<T>::value;

    export template <class T, bool = is_std_pair_v<T>>
    struct key_mapper;

    template <class T>
    struct key_mapper<T, false> {
        using key_type = T;
        using mapped_type = T;
    };

    template <class T>
    struct key_mapper<T, true> {
        using key_type = typename T::first_type;
        using mapped_type = typename T::second_type;
    };

    template <class T, bool IsConst, bool = is_std_pair_v<T>>
    struct reference_selector_impl;

    template <class T, bool IsConst>
    struct reference_selector_impl<T, IsConst, false> {
        using type = std::conditional_t<IsConst, const T&, T&>;
    };

    template <class T, bool IsConst>
    struct reference_selector_impl<T, IsConst, true> {
    private:
        using K = typename T::first_type;
        using V = typename T::second_type;
    public:
        using type = std::pair<const K, std::conditional_t<IsConst, const V, V>>&;
    };

    export template <class T, bool IsConst>
    using reference_selector = typename reference_selector_impl<T, IsConst>::type;

    export template <class Key>
    struct default_compare {
        bool operator()(Key const &lhs, Key const& rhs) const {
            if constexpr (is_std_pair_v<Key>) {
                return lhs.first < rhs.first;
            } else {
                return lhs < rhs;
            }
        }
    };
}