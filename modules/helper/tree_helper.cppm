/*
 * @ Created by jaehyung409 on 25. 2. 22.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>

export module j.tree_helper;

namespace j {
    export struct _dummy {};

    template <class T>
    struct _is_std_pair : std::false_type {};

    template <class K, class V>
    struct _is_std_pair<std::pair<K, V>> : std::true_type {};

    export template <class T>
    constexpr bool _is_std_pair_v = _is_std_pair<T>::value;

    template <class T>
    struct _has_dummy : std::false_type {};

    template <class K, class V>
    requires  std::same_as<V, _dummy>
    struct _has_dummy<std::pair<K, V>> : std::true_type {};

    export template <class T>
    constexpr bool _has_dummy_v = _has_dummy<T>::value;


    export template <class T, bool = _is_std_pair_v<T>, bool = _has_dummy_v<T>>
    struct _key_mapper;

    template <class T>
    struct _key_mapper<T, false> {
        using key_type = T;
        using mapped_type = T;
        using value_type = T;
    };

    template <class T>
    struct _key_mapper<T, true, false> {
        using key_type = typename T::first_type;
        using mapped_type = typename T::second_type;
        using value_type = T;
    };

    template <class T>
    struct _key_mapper<T, true, true> {
        using key_type = typename T::first_type;
        using mapped_type = typename T::first_type;
        using value_type = typename T::first_type;
    };

    export template <class T>
    struct _key_extractor {
        constexpr auto& extract(const T& value) {
            if constexpr (_is_std_pair_v<T>) {
                return value.first;
            } else {
                return value;
            }
        }
        constexpr const auto& extract(const T& value) const {
            if constexpr (_is_std_pair_v<T>) {
                return value.first;
            } else {
                return value;
            }
        }
    };

    export template <class T>
    struct _mapped_extractor {
        constexpr auto& extract(const T& value) {
            if constexpr (_is_std_pair_v<T>) {
                return value.second;
            } else {
                return value;
            }
        }
    };

    template <class T, bool IsConst, bool = _is_std_pair_v<T>, bool = _has_dummy_v<T>>
    struct _reference_selector_impl;

    template <class T, bool IsConst>
    struct _reference_selector_impl<T, IsConst, false> {
        using type = std::conditional_t<IsConst, const T&, T&>;
    };

    template <class T, bool IsConst>
    struct _reference_selector_impl<T, IsConst, true, false> {
    private:
        using K = typename T::first_type;
        using V = typename T::second_type;
    public:
        using type = std::pair<const K, std::conditional_t<IsConst, const V, V>>&;
    };

    template <class T, bool IsConst>
    struct _reference_selector_impl<T, IsConst, true, true> {
    private:
        using K = typename T::first_type;
    public:
        using type = std::conditional_t<IsConst, const K&, K&>;
    };

    export template <class T, bool IsConst>
    using _reference_selector = typename _reference_selector_impl<T, IsConst>::type;
}