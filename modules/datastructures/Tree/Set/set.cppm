/*
 * @ Created by jaehyung409 on 25. 9. 25..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <algorithm>
#include <functional>

export module j:set;

import :traits;
import :tree_selector;

namespace j {
export template <class Key, class Compare, class Allocator, class TreeSelector = use_skip_list> class multiset;

export template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>,
                 class TreeSelector = use_skip_list>
class set {
  private:
    using traits = set_traits<Key, Compare, Allocator>;
    using tree_type = select_tree_t<traits, TreeSelector>;
    tree_type _tree;

  public:
    using key_type = typename traits::key_type;
    using key_compare = typename traits::key_compare;
    using value_type = typename traits::value_type;
    using value_compare = typename traits::value_compare;
    using allocator_type = typename traits::allocator_type;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = typename tree_type::size_type;
    using difference_type = typename tree_type::difference_type;
    using iterator = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = typename tree_type::node_type;
    using insert_return_type = typename tree_type::insert_return_type;

    // construct/copy/destroy
    set() : set(Compare()) {}
    explicit set(const Compare &comp, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    set(InputIter first, InputIter last, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
    set(const set &x) = default; // Rule of zero
    set(set &&x) = default;      // Rule of zero
    explicit set(const Allocator &alloc);
    set(const set &x, const std::type_identity_t<Allocator> &alloc);
    set(set &&x, const std::type_identity_t<Allocator> &alloc);
    set(std::initializer_list<value_type> il, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    set(InputIter first, InputIter last, const Allocator &a) : set(first, last, Compare(), a) {}
    set(std::initializer_list<value_type> il, const Allocator &a) : set(il, Compare(), a) {}
    ~set() = default; // Rule of zero

    set &operator=(const set &x) = default; // Rule of zero
    set &operator=(set &&x) = default;      // Rule of zero (noexcept depends on tree_type, compiler can optimize it)
    set &operator=(std::initializer_list<value_type> il);
    [[nodiscard]] allocator_type get_allocator() const noexcept;

    // iterators
    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] iterator end() noexcept;
    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;
    [[nodiscard]] const_reverse_iterator rend() const noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    // capacity
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;

    // modifiers
    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    std::pair<iterator, bool> emplace(Args &&...args);
    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    iterator emplace_hint(const_iterator position, Args &&...args);
    std::pair<iterator, bool> insert(const value_type &x);
    std::pair<iterator, bool> insert(value_type &&x);
    template <class K>
        requires std::constructible_from<value_type, K &&>
    std::pair<iterator, bool> insert(K &&x);
    iterator insert(const_iterator position, const value_type &x);
    iterator insert(const_iterator position, value_type &&x);
    template <class K>
        requires std::constructible_from<value_type, K &&>
    iterator insert(const_iterator position, K &&x);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    void insert(InputIter first, InputIter last);
    void insert(std::initializer_list<value_type> il);

    node_type extract(const_iterator position);
    node_type extract(const key_type &x);
    template <class K>
        requires(IsTransparentlyComparable<K, key_type, key_compare> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, iterator> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, const_iterator>)
    node_type extract(K &&x);
    insert_return_type insert(node_type &&nh);
    iterator insert(const_iterator hint, node_type &&nh);

    iterator erase(iterator position)
        requires(!std::same_as<iterator, const_iterator>);
    iterator erase(const_iterator position);
    size_type erase(const key_type &x);
    template <class K>
        requires(IsTransparentlyComparable<K, key_type, key_compare> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, iterator> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, const_iterator>)
    size_type erase(K &&x);
    iterator erase(const_iterator first, const_iterator last);
    void swap(set &x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                               std::is_nothrow_swappable_v<Compare>);
    void clear() noexcept;

    // Later, we will implement heterogeneous merge for different select_tree_t types.
    // For now, we only implement merge for the same select_tree_t type.
    // Also, we only implement fast-path only (same compare).
    // _tree is private, so we need to implementation `merge(source)` in base tree class. (enhancement)
    template <class C2> void merge(set<Key, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(set<Key, C2, Allocator, TreeSelector> &&source);
    template <class C2> void merge(multiset<Key, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(multiset<Key, C2, Allocator, TreeSelector> &&source);

    // observers
    [[nodiscard]] key_compare key_comp() const;
    [[nodiscard]] value_compare value_comp() const;

    // set operations
    [[nodiscard]] iterator find(const key_type &x);
    [[nodiscard]] const_iterator find(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] iterator find(const K &x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] const_iterator find(const K &x) const;

    [[nodiscard]] size_type count(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] size_type count(const K &x) const;

    [[nodiscard]] bool contains(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] bool contains(const K &x) const;

    [[nodiscard]] iterator lower_bound(const key_type &x);
    [[nodiscard]] const_iterator lower_bound(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] iterator lower_bound(const K &x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] const_iterator lower_bound(const K &x) const;

    [[nodiscard]] iterator upper_bound(const key_type &x);
    [[nodiscard]] const_iterator upper_bound(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] iterator upper_bound(const K &x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] const_iterator upper_bound(const K &x) const;

    [[nodiscard]] std::pair<iterator, iterator> equal_range(const key_type &x);
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] std::pair<iterator, iterator> equal_range(const K &x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const K &x) const;
};

template <class InputIter, class Compare = std::less<typename std::iterator_traits<InputIter>::value_type>,
          class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
set(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
    -> set<typename std::iterator_traits<InputIter>::value_type, Compare, Allocator>;

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
set(std::initializer_list<Key>, Compare = Compare(), Allocator = Allocator()) -> set<Key, Compare, Allocator>;

template <class InputIter, class Allocator>
set(InputIter, InputIter, Allocator) -> set<typename std::iterator_traits<InputIter>::value_type,
                                            std::less<typename std::iterator_traits<InputIter>::value_type>, Allocator>;

template <class Key, class Allocator> set(std::initializer_list<Key>, Allocator) -> set<Key, std::less<Key>, Allocator>;

export template <class Key, class Compare, class Allocator, class TreeSelector>
bool operator==(const set<Key, Compare, Allocator, TreeSelector> &lhs,
                const set<Key, Compare, Allocator, TreeSelector> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class Key, class Compare, class Allocator, class TreeSelector>
auto operator<=>(const set<Key, Compare, Allocator, TreeSelector> &lhs,
                 const set<Key, Compare, Allocator, TreeSelector> &rhs) -> std::strong_ordering {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class Key, class Compare, class Allocator, class TreeSelector>
void swap(set<Key, Compare, Allocator, TreeSelector> &x,
          set<Key, Compare, Allocator, TreeSelector> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class Key, class Compare, class Allocator, class TreeSelector, class Pred>
set<Key, Compare, Allocator, TreeSelector>::size_type erase_if(set<Key, Compare, Allocator, TreeSelector> &c,
                                                               Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}
} // namespace j

namespace j {
template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::set(const Compare &comp, const Allocator &alloc) : _tree(comp, alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter> &&
             std::constructible_from<typename set_traits<Key, Compare, Allocator>::value_type,
                                     std::iter_reference_t<InputIter>>
set<Key, Compare, Allocator, TreeSelector>::set(InputIter first, InputIter last, const Compare &comp,
                                                const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::set(const Allocator &alloc) : _tree(Compare(), alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::set(const set &x, const std::type_identity_t<Allocator> &alloc)
    : _tree(x._tree, alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::set(set &&x, const std::type_identity_t<Allocator> &alloc)
    : _tree(std::move(x._tree), alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::set(std::initializer_list<value_type> il, const Compare &comp,
                                                const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(il.begin(), il.end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector> &
set<Key, Compare, Allocator, TreeSelector>::operator=(std::initializer_list<value_type> il) {
    _tree.clear();
    _tree.insert(il.begin(), il.end());
    return *this;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::allocator_type
set<Key, Compare, Allocator, TreeSelector>::get_allocator() const noexcept {
    return _tree.get_allocator();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator set<Key, Compare, Allocator, TreeSelector>::begin() noexcept {
    return _tree.begin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::begin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator set<Key, Compare, Allocator, TreeSelector>::end() noexcept {
    return _tree.end();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::end() const noexcept {
    return _tree.cend();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::cbegin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::cend() const noexcept {
    return _tree.cend();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
set<Key, Compare, Allocator, TreeSelector>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
bool set<Key, Compare, Allocator, TreeSelector>::empty() const noexcept {
    return _tree.empty();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::size_type
set<Key, Compare, Allocator, TreeSelector>::size() const noexcept {
    return _tree.size();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::size_type
set<Key, Compare, Allocator, TreeSelector>::max_size() const noexcept {
    return _tree.max_size();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename set_traits<Key, Compare, Allocator>::value_type, Args &&...>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator, bool>
set<Key, Compare, Allocator, TreeSelector>::emplace(Args &&...args) {
    return _tree.emplace(std::forward<Args>(args)...);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename set_traits<Key, Compare, Allocator>::value_type, Args &&...>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::emplace_hint(const_iterator position, Args &&...args) {
    return _tree.emplace_hint(position, std::forward<Args>(args)...);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator, bool>
set<Key, Compare, Allocator, TreeSelector>::insert(const value_type &x) {
    return _tree.emplace(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator, bool>
set<Key, Compare, Allocator, TreeSelector>::insert(value_type &&x) {
    return _tree.emplace(std::move(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires std::constructible_from<typename set_traits<Key, Compare, Allocator>::value_type, K &&>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator, bool>
set<Key, Compare, Allocator, TreeSelector>::insert(K &&x) {
    return _tree.emplace(std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::insert(const_iterator position, const value_type &x) {
    return _tree.emplace_hint(position, x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::insert(const_iterator position, value_type &&x) {
    return _tree.emplace_hint(position, std::move(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires std::constructible_from<typename set_traits<Key, Compare, Allocator>::value_type, K &&>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::insert(const_iterator position, K &&x) {
    return _tree.emplace_hint(position, std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter>
void set<Key, Compare, Allocator, TreeSelector>::insert(InputIter first, InputIter last) {
    return _tree.insert(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void set<Key, Compare, Allocator, TreeSelector>::insert(std::initializer_list<value_type> il) {
    return _tree.insert(il.begin(), il.end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::node_type
set<Key, Compare, Allocator, TreeSelector>::extract(const_iterator position) {
    return _tree.extract(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::node_type
set<Key, Compare, Allocator, TreeSelector>::extract(const key_type &x) {
    return _tree.extract(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(
        IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                  typename set_traits<Key, Compare, Allocator>::key_compare> &&
        !std::is_convertible_v<std::remove_cvref_t<K>,
                               typename select_tree_t<set_traits<Key, Compare, Allocator>, TreeSelector>::iterator> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<set_traits<Key, Compare, Allocator>,
                                                                              TreeSelector>::const_iterator>)
set<Key, Compare, Allocator, TreeSelector>::node_type set<Key, Compare, Allocator, TreeSelector>::extract(K &&x) {
    return _tree.extract(std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::insert_return_type
set<Key, Compare, Allocator, TreeSelector>::insert(node_type &&nh) {
    return _tree.insert(std::move(nh));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::insert(const_iterator hint, node_type &&nh) {
    return _tree.insert(hint, std::move(nh));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::erase(iterator position)
    requires(!std::same_as<typename select_tree_t<set_traits<Key, Compare, Allocator>, TreeSelector>::iterator,
                           typename select_tree_t<set_traits<Key, Compare, Allocator>, TreeSelector>::const_iterator>)
{
    return _tree.erase(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::erase(const_iterator position) {
    return _tree.erase(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::size_type
set<Key, Compare, Allocator, TreeSelector>::erase(const key_type &x) {
    return _tree.erase(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(
        IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                  typename set_traits<Key, Compare, Allocator>::key_compare> &&
        !std::is_convertible_v<std::remove_cvref_t<K>,
                               typename select_tree_t<set_traits<Key, Compare, Allocator>, TreeSelector>::iterator> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<set_traits<Key, Compare, Allocator>,
                                                                              TreeSelector>::const_iterator>)
set<Key, Compare, Allocator, TreeSelector>::size_type set<Key, Compare, Allocator, TreeSelector>::erase(K &&x) {
    return _tree.erase(std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::erase(const_iterator first, const_iterator last) {
    return _tree.erase(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void set<Key, Compare, Allocator, TreeSelector>::swap(set &x) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>) {
    _tree.swap(x._tree);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void set<Key, Compare, Allocator, TreeSelector>::clear() noexcept {
    _tree.clear();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void set<Key, Compare, Allocator, TreeSelector>::merge(set<Key, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(source._tree);
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void set<Key, Compare, Allocator, TreeSelector>::merge(set<Key, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(std::move(source._tree));
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void set<Key, Compare, Allocator, TreeSelector>::merge(::j::multiset<Key, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(source._tree);
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void set<Key, Compare, Allocator, TreeSelector>::merge(::j::multiset<Key, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(std::move(source._tree));
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::key_compare set<Key, Compare, Allocator, TreeSelector>::key_comp() const {
    return _tree.key_comp();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::value_compare
set<Key, Compare, Allocator, TreeSelector>::value_comp() const {
    return _tree.value_comp();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::find(const key_type &x) {
    return _tree.find(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::find(const key_type &x) const {
    return _tree.find(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::iterator set<Key, Compare, Allocator, TreeSelector>::find(const K &x) {
    return _tree.find(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::find(const K &x) const {
    return _tree.find(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::size_type
set<Key, Compare, Allocator, TreeSelector>::count(const key_type &x) const {
    return _tree.count(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::size_type
set<Key, Compare, Allocator, TreeSelector>::count(const K &x) const {
    return _tree.count(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
bool set<Key, Compare, Allocator, TreeSelector>::contains(const key_type &x) const {
    return _tree.contains(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
bool set<Key, Compare, Allocator, TreeSelector>::contains(const K &x) const {
    return _tree.contains(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) {
    return _tree.lower_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) const {
    return _tree.lower_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::lower_bound(const K &x) {
    return _tree.lower_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::lower_bound(const K &x) const {
    return _tree.lower_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) {
    return _tree.upper_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) const {
    return _tree.upper_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::iterator
set<Key, Compare, Allocator, TreeSelector>::upper_bound(const K &x) {
    return _tree.upper_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
set<Key, Compare, Allocator, TreeSelector>::const_iterator
set<Key, Compare, Allocator, TreeSelector>::upper_bound(const K &x) const {
    return _tree.upper_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator,
          typename set<Key, Compare, Allocator, TreeSelector>::iterator>
set<Key, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) {
    return _tree.equal_range(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::const_iterator,
          typename set<Key, Compare, Allocator, TreeSelector>::const_iterator>
set<Key, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) const {
    return _tree.equal_range(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::iterator,
          typename set<Key, Compare, Allocator, TreeSelector>::iterator>
set<Key, Compare, Allocator, TreeSelector>::equal_range(const K &x) {
    return _tree.equal_range(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename set_traits<Key, Compare, Allocator>::key_type,
                                       typename set_traits<Key, Compare, Allocator>::key_compare>
std::pair<typename set<Key, Compare, Allocator, TreeSelector>::const_iterator,
          typename set<Key, Compare, Allocator, TreeSelector>::const_iterator>
set<Key, Compare, Allocator, TreeSelector>::equal_range(const K &x) const {
    return _tree.equal_range(std::forward<const K &>(x));
}
} // namespace j

namespace j {
export template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>, class TreeSelector>
class multiset {
  private:
    using traits = multiset_traits<Key, Compare, Allocator>;
    using tree_type = select_tree_t<traits, TreeSelector>;
    tree_type _tree;

  public:
    using key_type = typename traits::key_type;
    using key_compare = typename traits::key_compare;
    using value_type = typename traits::value_type;
    using value_compare = typename traits::value_compare;
    using allocator_type = typename traits::allocator_type;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = typename tree_type::size_type;
    using difference_type = typename tree_type::difference_type;
    using iterator = typename tree_type::iterator;
    using const_iterator = typename tree_type::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = typename tree_type::node_type;

    // construct/copy/destroy
    multiset() : multiset(Compare()) {}
    explicit multiset(const Compare &comp, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> &&
                 std::constructible_from<value_type, typename std::iterator_traits<InputIter>::reference>
    multiset(InputIter first, InputIter last, const Compare &comp = Compare(), const Allocator & = Allocator());
    multiset(const multiset &x) = default; // Rule of zero
    multiset(multiset &&x) = default;      // Rule of zero
    explicit multiset(const Allocator &alloc);
    multiset(const multiset &, const std::type_identity_t<Allocator> &alloc);
    multiset(multiset &&, const std::type_identity_t<Allocator> &alloc);
    multiset(std::initializer_list<value_type> il, const Compare &comp = Compare(),
             const Allocator &alloc = Allocator());
    template <class InputIter>
    multiset(InputIter first, InputIter last, const Allocator &a) : multiset(first, last, Compare(), a) {}
    multiset(std::initializer_list<value_type> il, const Allocator &a) : multiset(il, Compare(), a) {}
    multiset &operator=(const multiset &x) = default; // Rule of zero
    multiset &
    operator=(multiset &&x) = default; // Rule of zero (noexcept depends on tree_type, compiler can optimize it)
    multiset &operator=(std::initializer_list<value_type>);
    ~multiset() = default; // Rule of zero
    [[nodiscard]] allocator_type get_allocator() const noexcept;

    // iterators
    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] iterator end() noexcept;
    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;
    [[nodiscard]] const_reverse_iterator rend() const noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    // capacity
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;

    // modifiers
    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    iterator emplace(Args &&...args);
    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    iterator emplace_hint(const_iterator position, Args &&...args);
    iterator insert(const value_type &x);
    iterator insert(value_type &&x);
    iterator insert(const_iterator position, const value_type &x);
    iterator insert(const_iterator position, value_type &&x);
    template <class InputIter> void insert(InputIter first, InputIter last);
    void insert(std::initializer_list<value_type> il);

    node_type extract(const_iterator position);
    node_type extract(const key_type &x);
    template <class K> node_type extract(K &&x);
    iterator insert(node_type &&nh);
    iterator insert(const_iterator hint, node_type &&nh);

    iterator erase(iterator position)
        requires(!std::same_as<iterator, const_iterator>);
    iterator erase(const_iterator position);
    size_type erase(const key_type &x);
    template <class K> size_type erase(K &&x);
    iterator erase(const_iterator first, const_iterator last);
    void swap(multiset &) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                   std::is_nothrow_swappable_v<Compare>);
    void clear() noexcept;

    template <class C2> void merge(multiset<Key, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(multiset<Key, C2, Allocator, TreeSelector> &&source);
    template <class C2> void merge(set<Key, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(set<Key, C2, Allocator, TreeSelector> &&source);

    // observers
    [[nodiscard]] key_compare key_comp() const;
    [[nodiscard]] value_compare value_comp() const;

    // set operations
    [[nodiscard]] iterator find(const key_type &x);
    [[nodiscard]] const_iterator find(const key_type &x) const;
    template <class K> [[nodiscard]] iterator find(const K &x);
    template <class K> [[nodiscard]] const_iterator find(const K &x) const;

    [[nodiscard]] size_type count(const key_type &x) const;
    template <class K> [[nodiscard]] size_type count(const K &x) const;

    [[nodiscard]] bool contains(const key_type &x) const;
    template <class K> [[nodiscard]] bool contains(const K &x) const;

    [[nodiscard]] iterator lower_bound(const key_type &x);
    [[nodiscard]] const_iterator lower_bound(const key_type &x) const;
    template <class K> [[nodiscard]] iterator lower_bound(const K &x);
    template <class K> [[nodiscard]] const_iterator lower_bound(const K &x) const;

    [[nodiscard]] iterator upper_bound(const key_type &x);
    [[nodiscard]] const_iterator upper_bound(const key_type &x) const;
    template <class K> [[nodiscard]] iterator upper_bound(const K &x);
    template <class K> [[nodiscard]] const_iterator upper_bound(const K &x) const;

    [[nodiscard]] std::pair<iterator, iterator> equal_range(const key_type &x);
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
    template <class K> [[nodiscard]] std::pair<iterator, iterator> equal_range(const K &x);
    template <class K> [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const K &x) const;
};

template <class InputIter, class Compare = std::less<typename std::iterator_traits<InputIter>::value_type>,
          class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
multiset(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
    -> multiset<typename std::iterator_traits<InputIter>::value_type, Compare, Allocator>;

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
multiset(std::initializer_list<Key>, Compare = Compare(), Allocator = Allocator()) -> multiset<Key, Compare, Allocator>;

template <class InputIter, class Allocator>
multiset(InputIter, InputIter, Allocator)
    -> multiset<typename std::iterator_traits<InputIter>::value_type,
                std::less<typename std::iterator_traits<InputIter>::value_type>, Allocator>;

template <class Key, class Allocator>
multiset(std::initializer_list<Key>, Allocator) -> multiset<Key, std::less<Key>, Allocator>;

export template <class Key, class Compare, class Allocator, class TreeSelector>
bool operator==(const multiset<Key, Compare, Allocator, TreeSelector> &lhs,
                const multiset<Key, Compare, Allocator, TreeSelector> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class Key, class Compare, class Allocator, class TreeSelector>
auto operator<=>(const multiset<Key, Compare, Allocator, TreeSelector> &lhs,
                 const multiset<Key, Compare, Allocator, TreeSelector> &rhs) -> std::strong_ordering {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class Key, class Compare, class Allocator, class TreeSelector>
void swap(multiset<Key, Compare, Allocator, TreeSelector> &x,
          multiset<Key, Compare, Allocator, TreeSelector> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class Key, class Compare, class Allocator, class TreeSelector, class Pred>
set<Key, Compare, Allocator, TreeSelector>::size_type erase_if(multiset<Key, Compare, Allocator, TreeSelector> &c,
                                                               Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}
} // namespace j

namespace j {
template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(const Compare &comp, const Allocator &alloc)
    : _tree(comp, alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter> &&
             std::constructible_from<typename multiset_traits<Key, Compare, Allocator>::value_type,
                                     typename std::iterator_traits<InputIter>::reference>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(InputIter first, InputIter last, const Compare &comp,
                                                          const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(const Allocator &alloc) : _tree(Compare(), alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(const multiset &x,
                                                          const std::type_identity_t<Allocator> &alloc)
    : _tree(x._tree, alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(multiset &&x, const std::type_identity_t<Allocator> &alloc)
    : _tree(std::move(x._tree), alloc) {}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::multiset(std::initializer_list<value_type> il, const Compare &comp,
                                                          const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(il.begin(), il.end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector> &
multiset<Key, Compare, Allocator, TreeSelector>::operator=(std::initializer_list<value_type> il) {
    _tree.clear();
    _tree.insert(il.begin(), il.end());
    return *this;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::allocator_type
multiset<Key, Compare, Allocator, TreeSelector>::get_allocator() const noexcept {
    return _tree.get_allocator();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::begin() noexcept {
    return _tree.begin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::begin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::end() noexcept {
    return _tree.end();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::end() const noexcept {
    return _tree.cend();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::cbegin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::cend() const noexcept {
    return _tree.cend();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_reverse_iterator
multiset<Key, Compare, Allocator, TreeSelector>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
bool multiset<Key, Compare, Allocator, TreeSelector>::empty() const noexcept {
    return _tree.empty();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::size() const noexcept {
    return _tree.size();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::max_size() const noexcept {
    return _tree.max_size();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename multiset_traits<Key, Compare, Allocator>::value_type, Args &&...>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::emplace(Args &&...args) {
    return _tree.emplace(std::forward<Args>(args)...).first;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename multiset_traits<Key, Compare, Allocator>::value_type, Args &&...>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::emplace_hint(const_iterator position, Args &&...args) {
    return _tree.emplace_hint(position, std::forward<Args>(args)...);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(const value_type &x) {
    return _tree.emplace(x).first;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(value_type &&x) {
    return _tree.emplace(std::move(x)).first;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(const_iterator position, const value_type &x) {
    return _tree.emplace_hint(position, x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(const_iterator position, value_type &&x) {
    return _tree.emplace_hint(position, std::move(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
void multiset<Key, Compare, Allocator, TreeSelector>::insert(InputIter first, InputIter last) {
    return _tree.insert(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void multiset<Key, Compare, Allocator, TreeSelector>::insert(std::initializer_list<value_type> il) {
    return _tree.insert(il.begin(), il.end());
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::node_type
multiset<Key, Compare, Allocator, TreeSelector>::extract(const_iterator position) {
    return _tree.extract(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::node_type
multiset<Key, Compare, Allocator, TreeSelector>::extract(const key_type &x) {
    return _tree.extract(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::node_type
multiset<Key, Compare, Allocator, TreeSelector>::extract(K &&x) {
    return _tree.extract(std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(node_type &&nh) {
    return _tree.insert(std::move(nh)).position;
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::insert(const_iterator hint, node_type &&nh) {
    return _tree.insert_hint(hint, std::move(nh));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::erase(iterator position)
    requires(
        !std::same_as<typename select_tree_t<multiset_traits<Key, Compare, Allocator>, TreeSelector>::iterator,
                      typename select_tree_t<multiset_traits<Key, Compare, Allocator>, TreeSelector>::const_iterator>)
{
    return _tree.erase(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::erase(const_iterator position) {
    return _tree.erase(position);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::erase(const key_type &x) {
    return _tree.erase(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::erase(K &&x) {
    return _tree.erase(std::forward<K>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::erase(const_iterator first, const_iterator last) {
    return _tree.erase(first, last);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void multiset<Key, Compare, Allocator, TreeSelector>::swap(multiset &x) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>) {
    _tree.swap(x._tree);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
void multiset<Key, Compare, Allocator, TreeSelector>::clear() noexcept {
    _tree.clear();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multiset<Key, Compare, Allocator, TreeSelector>::merge(multiset<Key, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(source._tree);
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multiset<Key, Compare, Allocator, TreeSelector>::merge(multiset<Key, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(std::move(source._tree));
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multiset<Key, Compare, Allocator, TreeSelector>::merge(set<Key, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(source._tree);
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multiset<Key, Compare, Allocator, TreeSelector>::merge(set<Key, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) { // Fast path
        return _tree.merge(std::move(source._tree));
    } // Slow path will be implemented later
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::key_compare
multiset<Key, Compare, Allocator, TreeSelector>::key_comp() const {
    return _tree.key_comp();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::value_compare
multiset<Key, Compare, Allocator, TreeSelector>::value_comp() const {
    return _tree.value_comp();
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::find(const key_type &x) {
    return _tree.find(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::find(const key_type &x) const {
    return _tree.find(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::find(const K &x) {
    return _tree.find(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::find(const K &x) const {
    return _tree.find(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::count(const key_type &x) const {
    return _tree.count(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::size_type
multiset<Key, Compare, Allocator, TreeSelector>::count(const K &x) const {
    return _tree.count(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
bool multiset<Key, Compare, Allocator, TreeSelector>::contains(const key_type &x) const {
    return _tree.contains(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
bool multiset<Key, Compare, Allocator, TreeSelector>::contains(const K &x) const {
    return _tree.contains(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) {
    return _tree.lower_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) const {
    return _tree.lower_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::lower_bound(const K &x) {
    return _tree.lower_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::lower_bound(const K &x) const {
    return _tree.lower_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) {
    return _tree.upper_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) const {
    return _tree.upper_bound(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::iterator
multiset<Key, Compare, Allocator, TreeSelector>::upper_bound(const K &x) {
    return _tree.upper_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
multiset<Key, Compare, Allocator, TreeSelector>::const_iterator
multiset<Key, Compare, Allocator, TreeSelector>::upper_bound(const K &x) const {
    return _tree.upper_bound(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename multiset<Key, Compare, Allocator, TreeSelector>::iterator,
          typename multiset<Key, Compare, Allocator, TreeSelector>::iterator>
multiset<Key, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) {
    return _tree.equal_range(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
std::pair<typename multiset<Key, Compare, Allocator, TreeSelector>::const_iterator,
          typename multiset<Key, Compare, Allocator, TreeSelector>::const_iterator>
multiset<Key, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) const {
    return _tree.equal_range(x);
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
std::pair<typename multiset<Key, Compare, Allocator, TreeSelector>::iterator,
          typename multiset<Key, Compare, Allocator, TreeSelector>::iterator>
multiset<Key, Compare, Allocator, TreeSelector>::equal_range(const K &x) {
    return _tree.equal_range(std::forward<const K &>(x));
}

template <class Key, class Compare, class Allocator, class TreeSelector>
template <class K>
std::pair<typename multiset<Key, Compare, Allocator, TreeSelector>::const_iterator,
          typename multiset<Key, Compare, Allocator, TreeSelector>::const_iterator>
multiset<Key, Compare, Allocator, TreeSelector>::equal_range(const K &x) const {
    return _tree.equal_range(std::forward<const K &>(x));
}
} // namespace j