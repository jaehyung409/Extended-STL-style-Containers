/*
 * @ Created by jaehyung409 on 25. 9. 10..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

export module j:tree_selector;

import :skip_list;
// import :red_black_tree;
// import :avl_tree;

namespace j {
struct use_red_black_tree {};
export struct use_skip_list {};
struct use_avl_tree {};

template <class Traits, class Selector> struct select_tree {
    using type = skip_list<Traits, false>;
};

template <class Traits> struct select_tree<Traits, use_skip_list> {
    using type = skip_list<Traits, true>;
};

template <class Traits> struct select_tree<Traits, use_red_black_tree> {
    // using type = red_black_tree<Traits>;
};

template <class Traits> struct select_tree<Traits, use_avl_tree> {
    // using type = avl_tree<Traits>;
};

/* if custom
 * template <class Traits>
 * struct select_tree<Traits, use_custom_tree> {
 *   using type = custom_tree<Traits>;
 * };
 */

export template <class Traits, class Selector> using select_tree_t = typename select_tree<Traits, Selector>::type;
} // namespace j