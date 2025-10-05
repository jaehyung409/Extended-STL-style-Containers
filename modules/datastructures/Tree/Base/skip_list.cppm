/*
 * @ Created by jaehyung409 on 25. 9. 16..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <functional>
#include <memory>
#include <random>
#include <unordered_map>
#include <utility>

#if defined(__clang__)
export module j:skip_list;
#else
module j:skip_list;
#endif

import :concepts;
import :memory;
import :unique_ptr;
import :vector;
import :array;

namespace j {
template <class Traits> class skip_list {
  private:
    class _iterator;
    class _const_iterator;
    static constexpr bool _MULTI = Traits::_MULTI;
    static constexpr bool _IS_SET = std::is_same_v<typename Traits::key_type, typename Traits::value_type>;

  public:
    using value_type = typename Traits::value_type;
    using key_type = typename Traits::key_type;
    using mapped_type = typename Traits::mapped_type;
    using key_compare = typename Traits::key_compare;
    using value_compare = typename Traits::value_compare; //
    using allocator_type = typename Traits::allocator_type;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using iterator = std::conditional_t<_IS_SET, _const_iterator, _iterator>;
    using const_iterator = _const_iterator;
    // using reverse_iterator       = std::reverse_iterator<iterator>;
    // using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    struct node_type;
    struct insert_return_type { // temp
        iterator position;
        bool inserted;
        node_type node;
    };

    skip_list(const key_compare &comp, const allocator_type &alloc);
    skip_list(const skip_list &other);
    skip_list(const skip_list &other, const std::type_identity_t<allocator_type> &alloc);
    skip_list(skip_list &&x);
    skip_list(skip_list &&x, const std::type_identity_t<allocator_type> &alloc);
    skip_list &operator=(const skip_list &x);
    skip_list &operator=(skip_list &&x) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value &&
                                                 std::is_nothrow_move_assignable_v<key_compare>);
    ~skip_list() noexcept;

    iterator begin() noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator cend() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;
    [[nodiscard]] allocator_type get_allocator() const noexcept;

    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    std::pair<iterator, bool> emplace(Args &&...args);
    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    iterator emplace_hint(const_iterator position, Args &&...args);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    void insert(InputIter first, InputIter last);
    node_type extract(const_iterator position);

    // Defined inline because the iterator would prevent matching with a separate declaration/definition.
    template <class K>
        requires(IsTransparentlyComparable<K, key_type, key_compare> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, iterator> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, const_iterator>)
    node_type extract(K &&x) {
        if (auto it = find(std::forward<K>(x)); it != end()) {
            return extract(it);
        }
        return node_type();
    }

    insert_return_type insert(node_type &&nh);
    iterator insert(const_iterator hint, node_type &&nh);

    template <class K, class... Args>
        requires detail::TryEmplaceConstraint<skip_list, K, Args...>
    iterator try_emplace(K &&key, Args &&...args);
    template <class K, class... Args>
        requires detail::TryEmplaceConstraint<skip_list, K, Args...>
    iterator try_emplace(const_iterator position, K &&key, Args &&...args);
    template <class K, class M>
        requires detail::InsertOrAssignConstraint<skip_list, K, M>
    iterator insert_or_assign(K &&key, M &&obj);
    template <class K, class M>
        requires detail::InsertOrAssignConstraint<skip_list, K, M>
    iterator insert_or_assign(const_iterator position, K &&key, M &&obj);

    iterator erase(const_iterator position);

    // Defined inline because the iterator would prevent matching with a separate declaration/definition.
    template <class K>
        requires(IsTransparentlyComparable<K, key_type, key_compare> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, iterator> &&
                 !std::is_convertible_v<std::remove_cvref_t<K>, const_iterator>)
    size_type erase(K &&key) {
        size_type count = 0;
        auto range = equal_range(std::forward<K>(key));
        while (range.first != range.second) {
            range.first = erase(range.first);
            ++count;
        }
        return count;
    }

    iterator erase(const_iterator first, const_iterator last);
    void swap(skip_list &x) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value &&
                                     std::is_nothrow_swappable_v<key_compare>);

    void clear() noexcept;

    template <class SourceTree>
        requires IsMergeable<Traits, SourceTree>
    void merge(SourceTree &source) = delete; // will implement slow path, and heterogeneous merge later
    void merge(skip_list &source);           // fast path only (same key_compare, same tree type)

    [[nodiscard]] key_compare key_comp() const;
    [[nodiscard]] value_compare value_comp() const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    iterator find(K &&key);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const_iterator find(K &&key) const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    size_type count(K &&key) const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    bool contains(K &&key) const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    iterator lower_bound(K &&key);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const_iterator lower_bound(K &&key) const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    iterator upper_bound(K &&key);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const_iterator upper_bound(K &&key) const;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    std::pair<iterator, iterator> equal_range(K &&key);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    std::pair<const_iterator, const_iterator> equal_range(K &&key) const;

  private:
    struct _skip_list_node;
    using Node = _skip_list_node;
    using node_ptr = Node *;
    using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
    mutable std::mt19937 rng;
    mutable std::bernoulli_distribution coin_flip{0.5}; // later, make it customizable

    class node_forward_guard;
    struct _strategy_copy {
        static constexpr bool copy = true;
    };
    struct _strategy_move {
        static constexpr bool copy = false;
    };
    class copy_guard;

    static const size_type MAX_LEVEL = 32;
    size_type _max_level; // update only when inserting a new node with higher level (not decrease)
    node_ptr _dummy;
    node_allocator_type _node_alloc;
    size_type _size;
    [[no_unique_address]] key_compare _key_comp;

    size_type _random_level() const;
    // Calling `construct` with a level-only constructor would complicate safe initialization and could cause UB.
    // Therefore, we treat Node as POD-like and manually initialize `_level` (and '_forward').
    [[nodiscard]] auto _construct_node(size_type level) -> node_forward_guard;
    void _deallocate_dummy_node() noexcept;
    void _deallocate_node(node_ptr node) noexcept;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    auto _find_predecessors(K &&key) const -> array<node_ptr, MAX_LEVEL + 1>;
    void _update_predecessors(const key_type key, array<node_ptr, MAX_LEVEL + 1> &predecessors);
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    bool _is_duplicate(K &&key, node_ptr next) const;
    template <class V>
        requires std::constructible_from<value_type, V>
    auto _init_node(V &&value, size_type level) -> node_forward_guard;
    void _init_dummy();
    void _move_state(skip_list &&x);
    template <class Strategy> void _clone_tree(const skip_list &x);
    void _destroy_tree() noexcept;

    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const_iterator _find_lower_bound(K &&key) const;
    template <class K>
        requires IsTransparentlyComparable<K, key_type, key_compare>
    const_iterator _find_upper_bound(K &&key) const;

    node_ptr _extract_node(const_iterator position);
    void _insert_node(node_ptr new_node, array<node_ptr, MAX_LEVEL + 1> &predecessors) noexcept;

    template <class... Args>
        requires std::constructible_from<value_type, Args &&...>
    std::pair<iterator, bool> _emplace(size_type level, Args &&...args);

    template <class K, class... Args>
        requires detail::TryEmplaceConstraint<skip_list<Traits>, K, Args...>
    iterator _try_emplace(K &&key, size_type level, Args &&...args);

    template <class K, class M>
        requires detail::InsertOrAssignConstraint<skip_list<Traits>, K, M>
    iterator _insert_or_assign(K &&key, M &&obj, size_type level);
};

template <class Traits> struct skip_list<Traits>::_skip_list_node {
    friend skip_list;
    friend _iterator;
    friend _const_iterator;

  private:
    using node_pointer = _skip_list_node *;
    value_type _value;
    size_type _level;
    node_pointer *_forward; // forward[i] = next node at level i
    node_pointer _backward;

    const key_type &_key() const noexcept {
        if constexpr (_IS_SET) {
            return _value; // set
        } else {
            return _value.first; // map
        }
    }

  public:
    ~_skip_list_node() = default;
};

template <class Traits> class skip_list<Traits>::_iterator {
    friend skip_list;
    friend _const_iterator;

  public:
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename skip_list::value_type;
    using difference_type = typename skip_list::difference_type;
    using pointer = typename skip_list::pointer;
    using reference = typename skip_list::reference;

  private:
    using node_pointer = Node *;
    node_pointer _ptr;

  public:
    explicit _iterator(node_pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    _iterator &operator=(const const_iterator &other) noexcept {
        _ptr = other._ptr;
        return *this;
    }

    reference operator*() const noexcept {
        return _ptr->_value;
    }
    pointer operator->() const noexcept {
        return &(_ptr->_value);
    }

    _iterator &operator++() noexcept {
        _ptr = _ptr->_forward[0];
        return *this;
    }

    _iterator operator++(int) noexcept {
        _iterator temp = *this;
        ++(*this);
        return temp;
    }

    _iterator &operator--() noexcept {
        _ptr = _ptr->_backward;
        return *this;
    }

    _iterator operator--(int) noexcept {
        _iterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const _iterator &other) const noexcept {
        return _ptr == other._ptr;
    }

    friend bool operator==(const _iterator &lhs, const _const_iterator &rhs) noexcept {
        return lhs._ptr == rhs._ptr;
    }
};

template <class Traits> class skip_list<Traits>::_const_iterator {
    friend skip_list;
    friend _iterator;

  public:
    using iterator_concept = std::bidirectional_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename skip_list::value_type;
    using difference_type = typename skip_list::difference_type;
    using pointer = typename skip_list::pointer;
    using reference = typename skip_list::reference;

  private:
    using node_pointer = Node *;
    node_pointer _ptr;

  public:
    explicit _const_iterator(node_pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    _const_iterator(const _iterator &other) noexcept : _ptr(other._ptr) {}
    _const_iterator &operator=(const _const_iterator &other) noexcept {
        _ptr = other._ptr;
        return *this;
    }

    const_reference operator*() const noexcept {
        return _ptr->_value;
    }
    const_pointer operator->() const noexcept {
        return &(_ptr->_value);
    }

    _const_iterator &operator++() noexcept {
        _ptr = _ptr->_forward[0];
        return *this;
    }

    _const_iterator operator++(int) noexcept {
        _const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    _const_iterator &operator--() noexcept {
        _ptr = _ptr->_backward;
        return *this;
    }

    _const_iterator operator--(int) noexcept {
        _const_iterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const _const_iterator &other) const noexcept {
        return _ptr == other._ptr;
    }

    template <class T>
    friend bool operator==(const skip_list<T>::_iterator &lhs, const skip_list<T>::_const_iterator &rhs) noexcept;
};

template <class Traits>
struct skip_list<Traits>::node_type { // node hanlder 구현이
                                      // 잘못됨.https://en.cppreference.com/w/cpp/container/node_handle.html
    friend skip_list;

  public:
    using allocator_type = typename skip_list::allocator_type;
    using key_type = typename skip_list::key_type;
    using mapped_type = typename skip_list::mapped_type;
    using value_type = typename skip_list::value_type;

  private:
    using Node = _skip_list_node;
    using node_ptr = Node *;
    node_ptr _ptr;
    allocator_type _alloc;
    size_type _level; // level of the node, used for deallocation
    static constexpr bool _IS_SET = std::is_same_v<key_type, value_type>;
    void _reset() {
        if (_ptr) {
            using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
            node_allocator_type _node_alloc(_alloc);
            if (_ptr->_forward) {
                using node_ptr_allocator =
                    typename std::allocator_traits<node_allocator_type>::template rebind_alloc<node_ptr>;
                node_ptr_allocator _node_ptr_alloc(_node_alloc);
                std::allocator_traits<node_ptr_allocator>::deallocate(_node_ptr_alloc, _ptr->_forward, _level + 1);
                _ptr->_forward = nullptr;
            }
            std::allocator_traits<node_allocator_type>::destroy(_node_alloc, &_ptr->_value);
            std::allocator_traits<node_allocator_type>::deallocate(_node_alloc, _ptr, 1);
            _ptr = nullptr;
        }
        _level = 0;
    }

  public:
    explicit node_type() : _ptr(nullptr), _alloc(allocator_type()), _level(0) {}
    explicit node_type(node_ptr ptr, const allocator_type &alloc) : _ptr(ptr), _alloc(alloc) {
        _level = ptr ? ptr->_level : 0;
    }
    node_type(const node_type &) = delete;
    node_type &operator=(const node_type &) = delete;
    node_type(node_type &&other) noexcept
        : _ptr(std::exchange(other._ptr, nullptr)), _alloc(other._alloc), _level(other._level) {}
    node_type &operator=(node_type &&other) noexcept {
        if (this != &other) {
            _reset();
            _ptr = std::exchange(other._ptr, nullptr);
            _alloc = std::move(other._alloc);
            _level = std::exchange(other._level, 0);
            other._level = 0;
        }
        return *this;
    }
    ~node_type() {
        _reset();
    } // destructor only node_type is out of tree

    bool empty() const noexcept {
        return _ptr == nullptr;
    }

    allocator_type get_allocator() const noexcept {
        return _alloc;
    }

    void swap(node_type &other) noexcept {
        using std::swap;
        swap(_ptr, other._ptr);
        swap(_alloc, other._alloc);
        swap(_level, other._level);
    }

    const key_type &key() const
        requires(!_IS_SET)
    {
        return _ptr->_value.first;
    }

    auto &mapped() const
        requires(!_IS_SET)
    { // to avoid compile error in set/multi_set
        return _ptr->_value.second;
    }

    value_type &value() const
        requires(_IS_SET)
    {
        return _ptr->_value;
    }
};

template <class Traits> class skip_list<Traits>::node_forward_guard {
  private:
    node_ptr _ptr;
    node_allocator_type _node_alloc;
    size_type _level;

  public:
    node_forward_guard() : _ptr(nullptr), _level(0) {}
    node_forward_guard(node_ptr node_ptr, const node_allocator_type &node_alloc, size_type level)
        : _ptr(node_ptr), _node_alloc(node_alloc), _level(level) {
        _ptr->_forward = nullptr;
        _ptr->_level = level;
    }
    ~node_forward_guard() {
        if (_ptr) {
            if (_ptr->_forward) {
                using node_ptr_allocator =
                    typename std::allocator_traits<node_allocator_type>::template rebind_alloc<node_ptr>;
                node_ptr_allocator _node_ptr_alloc(_node_alloc);
                std::allocator_traits<node_ptr_allocator>::deallocate(_node_ptr_alloc, _ptr->_forward, _level + 1);
                _ptr->_forward = nullptr;
            }
            std::allocator_traits<node_allocator_type>::destroy(_node_alloc, _ptr);
            std::allocator_traits<node_allocator_type>::deallocate(_node_alloc, _ptr, 1);
            _ptr = nullptr;
        }
    }
    node_forward_guard(const node_forward_guard &) = delete;
    node_forward_guard &operator=(const node_forward_guard &) = delete;
    node_forward_guard(node_forward_guard &&other) noexcept : _ptr(other._ptr), _node_alloc(other._node_alloc) {
        other._ptr = nullptr;
    }
    // move assignment should not be used
    node_forward_guard &operator=(node_forward_guard &&) = delete;

    node_ptr release() noexcept {
        node_ptr temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    node_ptr get() const noexcept {
        return _ptr;
    }
};

template <class Traits> class skip_list<Traits>::copy_guard {
  private:
    std::unordered_map<node_ptr, node_ptr> _node_map; // later, use custom hash function
    node_allocator_type _node_alloc;

  public:
    explicit copy_guard(const node_allocator_type &node_alloc) : _node_alloc(node_alloc) {};
    copy_guard(const copy_guard &) = delete;
    copy_guard &operator=(const copy_guard &) = delete;
    copy_guard(copy_guard &&) = delete;
    copy_guard &operator=(copy_guard &&) = delete;
    ~copy_guard() {
        for (auto &[old_node, new_node] : _node_map) {
            if (new_node) {
                std::allocator_traits<node_allocator_type>::destroy(_node_alloc, &new_node->_value);
                std::allocator_traits<node_allocator_type>::destroy(_node_alloc, new_node);
                std::allocator_traits<node_allocator_type>::deallocate(_node_alloc, new_node, 1);
                new_node = nullptr;
            }
        }
    }

    void insert(node_ptr old_node, node_ptr new_node) {
        _node_map[old_node] = new_node;
    }

    node_ptr get_new_node(node_ptr old_node) const {
        auto it = _node_map.find(old_node);
        return (it != _node_map.end()) ? it->second : nullptr;
    }

    void release() {
        _node_map.clear();
    }
};

} // namespace j

namespace j {
template <class Traits> skip_list<Traits>::size_type skip_list<Traits>::_random_level() const {
    size_type level = 0;
    while (coin_flip(rng) && level <= MAX_LEVEL) {
        ++level;
    }
    return level;
}

template <class Traits> auto skip_list<Traits>::_construct_node(size_type level) -> node_forward_guard {
    node_forward_guard node_guard(std::allocator_traits<node_allocator_type>::allocate(_node_alloc, 1), _node_alloc,
                                  level);

    using node_ptr_allocator = typename std::allocator_traits<node_allocator_type>::template rebind_alloc<node_ptr>;
    node_ptr_allocator _node_ptr_alloc(_node_alloc);
    node_guard.get()->_forward = std::allocator_traits<node_ptr_allocator>::allocate(_node_ptr_alloc, level + 1);

    return node_guard;
}

template <class Traits> void skip_list<Traits>::_deallocate_dummy_node() noexcept {
    using node_ptr_allocator = typename std::allocator_traits<node_allocator_type>::template rebind_alloc<node_ptr>;
    node_ptr_allocator _node_ptr_alloc(_node_alloc);
    std::allocator_traits<node_ptr_allocator>::deallocate(_node_ptr_alloc, _dummy->_forward, _dummy->_level + 1);
    std::allocator_traits<node_allocator_type>::deallocate(_node_alloc, _dummy, 1);
}

template <class Traits> void skip_list<Traits>::_deallocate_node(Node *node) noexcept {
    std::allocator_traits<node_allocator_type>::destroy(_node_alloc, &node->_value);
    using node_ptr_allocator = typename std::allocator_traits<node_allocator_type>::template rebind_alloc<node_ptr>;
    node_ptr_allocator _node_ptr_alloc(_node_alloc);
    std::allocator_traits<node_ptr_allocator>::deallocate(_node_ptr_alloc, node->_forward, node->_level + 1);
    std::allocator_traits<node_allocator_type>::deallocate(_node_alloc, node, 1);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
auto skip_list<Traits>::_find_predecessors(K &&key) const -> array<node_ptr, MAX_LEVEL + 1> {
    array<node_ptr, MAX_LEVEL + 1> predecessors;
    Node *current = _dummy;
    for (size_type i = _max_level + 1; i > 0; --i) { // avoid unsigned int underflow
        while (current->_forward[i - 1] != _dummy && _key_comp(current->_forward[i - 1]->_key(), key)) {
            current = current->_forward[i - 1];
        }
        predecessors[i - 1] = current;
    }
    return predecessors;
}

// If we already know the predecessors and want to insert a new node after them,
template <class Traits>
void skip_list<Traits>::_update_predecessors(const key_type key, array<Node *, MAX_LEVEL + 1> &predecessors) {
    for (size_type i = _max_level + 1; i > 0; --i) {
        while (predecessors[i - 1]->_forward[i - 1] != _dummy &&
               !_key_comp(key, predecessors[i - 1]->_forward[i - 1]->_key())) {
            predecessors[i - 1] = predecessors[i - 1]->_forward[i - 1];
        }
    }
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
bool skip_list<Traits>::_is_duplicate(K &&key, Node *next) const {
    return next != _dummy && !_key_comp(key, next->_key()) && !_key_comp(next->_key(), key);
}

template <class Traits>
template <class V>
    requires std::constructible_from<typename Traits::value_type, V>
auto skip_list<Traits>::_init_node(V &&value, size_type level) -> node_forward_guard {
    node_forward_guard node_guard(std::move(_construct_node(level)));
    allocator_type _alloc = get_allocator();
    std::allocator_traits<allocator_type>::construct(_alloc, &node_guard.get()->_value, std::forward<V>(value));

    return node_guard;
}

template <class Traits> void skip_list<Traits>::_init_dummy() {
    node_forward_guard head_guard(std::move(_construct_node(MAX_LEVEL)));
    for (size_type i = 0; i <= MAX_LEVEL; ++i) {
        head_guard.get()->_forward[i] = head_guard.get();
    }
    head_guard.get()->_backward = head_guard.get();
    _dummy = head_guard.release();
}

template <class Traits>
void skip_list<Traits>::_move_state(skip_list &&x) { // pre-require: _dummy is initialized (call _init_dummy())
    using std::swap;
    swap(_dummy, x._dummy);
    swap(_max_level, x._max_level);
    swap(_size, x._size);
}

template <class Traits> template <class Strategy> void skip_list<Traits>::_clone_tree(const skip_list &other) {
    if (other.empty()) {
        _init_dummy();
        _max_level = 0;
        _size = 0;
        return;
    }

    copy_guard guard(_node_alloc);
    node_forward_guard init_guard(std::move(_construct_node(MAX_LEVEL)));
    guard.insert(other._dummy, init_guard.get());
    init_guard.release();

    node_ptr current_other = other._dummy->_forward[0];
    while (current_other != other._dummy) {
        if constexpr (Strategy::copy) {
            node_forward_guard new_guard(std::move(_init_node(current_other->_value, current_other->_level)));
            guard.insert(current_other, new_guard.get());
            new_guard.release();
        } else { // Strategy::move
            node_forward_guard new_guard(
                std::move(_init_node(std::move(const_cast<Node &>(*current_other)._value), current_other->_level)));
            guard.insert(current_other, new_guard.get());
            new_guard.release();
        }
        current_other = current_other->_forward[0];
    }
    // current_other is now other._dummy
    do {
        node_ptr new_node = guard.get_new_node(current_other);

        for (size_type i = 0; i <= current_other->_level; ++i) {
            new_node->_forward[i] = guard.get_new_node(current_other->_forward[i]);
        }
        new_node->_backward = guard.get_new_node(current_other->_backward);

        current_other = current_other->_forward[0];
    } while (current_other != other._dummy);

    _dummy = guard.get_new_node(other._dummy);
    guard.release();
    _max_level = other._max_level;
    _size = other._size;
}

template <class Traits> void skip_list<Traits>::_destroy_tree() noexcept {
    node_ptr current = _dummy->_forward[0];
    while (current != _dummy) {
        node_ptr next = current->_forward[0];
        _deallocate_node(current);
        current = next;
    }
    _deallocate_dummy_node();
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::const_iterator skip_list<Traits>::_find_lower_bound(K &&key) const {
    Node *current = _dummy;
    for (size_type i = _max_level + 1; i > 0; --i) {
        while (current->_forward[i - 1] != _dummy && _key_comp(current->_forward[i - 1]->_key(), key)) {
            current = current->_forward[i - 1];
        }
    }
    return const_iterator(current->_forward[0]);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::const_iterator skip_list<Traits>::_find_upper_bound(K &&key) const {
    Node *current = _dummy;
    for (size_type i = _max_level + 1; i > 0; --i) {
        while (current->_forward[i - 1] != _dummy && !_key_comp(key, current->_forward[i - 1]->_key())) {
            current = current->_forward[i - 1];
        }
    }
    return const_iterator(current->_forward[0]);
}

template <class Traits> skip_list<Traits>::node_ptr skip_list<Traits>::_extract_node(const_iterator position) {
    auto predecessors = _find_predecessors(position._ptr->_key());
    for (size_type i = 0; i <= position._ptr->_level; ++i) {
        predecessors[i]->_forward[i] = position._ptr->_forward[i];
    }
    position._ptr->_forward[0]->_backward = position._ptr->_backward;
    --_size;
    return position._ptr;
}

template <class Traits>
void skip_list<Traits>::_insert_node(Node *new_node, array<node_ptr, MAX_LEVEL + 1> &predecessors) noexcept {
    if (new_node->_level > _max_level) {
        std::fill(predecessors.begin() + _max_level + 1, predecessors.begin() + new_node->_level + 1, _dummy);
        _max_level = new_node->_level;
    }

    for (size_type i = 0; i <= new_node->_level; ++i) {
        new_node->_forward[i] = predecessors[i]->_forward[i];
        predecessors[i]->_forward[i] = new_node;
    }
    new_node->_backward = predecessors[0];

    new_node->_forward[0]->_backward = new_node;
    ++_size;
}

template <class Traits>
template <class... Args>
    requires std::constructible_from<typename Traits::value_type, Args &&...>
std::pair<typename skip_list<Traits>::iterator, bool> skip_list<Traits>::_emplace(size_type level, Args &&...args) {
    value_type val(std::forward<Args>(args)...);
    key_type key;
    if constexpr (_IS_SET) {
        key = val;
    } else {
        key = val.first;
    }
    auto predecessors = _find_predecessors(key);
    if constexpr (!_MULTI) {
        auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
        if (_is_duplicate(key, dup_check)) {
            return {iterator(dup_check), false};
        }
    }
    node_forward_guard new_node_guard(std::move(_init_node(std::move(val), level)));
    _insert_node(new_node_guard.get(), predecessors);
    return {iterator(new_node_guard.release()), true};
}

template <class Traits>
template <class K, class... Args>
    requires detail::TryEmplaceConstraint<skip_list<Traits>, K, Args...>
skip_list<Traits>::iterator skip_list<Traits>::_try_emplace(K &&key, size_type level, Args &&...args) {
    auto predecessors = _find_predecessors(key);

    auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
    if (_is_duplicate(key, dup_check)) {
        return iterator(dup_check);
    }

    node_forward_guard new_node_guard(
        std::move(_init_node(std::piecewise_construct, std::forward_as_tuple(std::forward<K>(key)),
                             std::forward_as_tuple(std::forward<Args>(args)...), level)));
    _insert_node(new_node_guard.get(), predecessors);
    return iterator(new_node_guard.release());
}

template <class Traits>
template <class K, class M>
    requires detail::InsertOrAssignConstraint<skip_list<Traits>, K, M>
skip_list<Traits>::iterator skip_list<Traits>::_insert_or_assign(K &&key, M &&obj, size_type level) {
    auto predecessors = _find_predecessors(key);
    auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
    if (_is_duplicate(key, dup_check)) {
        if constexpr (!std::is_const_v<typename std::remove_reference<M>::type>) {
            dup_check->_value.second = std::forward<M>(obj);
        } else {
            dup_check->_value.second = obj;
        }
        return iterator(dup_check);
    }
    node_forward_guard new_node_guard(
        std::move(_init_node(std::piecewise_construct, std::forward_as_tuple(std::forward<K>(key)),
                             std::forward_as_tuple(std::forward<M>(obj)), level)));
    _insert_node(new_node_guard.get(), predecessors);
    return iterator(new_node_guard.release());
}

template <class Traits>
skip_list<Traits>::skip_list(const key_compare &comp, const allocator_type &alloc)
    : _node_alloc(alloc), _key_comp(comp) {
    rng.seed(std::random_device{}());
    _init_dummy();
    _max_level = 0;
    _size = 0;
}

template <class Traits>
skip_list<Traits>::skip_list(const skip_list &other)
    : _node_alloc(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other._node_alloc)),
      _key_comp(other._key_comp) {
    _clone_tree<_strategy_copy>(other);
}

template <class Traits>
skip_list<Traits>::skip_list(const skip_list &other, const std::type_identity_t<allocator_type> &alloc)
    : _node_alloc(alloc), _key_comp(other._key_comp) {
    _clone_tree<_strategy_copy>(other);
}

template <class Traits>
skip_list<Traits>::skip_list(skip_list &&x)
    : rng(std::move(x.rng)), coin_flip(std::move(x.coin_flip)), _max_level(0), _node_alloc(std::move(x._node_alloc)),
      _size(0), _key_comp(std::move(x._key_comp)) {
    _init_dummy();
    _move_state(std::move(x));
}

template <class Traits>
skip_list<Traits>::skip_list(skip_list &&x, const std::type_identity_t<allocator_type> &alloc)
    : rng(std::move(x.rng)), coin_flip(std::move(x.coin_flip)), _max_level(0), _node_alloc(std::move(x._node_alloc)),
      _size(0), _key_comp(std::move(x._key_comp)) {
    if constexpr (!std::allocator_traits<allocator_type>::is_always_equal::value) {
        if (_node_alloc != x._node_alloc) {
            _clone_tree<_strategy_move>(x);
            x.clear();
            return;
        }
    }
    _init_dummy();
    _move_state(x);
}

template <class Traits> skip_list<Traits> &skip_list<Traits>::operator=(const skip_list &x) {
    if (this == std::addressof(x)) {
        return *this;
    }

    clear();
    _key_comp = x._key_comp;
    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
        if (this->_node_alloc != x._node_alloc) {
            this->_node_alloc = x._node_alloc;
        }
    }
    _clone_tree<_strategy_copy>(x);

    return *this;
}

template <class Traits>
skip_list<Traits> &
skip_list<Traits>::operator=(skip_list &&x) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value &&
                                                     std::is_nothrow_move_assignable_v<key_compare>) {
    if (this == std::addressof(x)) {
        return *this;
    }

    if constexpr (!std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value &&
                  !std::allocator_traits<allocator_type>::is_always_equal::value) { // POCMA
        if (_node_alloc != x._node_alloc) {
            clear();
            _key_comp = std::move(x._key_comp);
            _clone_tree<_strategy_move>(x);
            x.clear();
            return *this;
        }
    }
    clear();
    _key_comp = std::move(x._key_comp);
    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
        if (this->_node_alloc != x._node_alloc) {
            this->_node_alloc = std::move(x._node_alloc);
        }
    }
    _move_state(std::move(x));

    return *this;
}

template <class Traits> skip_list<Traits>::~skip_list() noexcept {
    _destroy_tree();
}

template <class Traits> skip_list<Traits>::iterator skip_list<Traits>::begin() noexcept {
    return iterator(_dummy->_forward[0]);
}

template <class Traits> skip_list<Traits>::const_iterator skip_list<Traits>::cbegin() const noexcept {
    return const_iterator(_dummy->_forward[0]);
}

template <class Traits> skip_list<Traits>::iterator skip_list<Traits>::end() noexcept {
    return iterator(_dummy);
}

template <class Traits> skip_list<Traits>::const_iterator skip_list<Traits>::cend() const noexcept {
    return const_iterator(_dummy);
}

template <class Traits> bool skip_list<Traits>::empty() const noexcept {
    return _size == 0;
}

template <class Traits> skip_list<Traits>::size_type skip_list<Traits>::size() const noexcept {
    return _size;
}

template <class Traits> skip_list<Traits>::size_type skip_list<Traits>::max_size() const noexcept {
    return std::allocator_traits<allocator_type>::max_size(get_allocator());
}

template <class Traits> skip_list<Traits>::allocator_type skip_list<Traits>::get_allocator() const noexcept {
    return allocator_type(_node_alloc);
}

template <class Traits>
template <class... Args>
    requires std::constructible_from<typename Traits::value_type, Args &&...>
std::pair<typename skip_list<Traits>::iterator, bool> skip_list<Traits>::emplace(Args &&...args) {
    size_type new_node_level = _random_level();
    return _emplace(new_node_level, std::forward<Args>(args)...);
}

template <class Traits>
template <class... Args>
    requires std::constructible_from<typename Traits::value_type, Args &&...>
skip_list<Traits>::iterator skip_list<Traits>::emplace_hint(const_iterator position, Args &&...args) {
    value_type val(std::forward<Args>(args)...);
    key_type key;
    if constexpr (_IS_SET) {
        key = val;
    } else {
        key = val.first;
    }
    size_type new_node_level = _random_level();
    if (position != cbegin()) {
        auto prev = std::prev(position);
        if (!_key_comp(key, prev._ptr->_key()) && _key_comp(key, position._ptr->_key()) &&
            prev._ptr->_level >= new_node_level) {
            if (!_MULTI && key == prev._ptr->_key()) {
                return iterator(prev._ptr);
            }
            array<node_ptr, MAX_LEVEL + 1> predecessors;
            std::fill_n(predecessors.begin(), new_node_level + 1, prev._ptr);
            node_forward_guard new_node_guard(std::move(_init_node(std::move(val), new_node_level)));
            _insert_node(new_node_guard.get(), predecessors);
            return iterator(new_node_guard.release());
        }
    }
    return _emplace(new_node_level, std::move(val)).first;
}

template <class Traits>
template <class InputIter>
    requires std::input_iterator<InputIter>
void skip_list<Traits>::insert(InputIter first, InputIter last) {
    if (first == last)
        return;

    array<Node *, MAX_LEVEL + 1> predecessors;
    predecessors.fill(_dummy);

    key_type key;
    for (; first != last; ++first) {
        size_type new_node_level = _random_level();
        if constexpr (_IS_SET) {
            key = *first;
        } else {
            key = first->first;
        }

        if (predecessors[0] == _dummy || !_key_comp(key, predecessors[0]->_key())) {
            _update_predecessors(key, predecessors);
        } else {
            predecessors = _find_predecessors(key);
            std::fill(predecessors.begin() + _max_level + 1, predecessors.end(), _dummy);
        }
        if constexpr (!_MULTI) {
            auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
            if (_is_duplicate(key, dup_check)) {
                continue;
            }
        }
        node_forward_guard new_node_guard(std::move(_init_node(*first, new_node_level)));
        _insert_node(new_node_guard.get(), predecessors);
        new_node_guard.release();
    }
} // Should benchmark.

template <class Traits> skip_list<Traits>::node_type skip_list<Traits>::extract(const_iterator position) {
    return node_type{_extract_node(position), get_allocator()};
}

template <class Traits> skip_list<Traits>::insert_return_type skip_list<Traits>::insert(node_type &&nh) {
    auto predecessors = _find_predecessors(nh._ptr->_key());
    if constexpr (!_MULTI) {
        auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
        if (_is_duplicate(nh._ptr->_key(), dup_check)) {
            return {iterator(dup_check), false, std::move(nh)};
        }
    }
    _insert_node(nh._ptr, predecessors);
    nh._ptr = nullptr;
    return {iterator(predecessors[0]->_forward[0]), true, std::move(nh)};
}

template <class Traits> skip_list<Traits>::iterator skip_list<Traits>::insert(const_iterator position, node_type &&nh) {
    size_type new_node_level = nh._ptr->_level;
    key_type key = nh._ptr->_key();

    if (position != cbegin()) {
        auto prev = std::prev(position);
        if (!_key_comp(key, prev._ptr->_key()) && _key_comp(key, position._ptr->_key()) &&
            prev._ptr->_level >= new_node_level) {
            if (!_MULTI && key == prev._ptr->_key()) {
                return iterator(prev._ptr);
            }
            array<node_ptr, MAX_LEVEL + 1> predecessors;
            _insert_node(nh._ptr, predecessors);
            nh._ptr = nullptr;
            return iterator(predecessors[0]->_forward[0]);
        }
    }
    return insert(std::move(nh)).first;
}

template <class Traits>
template <class K, class... Args>
    requires detail::TryEmplaceConstraint<skip_list<Traits>, K, Args...>
skip_list<Traits>::iterator skip_list<Traits>::try_emplace(K &&key, Args &&...args) {
    size_type new_node_level = _random_level();
    return _try_emplace(std::forward<K>(key), new_node_level, std::forward<Args>(args)...);
}

template <class Traits>
template <class K, class... Args>
    requires detail::TryEmplaceConstraint<skip_list<Traits>, K, Args...>
skip_list<Traits>::iterator skip_list<Traits>::try_emplace(const_iterator position, K &&key, Args &&...args) {
    size_type new_node_level = _random_level();

    if (position != cbegin()) {
        auto prev = std::prev(position);
        if (!_key_comp(key, prev._ptr->_key()) && _key_comp(key, position._ptr->_key()) &&
            prev._ptr->_level >= new_node_level) {
            if (key == prev._ptr->_key()) { // not check _MULTI (try_emplace for map only)
                return iterator(prev._ptr);
            }
            array<node_ptr, MAX_LEVEL + 1> predecessors;
            std::fill(predecessors.begin(), new_node_level + 1, prev._ptr);
            auto new_node_guard(
                std::move(_init_node(std::piecewise_construct, std::forward_as_tuple(std::forward<K>(key)),
                                     std::forward_as_tuple(std::forward<Args>(args)...), new_node_level)));
            _insert_node(new_node_guard.get(), predecessors);
            return iterator(new_node_guard.release());
        }
    }

    return _try_emplace(std::forward<K>(key), new_node_level, std::forward<Args>(args)...);
}

template <class Traits>
template <class K, class M>
    requires detail::InsertOrAssignConstraint<skip_list<Traits>, K, M>
skip_list<Traits>::iterator skip_list<Traits>::insert_or_assign(K &&key, M &&obj) {
    size_type new_node_level = _random_level();
    return _insert_or_assign(std::forward<K>(key), std::forward<M>(obj), new_node_level);
}

template <class Traits>
template <class K, class M>
    requires detail::InsertOrAssignConstraint<skip_list<Traits>, K, M>
skip_list<Traits>::iterator skip_list<Traits>::insert_or_assign(const_iterator position, K &&key, M &&obj) {
    size_type new_node_level = _random_level();
    if (position != cbegin()) {
        auto prev = std::prev(position);
        if (!_key_comp(key, prev._ptr->_key()) && _key_comp(key, position._ptr->_key()) &&
            prev._ptr->_level >= new_node_level) {
            if (key == prev._ptr->_key()) { // not check _MULTI (insert_or_assign for map only)
                if constexpr (!std::is_const_v<typename std::remove_reference<M>::type>) {
                    prev._ptr->_value.second = std::forward<M>(obj);
                } else {
                    prev._ptr->_value.second = obj;
                }
                return iterator(prev._ptr);
            }
        }
    }
    return _insert_or_assign(std::forward<K>(key), std::forward<M>(obj), new_node_level);
}

template <class Traits> skip_list<Traits>::iterator skip_list<Traits>::erase(const_iterator position) {
    auto next_it = iterator(position._ptr->_forward[0]);

    _deallocate_node(_extract_node(position));
    return next_it;
}

template <class Traits>
skip_list<Traits>::iterator skip_list<Traits>::erase(const_iterator first, const_iterator last) {
    while (first != last) {
        first = erase(first);
    }
    return iterator(first._ptr);
}

template <class Traits>
void skip_list<Traits>::swap(skip_list &x) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value &&
                                                    std::is_nothrow_swappable_v<key_compare>) {
    using std::swap;
    if constexpr (std::allocator_traits<typename Traits::allocator_type>::propagate_on_container_swap::value) {
        swap(_node_alloc, x._node_alloc);
    }
    swap(rng, x.rng);
    swap(coin_flip, x.coin_flip);
    swap(_max_level, x._max_level);
    swap(_dummy, x._dummy);
    swap(_size, x._size);
    swap(_key_comp, x._key_comp);
}

template <class Traits> void skip_list<Traits>::clear() noexcept {
    node_ptr current = _dummy->_forward[0];
    while (current != _dummy) {
        node_ptr next = current->_forward[0];
        _deallocate_node(current);
        current = next;
    }
    for (size_type i = 0; i <= _max_level; ++i) {
        _dummy->_forward[i] = _dummy;
    }
    _dummy->_backward = _dummy;
    _max_level = 0;
    _size = 0;
}

// template <class Traits>
// template <class SourceTree>
// requires IsMergeable<Traits, SourceTree>
// void skip_list<Traits>::merge(SourceTree &source) {}

template <class Traits> void skip_list<Traits>::merge(skip_list &source) {
    if (this == std::addressof(source) || source.empty()) {
        return;
    }
    array<Node *, MAX_LEVEL + 1> predecessors;
    predecessors.fill(_dummy);

    auto end = source.end();
    for (auto it = source.begin(); it != end;) {
        auto next_it = std::next(it);
        key_type key = it._ptr->_key();
        size_type level = it._ptr->_level;
        _update_predecessors(key, predecessors);
        if constexpr (!_MULTI) {
            auto dup_check = predecessors[0] == _dummy ? _dummy->_backward : predecessors[0];
            if (_is_duplicate(key, dup_check)) {
                ++it;
                continue;
            }
        }
        _insert_node(source._extract_node(it), predecessors);
        it = next_it;
    }
}

template <class Traits> skip_list<Traits>::key_compare skip_list<Traits>::key_comp() const {
    return _key_comp;
}

template <class Traits> skip_list<Traits>::value_compare skip_list<Traits>::value_comp() const {
    return value_compare(_key_comp);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::iterator skip_list<Traits>::find(K &&key) {
    const_iterator cit = std::as_const(*this).find(std::forward<K>(key));
    return iterator(cit._ptr);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::const_iterator skip_list<Traits>::find(K &&key) const {
    const_iterator it = _find_lower_bound(std::forward<K>(key));
    value_compare value_comp = this->value_comp();
    if (it != cend() && !value_comp(key, *it) && !value_comp(*it, key)) {
        return it;
    }
    return cend();
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::size_type skip_list<Traits>::count(K &&key) const {
    if constexpr (_MULTI) {
        auto range = equal_range(std::forward<K>(key));
        return std::distance(range.first, range.second);
    } else {
        return find(std::forward<K>(key)) != cend() ? 1 : 0;
    }
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
bool skip_list<Traits>::contains(K &&key) const {
    return find(std::forward<K>(key)) != cend();
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::iterator skip_list<Traits>::lower_bound(K &&key) {
    const_iterator cit = _find_lower_bound(std::forward<K>(key));
    return iterator(cit._ptr);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::const_iterator skip_list<Traits>::lower_bound(K &&key) const {
    return _find_lower_bound(std::forward<K>(key));
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::iterator skip_list<Traits>::upper_bound(K &&key) {
    const_iterator cit = _find_upper_bound(std::forward<K>(key));
    return iterator(cit._ptr);
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
skip_list<Traits>::const_iterator skip_list<Traits>::upper_bound(K &&key) const {
    return _find_upper_bound(std::forward<K>(key));
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
std::pair<typename skip_list<Traits>::iterator, typename skip_list<Traits>::iterator>
skip_list<Traits>::equal_range(K &&key) {
    auto lower = _find_lower_bound(std::forward<K>(key));
    auto upper = _find_upper_bound(std::forward<K>(key));
    return {iterator(lower._ptr), iterator(upper._ptr)};
}

template <class Traits>
template <class K>
    requires IsTransparentlyComparable<K, typename Traits::key_type, typename Traits::key_compare>
std::pair<typename skip_list<Traits>::const_iterator, typename skip_list<Traits>::const_iterator>
skip_list<Traits>::equal_range(K &&key) const {
    return {_find_lower_bound(std::forward<K>(key)), _find_upper_bound(std::forward<K>(key))};
}

} // namespace j