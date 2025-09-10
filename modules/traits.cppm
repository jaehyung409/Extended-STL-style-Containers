/*
 * @ Created by jaehyung409 on 25. 9. 10..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

module;
#include <utility>

export module j:traits;

namespace j {
template <class T>
struct key_traits {
    using key_type = T;
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
// need to add iterator related traits (const/non-const iterator)
}