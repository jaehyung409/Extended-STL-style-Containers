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
template <class T>
struct key_traits {
    using key_type = T;
    using mapped_type = void;
    using value_type = T;

    static const key_type& extract_key(const value_type& value) {
        return value;
    }
};

template <class K, class V>
struct key_traits<std::pair<const K, V>> {
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;

    static const key_type& extract_key(const value_type& value) {
        return value.first;
    }
};

template <class Key, class Value, class Compare, class Traits>
struct value_compare_traits {
    using value_compare = std::conditional_t<
        std::is_same_v<Value, Key>,
        Compare,
        struct value_compare_impl {
        protected:
            Compare comp;
            explicit value_compare_impl(const Compare& c) : comp(c) {}
        public:
            bool operator()(const Value& lhs, const Value& rhs) const {
                return comp(Traits::extract_key(lhs), Traits::extract_key(rhs));
            }
        }
    >;
};
// need to add iterator related traits (const/non-const iterator)
}