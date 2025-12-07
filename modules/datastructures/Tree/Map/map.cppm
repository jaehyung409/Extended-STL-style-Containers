/*
 * @ Created by jaehyung409 on 25. 9. 25..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <algorithm>
#include <functional>
#include <stdexcept>

export module j:map;

import :traits;
import :tree_selector;

namespace j {
export template <class Key, class T, class Compare, class Allocator, class TreeSelector> class multimap;

export template <class Key, class T, class Compare = std::less<Key>,
                 class Allocator = std::allocator<std::pair<const Key, T>>, class TreeSelector = use_skip_list>
class map {
  private:
    using traits = map_traits<Key, T, Compare, Allocator>;
    using tree_type = select_tree_t<traits, TreeSelector>;
    tree_type _tree;

  public:
    using key_type = typename traits::key_type;
    using key_compare = typename traits::key_compare;
    using mapped_type = typename traits::mapped_type;
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
    map() : map(Compare()) {}
    explicit map(const Compare &comp, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    map(InputIter first, InputIter last, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
    map(const map &x) = default; // Rule of zero
    map(map &&x) = default;      // Rule of zero
    explicit map(const Allocator &alloc);
    map(const map &x, const std::type_identity_t<Allocator> &alloc);
    map(map &&x, const std::type_identity_t<Allocator> &alloc);
    map(std::initializer_list<value_type> il, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    map(InputIter first, InputIter last, const Allocator &a) : map(first, last, Compare(), a) {}
    map(std::initializer_list<value_type> il, const Allocator &a) : map(il, Compare(), a) {}
    ~map() = default; // Rule of zero

    map &operator=(const map &x) = default; // Rule of zero
    map &operator=(map &&x) = default;      // Rule of zero (noexcept depends on tree_type, compiler can optimize it)
    map &operator=(std::initializer_list<value_type> il);
    [[nodiscard]] allocator_type get_allocator() const noexcept;

    mapped_type &at(const key_type &x);
    const mapped_type &at(const key_type &x) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    mapped_type &at(const K &x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const mapped_type &at(const K &x) const;

    mapped_type &operator[](const key_type &x);
    mapped_type &operator[](key_type &&x);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare> && std::constructible_from<key_type, K>
    mapped_type &operator[](K &&x);

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

    template <class... Args> std::pair<iterator, bool> try_emplace(const key_type &k, Args &&...args);
    template <class... Args> std::pair<iterator, bool> try_emplace(key_type &&k, Args &&...args);
    template <class K, class... Args>
        requires IsTransparentlyComparable<K, key_type, key_compare> && std::constructible_from<key_type, K>
    std::pair<iterator, bool> try_emplace(K &&k, Args &&...args);
    template <class... Args>
    std::pair<iterator, bool> try_emplace(const_iterator hint, const key_type &k, Args &&...args);
    template <class... Args> std::pair<iterator, bool> try_emplace(const_iterator hint, key_type &&k, Args &&...args);
    template <class K, class... Args>
        requires IsTransparentlyComparable<K, key_type, key_compare> && std::constructible_from<key_type, K>
    std::pair<iterator, bool> try_emplace(const_iterator hint, K &&k, Args &&...args);

    template <class M> std::pair<iterator, bool> insert_or_assign(const key_type &k, M &&obj);
    template <class M> std::pair<iterator, bool> insert_or_assign(key_type &&k, M &&obj);
    template <class K, class M>
        requires IsTransparentlyComparable<K, key_type, key_compare> && std::constructible_from<key_type, K>
    std::pair<iterator, bool> insert_or_assign(K &&k, M &&obj);
    template <class M> iterator insert_or_assign(const_iterator hint, const key_type &k, M &&obj);
    template <class M> iterator insert_or_assign(const_iterator hint, key_type &&k, M &&obj);
    template <class K, class M>
        requires IsTransparentlyComparable<K, key_type, key_compare> && std::constructible_from<key_type, K>
    iterator insert_or_assign(const_iterator hint, K &&k, M &&obj);

    std::pair<iterator, bool> insert(const value_type &x);
    std::pair<iterator, bool> insert(value_type &&x);
    template <class P>
        requires std::constructible_from<value_type, P &&>
    std::pair<iterator, bool> insert(P &&x);
    iterator insert(const_iterator position, const value_type &x);
    iterator insert(const_iterator position, value_type &&x);
    template <class P>
        requires std::constructible_from<value_type, P &&>
    iterator insert(const_iterator position, P &&x);
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
    void swap(map &x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                               std::is_nothrow_swappable_v<Compare>);
    void clear() noexcept;

    // Later, we will implement heterogeneous merge for different select_tree_t types.
    // For now, we only implement merge for the same select_tree_t type.
    // Also, we only implement fast-path only (same compare).
    // _tree is private, so we need to implementation `merge(source)` in base tree class. (enhancement)
    template <class C2> void merge(map<Key, T, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(map<Key, T, C2, Allocator, TreeSelector> &&source);
    template <class C2> void merge(multimap<Key, T, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(multimap<Key, T, C2, Allocator, TreeSelector> &&source);

    // observers
    [[nodiscard]] key_compare key_comp() const;
    [[nodiscard]] value_compare value_comp() const;

    // map operations
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

template <class InputIter, class Compare = std::less<typename std::iterator_traits<InputIter>::value_type::first_type>,
          class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
map(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
    -> map<typename std::iterator_traits<InputIter>::value_type::first_type,
           typename std::iterator_traits<InputIter>::value_type::second_type, Compare, Allocator>;

template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
map(std::initializer_list<std::pair<const Key, T>>, Compare = Compare(), Allocator = Allocator())
    -> map<Key, T, Compare, Allocator>;

template <class InputIter, class Allocator>
map(InputIter, InputIter, Allocator)
    -> map<typename std::iterator_traits<InputIter>::value_type::first_type,
           typename std::iterator_traits<InputIter>::value_type::second_type,
           std::less<typename std::iterator_traits<InputIter>::value_type::first_type>, Allocator>;

template <class Key, class T, class Allocator>
map(std::initializer_list<std::pair<const Key, T>>, Allocator) -> map<Key, T, std::less<Key>, Allocator>;

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool operator==(const map<Key, T, Compare, Allocator, TreeSelector> &lhs,
                const map<Key, T, Compare, Allocator, TreeSelector> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
auto operator<=>(const map<Key, T, Compare, Allocator, TreeSelector> &lhs,
                 const map<Key, T, Compare, Allocator, TreeSelector> &rhs) -> std::strong_ordering {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void swap(map<Key, T, Compare, Allocator, TreeSelector> &x,
          map<Key, T, Compare, Allocator, TreeSelector> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector, class Pred>
map<Key, T, Compare, Allocator, TreeSelector>::size_type erase_if(map<Key, T, Compare, Allocator, TreeSelector> &c,
                                                                  Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}
} // namespace j

namespace j {
template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::map(const Compare &comp, const Allocator &alloc) : _tree(comp, alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::value_type,
                                     std::iter_reference_t<InputIter>>
map<Key, T, Compare, Allocator, TreeSelector>::map(InputIter first, InputIter last, const Compare &comp,
                                                   const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::map(const Allocator &alloc) : _tree(Compare(), alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::map(const map &x, const std::type_identity_t<Allocator> &alloc)
    : _tree(x._tree, alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::map(map &&x, const std::type_identity_t<Allocator> &alloc)
    : _tree(std::move(x._tree), alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::map(std::initializer_list<value_type> il, const Compare &comp,
                                                   const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(il.begin(), il.end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector> &
map<Key, T, Compare, Allocator, TreeSelector>::operator=(std::initializer_list<value_type> il) {
    _tree.clear();
    _tree.insert(il.begin(), il.end());
    return *this;
}

// element access
template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::at(const key_type &x) {
    auto it = _tree.find(x);
    if (it == _tree.end()) {
        throw std::out_of_range("map::at() : key not found");
    }
    return it->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
const map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::at(const key_type &x) const {
    auto it = _tree.find(x);
    if (it == _tree.end()) {
        throw std::out_of_range("map::at() : key not found");
    }
    return it->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::at(const K &x) {
    auto it = _tree.find(x);
    if (it == _tree.end()) {
        throw std::out_of_range("map::at(): key not found");
    }
    return it->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
const map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::at(const K &x) const {
    auto it = _tree.find(x);
    if (it == _tree.end()) {
        throw std::out_of_range("map::at(): key not found");
    }
    return it->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::operator[](const key_type &x) {
    return _tree.try_emplace(x).first->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::operator[](key_type &&x) {
    return _tree.try_emplace(std::move(x)).first->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::key_type, K>
map<Key, T, Compare, Allocator, TreeSelector>::mapped_type &
map<Key, T, Compare, Allocator, TreeSelector>::operator[](K &&x) {
    return _tree.try_emplace(std::forward<K>(x)).first->second;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::begin() noexcept {
    return _tree.begin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::begin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator map<Key, T, Compare, Allocator, TreeSelector>::end() noexcept {
    return _tree.end();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::end() const noexcept {
    return _tree.cend();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::cbegin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::cend() const noexcept {
    return _tree.cend();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
map<Key, T, Compare, Allocator, TreeSelector>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool map<Key, T, Compare, Allocator, TreeSelector>::empty() const noexcept {
    return _tree.empty();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::size_type
map<Key, T, Compare, Allocator, TreeSelector>::size() const noexcept {
    return _tree.size();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::size_type
map<Key, T, Compare, Allocator, TreeSelector>::max_size() const noexcept {
    return _tree.max_size();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::value_type, Args &&...>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::emplace(Args &&...args) {
    return _tree.emplace(std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::value_type, Args &&...>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::emplace_hint(const_iterator position, Args &&...args) {
    return _tree.emplace_hint(position, std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(const key_type &k, Args &&...args) {
    return _tree.try_emplace(k, std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(key_type &&k, Args &&...args) {
    return _tree.try_emplace(std::move(k), std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K, class... Args>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::key_type, K>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(K &&k, Args &&...args) {
    return _tree.try_emplace(std::forward<K>(k), std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(const_iterator hint, const key_type &k, Args &&...args) {
    return _tree.try_emplace(hint, k, std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(const_iterator hint, key_type &&k, Args &&...args) {
    return _tree.try_emplace(hint, std::move(k), std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K, class... Args>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::key_type, K>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::try_emplace(const_iterator hint, K &&k, Args &&...args) {
    return _tree.try_emplace(hint, std::forward<K>(k), std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class M>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(const key_type &k, M &&obj) {
    return _tree.insert_or_assign(k, std::forward<M>(obj));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class M>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(key_type &&k, M &&obj) {
    return _tree.insert_or_assign(std::move(k), std::forward<M>(obj));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K, class M>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::key_type, K>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(K &&k, M &&obj) {
    return _tree.insert_or_assign(std::forward<K>(k), std::forward<M>(obj));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class M>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(const_iterator hint, const key_type &k, M &&obj) {
    return _tree.insert_or_assign(hint, k, std::forward<M>(obj)).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class M>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(const_iterator hint, key_type &&k, M &&obj) {
    return _tree.insert_or_assign(hint, std::move(k), std::forward<M>(obj)).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K, class M>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
             std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::key_type, K>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert_or_assign(const_iterator hint, K &&k, M &&obj) {
    return _tree.insert_or_assign(hint, std::forward<K>(k), std::forward<M>(obj)).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert(const value_type &x) {
    return _tree.emplace(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert(value_type &&x) {
    return _tree.emplace(std::move(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class P>
    requires std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::value_type, P &&>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator, bool>
map<Key, T, Compare, Allocator, TreeSelector>::insert(P &&x) {
    return _tree.emplace(std::forward<P>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, const value_type &x) {
    return _tree.emplace_hint(position, x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, value_type &&x) {
    return _tree.emplace_hint(position, std::move(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class P>
    requires std::constructible_from<typename map_traits<Key, T, Compare, Allocator>::value_type, P &&>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, P &&x) {
    return _tree.emplace_hint(position, std::forward<P>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter>
void map<Key, T, Compare, Allocator, TreeSelector>::insert(InputIter first, InputIter last) {
    return _tree.insert(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void map<Key, T, Compare, Allocator, TreeSelector>::insert(std::initializer_list<value_type> il) {
    return _tree.insert(il.begin(), il.end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::node_type
map<Key, T, Compare, Allocator, TreeSelector>::extract(const_iterator position) {
    return _tree.extract(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::node_type
map<Key, T, Compare, Allocator, TreeSelector>::extract(const key_type &x) {
    return _tree.extract(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(
        IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                  typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<map_traits<Key, T, Compare, Allocator>,
                                                                              TreeSelector>::iterator> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<map_traits<Key, T, Compare, Allocator>,
                                                                              TreeSelector>::const_iterator>)
map<Key, T, Compare, Allocator, TreeSelector>::node_type map<Key, T, Compare, Allocator, TreeSelector>::extract(K &&x) {
    return _tree.extract(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::insert_return_type
map<Key, T, Compare, Allocator, TreeSelector>::insert(node_type &&nh) {
    return _tree.insert(std::move(nh));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator hint, node_type &&nh) {
    return _tree.insert(hint, std::move(nh));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::erase(iterator position)
    requires(
        !std::same_as<typename select_tree_t<map_traits<Key, T, Compare, Allocator>, TreeSelector>::iterator,
                      typename select_tree_t<map_traits<Key, T, Compare, Allocator>, TreeSelector>::const_iterator>)
{
    return _tree.erase(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::erase(const_iterator position) {
    return _tree.erase(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::size_type
map<Key, T, Compare, Allocator, TreeSelector>::erase(const key_type &x) {
    return _tree.erase(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(
        IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                  typename map_traits<Key, T, Compare, Allocator>::key_compare> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<map_traits<Key, T, Compare, Allocator>,
                                                                              TreeSelector>::iterator> &&
        !std::is_convertible_v<std::remove_cvref_t<K>, typename select_tree_t<map_traits<Key, T, Compare, Allocator>,
                                                                              TreeSelector>::const_iterator>)
map<Key, T, Compare, Allocator, TreeSelector>::size_type map<Key, T, Compare, Allocator, TreeSelector>::erase(K &&x) {
    return _tree.erase(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::erase(const_iterator first, const_iterator last) {
    return _tree.erase(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void map<Key, T, Compare, Allocator, TreeSelector>::swap(map &x) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>) {
    _tree.swap(x._tree);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void map<Key, T, Compare, Allocator, TreeSelector>::clear() noexcept {
    _tree.clear();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void map<Key, T, Compare, Allocator, TreeSelector>::merge(map<Key, T, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        return _tree.merge(source._tree);
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void map<Key, T, Compare, Allocator, TreeSelector>::merge(map<Key, T, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        return _tree.merge(std::move(source._tree));
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void map<Key, T, Compare, Allocator, TreeSelector>::merge(multimap<Key, T, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        return _tree.merge(source._tree);
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void map<Key, T, Compare, Allocator, TreeSelector>::merge(multimap<Key, T, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        return _tree.merge(std::move(source._tree));
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::key_compare
map<Key, T, Compare, Allocator, TreeSelector>::key_comp() const {
    return _tree.key_comp();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::value_compare
map<Key, T, Compare, Allocator, TreeSelector>::value_comp() const {
    return value_compare(_tree.key_comp());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::find(const key_type &x) {
    return _tree.find(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::find(const key_type &x) const {
    return _tree.find(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::find(const K &x) {
    return _tree.find(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::find(const K &x) const {
    return _tree.find(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::size_type
map<Key, T, Compare, Allocator, TreeSelector>::count(const key_type &x) const {
    return _tree.count(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::size_type
map<Key, T, Compare, Allocator, TreeSelector>::count(const K &x) const {
    return _tree.count(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool map<Key, T, Compare, Allocator, TreeSelector>::contains(const key_type &x) const {
    return _tree.contains(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
bool map<Key, T, Compare, Allocator, TreeSelector>::contains(const K &x) const {
    return _tree.contains(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) {
    return _tree.lower_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) const {
    return _tree.lower_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const K &x) {
    return _tree.lower_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const K &x) const {
    return _tree.lower_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) {
    return _tree.upper_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) const {
    return _tree.upper_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::iterator
map<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const K &x) {
    return _tree.upper_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
map<Key, T, Compare, Allocator, TreeSelector>::const_iterator
map<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const K &x) const {
    return _tree.upper_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator,
          typename map<Key, T, Compare, Allocator, TreeSelector>::iterator>
map<Key, T, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) {
    return _tree.equal_range(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::const_iterator,
          typename map<Key, T, Compare, Allocator, TreeSelector>::const_iterator>
map<Key, T, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) const {
    return _tree.equal_range(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::iterator,
          typename map<Key, T, Compare, Allocator, TreeSelector>::iterator>
map<Key, T, Compare, Allocator, TreeSelector>::equal_range(const K &x) {
    return _tree.equal_range(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename map_traits<Key, T, Compare, Allocator>::key_type,
                                       typename map_traits<Key, T, Compare, Allocator>::key_compare>
std::pair<typename map<Key, T, Compare, Allocator, TreeSelector>::const_iterator,
          typename map<Key, T, Compare, Allocator, TreeSelector>::const_iterator>
map<Key, T, Compare, Allocator, TreeSelector>::equal_range(const K &x) const {
    return _tree.equal_range(std::forward<K>(x));
}
} // namespace j

namespace j {
export template <class Key, class T, class Compare = std::less<Key>,
                 class Allocator = std::allocator<std::pair<const Key, T>>, class TreeSelector = use_skip_list>
class multimap {
  private:
    using traits = multimap_traits<Key, T, Compare, Allocator>;
    using tree_type = select_tree_t<traits, TreeSelector>;
    tree_type _tree;

  public:
    using key_type = typename traits::key_type;
    using mapped_type = typename traits::mapped_type;
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
    multimap() : multimap(Compare()) {}
    explicit multimap(const Compare &comp, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    multimap(InputIter first, InputIter last, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
    multimap(const multimap &x) = default;
    multimap(multimap &&x) = default;
    explicit multimap(const Allocator &alloc);
    multimap(const multimap &x, const std::type_identity_t<Allocator> &alloc);
    multimap(multimap &&x, const std::type_identity_t<Allocator> &alloc);
    multimap(std::initializer_list<value_type> il, const Compare &comp = Compare(),
             const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter> && std::constructible_from<value_type, std::iter_reference_t<InputIter>>
    multimap(InputIter first, InputIter last, const Allocator &a) : multimap(first, last, Compare(), a) {}
    multimap(std::initializer_list<value_type> il, const Allocator &a) : multimap(il, Compare(), a) {}
    ~multimap() = default;

    multimap &operator=(const multimap &x) = default;
    multimap &operator=(multimap &&x) = default;
    multimap &operator=(std::initializer_list<value_type> il);
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
    template <class P>
        requires std::constructible_from<value_type, P &&>
    iterator insert(P &&x);
    iterator insert(const_iterator position, const value_type &x);
    iterator insert(const_iterator position, value_type &&x);
    template <class P>
        requires std::constructible_from<value_type, P &&>
    iterator insert(const_iterator position, P &&x);
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
    iterator insert(node_type &&nh);
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
    void swap(multimap &x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
                                    std::is_nothrow_swappable_v<Compare>);
    void clear() noexcept;

    // Later, we will implement heterogeneous merge for different select_tree_t types.
    // For now, we only implement merge for the same select_tree_t type.
    // Also, we only implement fast-path only (same compare).
    // _tree is private, so we need to implementation `merge(source)` in base tree class. (enhancement)
    template <class C2> void merge(multimap<Key, T, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(multimap<Key, T, C2, Allocator, TreeSelector> &&source);
    template <class C2> void merge(map<Key, T, C2, Allocator, TreeSelector> &source);
    template <class C2> void merge(map<Key, T, C2, Allocator, TreeSelector> &&source);

    // observers
    [[nodiscard]] key_compare key_comp() const;
    [[nodiscard]] value_compare value_comp() const;

    // map operations
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

template <class InputIter, class Compare = std::less<typename std::iterator_traits<InputIter>::value_type::first_type>,
          class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
multimap(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
    -> multimap<typename std::iterator_traits<InputIter>::value_type::first_type,
                typename std::iterator_traits<InputIter>::value_type::second_type, Compare, Allocator>;

template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
multimap(std::initializer_list<std::pair<const Key, T>>, Compare = Compare(), Allocator = Allocator())
    -> multimap<Key, T, Compare, Allocator>;

template <class InputIter, class Allocator>
multimap(InputIter, InputIter, Allocator)
    -> multimap<typename std::iterator_traits<InputIter>::value_type::first_type,
                typename std::iterator_traits<InputIter>::value_type::second_type,
                std::less<typename std::iterator_traits<InputIter>::value_type::first_type>, Allocator>;

template <class Key, class T, class Allocator>
multimap(std::initializer_list<std::pair<const Key, T>>, Allocator) -> multimap<Key, T, std::less<Key>, Allocator>;

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool operator==(const multimap<Key, T, Compare, Allocator, TreeSelector> &lhs,
                const multimap<Key, T, Compare, Allocator, TreeSelector> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
auto operator<=>(const multimap<Key, T, Compare, Allocator, TreeSelector> &lhs,
                 const multimap<Key, T, Compare, Allocator, TreeSelector> &rhs) -> std::strong_ordering {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void swap(multimap<Key, T, Compare, Allocator, TreeSelector> &x,
          multimap<Key, T, Compare, Allocator, TreeSelector> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class Key, class T, class Compare, class Allocator, class TreeSelector, class Pred>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
erase_if(multimap<Key, T, Compare, Allocator, TreeSelector> &c, Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}
} // namespace j

namespace j {
template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(const Compare &comp, const Allocator &alloc)
    : _tree(comp, alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter> &&
             std::constructible_from<typename multimap_traits<Key, T, Compare, Allocator>::value_type,
                                     std::iter_reference_t<InputIter>>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(InputIter first, InputIter last, const Compare &comp,
                                                             const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(const Allocator &alloc) : _tree(Compare(), alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(const multimap &x,
                                                             const std::type_identity_t<Allocator> &alloc)
    : _tree(x._tree, alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(multimap &&x, const std::type_identity_t<Allocator> &alloc)
    : _tree(std::move(x._tree), alloc) {}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::multimap(std::initializer_list<value_type> il, const Compare &comp,
                                                             const Allocator &alloc)
    : _tree(comp, alloc) {
    _tree.insert(il.begin(), il.end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector> &
multimap<Key, T, Compare, Allocator, TreeSelector>::operator=(std::initializer_list<value_type> il) {
    _tree.clear();
    _tree.insert(il.begin(), il.end());
    return *this;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::allocator_type
multimap<Key, T, Compare, Allocator, TreeSelector>::get_allocator() const noexcept {
    return _tree.get_allocator();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::begin() noexcept {
    return _tree.begin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::begin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::end() noexcept {
    return _tree.end();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::end() const noexcept {
    return _tree.cend();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::cbegin() const noexcept {
    return _tree.cbegin();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::cend() const noexcept {
    return _tree.cend();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_reverse_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool multimap<Key, T, Compare, Allocator, TreeSelector>::empty() const noexcept {
    return _tree.empty();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::size() const noexcept {
    return _tree.size();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::max_size() const noexcept {
    return _tree.max_size();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename multimap_traits<Key, T, Compare, Allocator>::value_type, Args &&...>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::emplace(Args &&...args) {
    return _tree.emplace(std::forward<Args>(args)...).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class... Args>
    requires std::constructible_from<typename multimap_traits<Key, T, Compare, Allocator>::value_type, Args &&...>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::emplace_hint(const_iterator position, Args &&...args) {
    return _tree.emplace_hint(position, std::forward<Args>(args)...);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(const value_type &x) {
    return _tree.emplace(x).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(value_type &&x) {
    return _tree.emplace(std::move(x)).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class P>
    requires std::constructible_from<typename multimap_traits<Key, T, Compare, Allocator>::value_type, P &&>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(P &&x) {
    return _tree.emplace(std::forward<P>(x)).first;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, const value_type &x) {
    return _tree.emplace_hint(position, x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, value_type &&x) {
    return _tree.emplace_hint(position, std::move(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class P>
    requires std::constructible_from<typename multimap_traits<Key, T, Compare, Allocator>::value_type, P &&>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator position, P &&x) {
    return _tree.emplace_hint(position, std::forward<P>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class InputIter>
    requires std::input_iterator<InputIter>
void multimap<Key, T, Compare, Allocator, TreeSelector>::insert(InputIter first, InputIter last) {
    _tree.insert(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void multimap<Key, T, Compare, Allocator, TreeSelector>::insert(std::initializer_list<value_type> il) {
    _tree.insert(il.begin(), il.end());
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::node_type
multimap<Key, T, Compare, Allocator, TreeSelector>::extract(const_iterator position) {
    return _tree.extract(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::node_type
multimap<Key, T, Compare, Allocator, TreeSelector>::extract(const key_type &x) {
    return _tree.extract(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare> &&
             !std::is_convertible_v<
                 std::remove_cvref_t<K>,
                 typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::iterator> &&
             !std::is_convertible_v<
                 std::remove_cvref_t<K>,
                 typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::const_iterator>)
multimap<Key, T, Compare, Allocator, TreeSelector>::node_type
multimap<Key, T, Compare, Allocator, TreeSelector>::extract(K &&x) {
    return _tree.extract(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(node_type &&nh) {
    return _tree.insert(std::move(nh)).position;
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::insert(const_iterator hint, node_type &&nh) {
    return _tree.insert(hint, std::move(nh));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::erase(iterator position)
    requires(!std::same_as<
             typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::iterator,
             typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::const_iterator>)
{
    return _tree.erase(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::erase(const_iterator position) {
    return _tree.erase(position);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::erase(const key_type &x) {
    return _tree.erase(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires(IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare> &&
             !std::is_convertible_v<
                 std::remove_cvref_t<K>,
                 typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::iterator> &&
             !std::is_convertible_v<
                 std::remove_cvref_t<K>,
                 typename select_tree_t<multimap_traits<Key, T, Compare, Allocator>, TreeSelector>::const_iterator>)
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::erase(K &&x) {
    return _tree.erase(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::erase(const_iterator first, const_iterator last) {
    return _tree.erase(first, last);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void multimap<Key, T, Compare, Allocator, TreeSelector>::swap(multimap &x) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable_v<Compare>) {
    _tree.swap(x._tree);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
void multimap<Key, T, Compare, Allocator, TreeSelector>::clear() noexcept {
    _tree.clear();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multimap<Key, T, Compare, Allocator, TreeSelector>::merge(multimap<Key, T, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        _tree.merge(source._tree);
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multimap<Key, T, Compare, Allocator, TreeSelector>::merge(multimap<Key, T, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        _tree.merge(std::move(source._tree));
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multimap<Key, T, Compare, Allocator, TreeSelector>::merge(::j::map<Key, T, C2, Allocator, TreeSelector> &source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        _tree.merge(source._tree);
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class C2>
void multimap<Key, T, Compare, Allocator, TreeSelector>::merge(::j::map<Key, T, C2, Allocator, TreeSelector> &&source) {
    if constexpr (std::is_same_v<Compare, std::remove_cvref_t<C2>>) {
        _tree.merge(std::move(source._tree));
    }
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::key_compare
multimap<Key, T, Compare, Allocator, TreeSelector>::key_comp() const {
    return _tree.key_comp();
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::value_compare
multimap<Key, T, Compare, Allocator, TreeSelector>::value_comp() const {
    return value_compare(_tree.key_comp());
}

// multimap operations (delegate to tree)
template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::find(const key_type &x) {
    return _tree.find(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::find(const key_type &x) const {
    return _tree.find(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::find(const K &x) {
    return _tree.find(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::find(const K &x) const {
    return _tree.find(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::count(const key_type &x) const {
    return _tree.count(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::size_type
multimap<Key, T, Compare, Allocator, TreeSelector>::count(const K &x) const {
    return _tree.count(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
bool multimap<Key, T, Compare, Allocator, TreeSelector>::contains(const key_type &x) const {
    return _tree.contains(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
bool multimap<Key, T, Compare, Allocator, TreeSelector>::contains(const K &x) const {
    return _tree.contains(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) {
    return _tree.lower_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const key_type &x) const {
    return _tree.lower_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const K &x) {
    return _tree.lower_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::lower_bound(const K &x) const {
    return _tree.lower_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) {
    return _tree.upper_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const key_type &x) const {
    return _tree.upper_bound(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const K &x) {
    return _tree.upper_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator
multimap<Key, T, Compare, Allocator, TreeSelector>::upper_bound(const K &x) const {
    return _tree.upper_bound(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename multimap<Key, T, Compare, Allocator, TreeSelector>::iterator,
          typename multimap<Key, T, Compare, Allocator, TreeSelector>::iterator>
multimap<Key, T, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) {
    return _tree.equal_range(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
std::pair<typename multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator,
          typename multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator>
multimap<Key, T, Compare, Allocator, TreeSelector>::equal_range(const key_type &x) const {
    return _tree.equal_range(x);
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
std::pair<typename multimap<Key, T, Compare, Allocator, TreeSelector>::iterator,
          typename multimap<Key, T, Compare, Allocator, TreeSelector>::iterator>
multimap<Key, T, Compare, Allocator, TreeSelector>::equal_range(const K &x) {
    return _tree.equal_range(std::forward<K>(x));
}

template <class Key, class T, class Compare, class Allocator, class TreeSelector>
template <class K>
    requires IsTransparentlyComparable<K, typename multimap_traits<Key, T, Compare, Allocator>::key_type,
                                       typename multimap_traits<Key, T, Compare, Allocator>::key_compare>
std::pair<typename multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator,
          typename multimap<Key, T, Compare, Allocator, TreeSelector>::const_iterator>
multimap<Key, T, Compare, Allocator, TreeSelector>::equal_range(const K &x) const {
    return _tree.equal_range(std::forward<K>(x));
}
} // namespace j
