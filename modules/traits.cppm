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
} // namespace j