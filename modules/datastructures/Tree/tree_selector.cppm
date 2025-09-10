/*
 * @ Created by jaehyung409 on 25. 9. 10..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

#if defined(__clang__)
export module j:tree_selector;
#else
module j:tree_selector;
#endif

namespace j {
// tree policy tag
// ex: struct use_XX{}; (draft)

// concept
// template <typename Policy>
// concept TreePolicy = std::is_same_v<Policy use_XX> || ... ;

// template <class Value, class Compare, class Alloc, class Policy>
// struct tree_selector;
//
// template <class Value, class Compare, class Alloc>
// struct tree_selector<Value, Compare, Alloc, use_XX> {
//     using type = XX<Value, Compare, Alloc>;
// };
}