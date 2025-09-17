/*
 * @ Created by jaehyung409 on 25. 9. 16..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

module;
#include <utility>
#include <memory>
#include <functional>
#include <random>

export module j:skip_list;

import :traits;
import :vector;

namespace j {
export template <class Value, class Compare, class Allocator>
class skip_list : private value_compare_traits<
                     typename key_traits<Value>::key_type,
                     Value,
                     Compare,
                     key_traits<Value>>::value_compare {
public:
    using value_type = Value;
    using key_type   = typename key_traits<Value>::key_type;
    using mapped_type = typename key_traits<Value>::mapped_type; // for map, multimap
    using key_compare = Compare;
    using value_compare = typename value_compare_traits<key_type, value_type, Compare, key_traits<Value>>::value_compare; // EBO
    using allocator_type         = Allocator;
    using pointer                = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type = typename std::allocator_traits<Allocator>::size_type;
    using difference_type = typename std::allocator_traits<Allocator>::difference_type;
    using iterator = /* not yet */
    using const_iterator = /* not yet */
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    struct node_type;
    using insert_return_type     = std::pair<iterator, node_type>; // for set, map

    skip_list() : skip_list(Compare()) {}
    explicit skip_list(const Compare& comp, const Allocator& alloc = Allocator());
    template<class InputIter>
    requires std::input_iterator<InputIter>
    skip_list(InputIter first, InputIter last, const Compare& comp = Compare(), const Allocator& alloc = Allocator());
    skip_list(const skip_list& other);
    skip_list(const skip_list& other, const Allocator& alloc);
    skip_list(skip_list&& other);
    skip_list(skip_list&& other, const Allocator& alloc);
    skip_list(std::initializer_list<value_type> il, const Compare& comp = Compare(), const Allocator& alloc = Allocator());
    skip_list& operator=(const skip_list& x);
    skip_list& operator=(skip_list&& x);
    skip_list& operator=(std::initializer_list<value_type> il);
    ~skip_list();
    allocator_type get_allocator() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;


    auto lower_bound(const key_type& key) const;
    template<class... Args>
    auto emplace(const_iterator position, Args&&... args);
    auto erase(const_iterator position);
    auto insert(const iterator position, node_type node);
    auto extract(const_iterator position);
    void clear();
    void swap(skip_list& other) noexcept;

private:
    struct _skip_list_node;
    using Node = _skip_list_node;
    using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    mutable std::mt19937 rng;
    mutable std::bernoulli_distribution coin_flip{0.5};

    static const size_type MAX_LEVEL = 32;
    Node* _header; // header node
    Node* _tail;   // tail node
    node_allocator_type _node_alloc;
    size_type _size;

    size_type _random_level() const {
        size_type level = 0;
        while (coin_flip(rng)) {
            ++level;
        }
        return level;
    }
    void _find_predecessors(const key_type& key, vector<Node*>& predecessors) const; // 아직 구조를 확정 못함.
};

// non-member functions will be added later

template <class Value, class Compare, class Allocator>
struct skip_list<Value, Compare, Allocator>::_skip_list_node {
    friend skip_list;
    friend iterator;
    friend const_iterator;
private:
    value_type _value;
    size_type _level;
    vector<Node*> _forward; // forward[i] = next node at level i
    Node* _backward;

    explicit _skip_list_node(value_type& value) : _value(value) {
        _level = _random_level();
        _forward = vector<Node*>(_level + 1, nullptr);
        _backward = nullptr;
    }
    ~_skip_list_node() = default; // destructor will be defined later
};
template <class Value, class Compare, class Allocator>
template <bool IsConst>
class skip_list<Value, Compare, Allocator>::_iterator_impl {};

template <class Value, class Compare, class Allocator>
struct skip_list<Value, Compare, Allocator>::node_type {};
}

namespace j {

}