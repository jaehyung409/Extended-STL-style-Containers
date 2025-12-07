/*
 * @ Created by jaehyung409 on 25. 9. 10..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <utility>

#if defined(__clang__)
export module j:traits;
#else
module j:traits;
#endif

import :concepts;

namespace j {
template <class Key, class Compare, class Allocator> struct set_traits {
    using key_type = Key;
    using mapped_type = void;
    using value_type = key_type;
    using key_compare = Compare;
    using value_compare = key_compare;
    using allocator_type = Allocator;
    static constexpr bool _MULTI = false;
};

template <class Key, class Compare, class Allocator> struct multiset_traits {
    using key_type = Key;
    using mapped_type = void;
    using value_type = key_type;
    using key_compare = Compare;
    using value_compare = key_compare;
    using allocator_type = Allocator;
    static constexpr bool _MULTI = true;
};

template <class Key, class T, class Compare, class Allocator> struct map_traits {
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;
    struct value_compare {
        friend struct map_traits;

      public:
        Compare comp;
        explicit value_compare(Compare c) : comp(c) {}
        bool operator()(const value_type &lhs, const value_type &rhs) const {
            return comp(lhs.first, rhs.first);
        }
        template <class K>
            requires IsTransparentlyComparable<K, key_type, Compare>
        bool operator()(const value_type &lhs, const K &rhs) const {
            return comp(lhs.first, rhs);
        }
        template <class K>
            requires IsTransparentlyComparable<K, key_type, Compare>
        bool operator()(const K &lhs, const value_type &rhs) const {
            return comp(lhs, rhs.first);
        }
    };
    using allocator_type = Allocator;
    static constexpr bool _MULTI = false;
};

template <class Key, class T, class Compare, class Allocator> struct multimap_traits {
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;
    struct value_compare {
        friend struct multimap_traits;

      public:
        Compare comp;
        explicit value_compare(Compare c) : comp(c) {}
        bool operator()(const value_type &lhs, const value_type &rhs) const {
            return comp(lhs.first, rhs.first);
        }
        template <class K>
            requires IsTransparentlyComparable<K, key_type, Compare>
        bool operator()(const value_type &lhs, const K &rhs) const {
            return comp(lhs.first, rhs);
        }
        template <class K>
            requires IsTransparentlyComparable<K, key_type, Compare>
        bool operator()(const K &lhs, const value_type &rhs) const {
            return comp(lhs, rhs.first);
        }
    };
    using allocator_type = Allocator;
    static constexpr bool _MULTI = true;
};
} // namespace j
