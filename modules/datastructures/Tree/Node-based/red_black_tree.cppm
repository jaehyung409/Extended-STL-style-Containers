/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>
#include <memory>

export module j.red_black_tree;

import j.vector;
import j.basics;

namespace j {
    enum class color { RED, BLACK };

    template <typename Key, bool IsConst, bool IsPairValue>
    struct reference_selector {
        using type = const Key&;
    };

    template <typename Key>
    struct reference_selector<Key, false, true> {
        using type = std::pair< const typename Key::first_type, typename Key::second_type>&;
    };

    template <typename Key>
    struct reference_selector<Key, true, true> {
        using type = const Key&;
    };

    export template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    class red_black_tree {
    private:
        class _red_black_tree_node;
        class _nil_node;
        template <bool IsConst>
        class _iterator_base;

    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = std::size_t;
        using key_compare = Compare;
        struct value_compare;
        using allocator_type = Allocator;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using difference_type = std::ptrdiff_t;
        using iterator = _iterator_base<false>;
        using const_iterator = _iterator_base<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        class node_type;
        using insert_return_type = std::pair<iterator, node_type>;

    private:
        using Node = _red_black_tree_node;
        using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node *_root;
        Node *_nil;// _nil is a dummy node (sentinel), using Nil struct
        size_type _size;

        key_compare _comp;
        node_allocator_type _alloc;

        void _left_rotate(Node* x);
        void _right_rotate(Node* x);
        std::pair<const_iterator, bool> find_insert_position(const_iterator hint, const key_type &x); // helper function
        void _insert_up(Node* position); // helper function
        Node* find_for_min(Node *position); // helper function
        Node* _erase(Node *position); // helper function
        void _transplant(Node* u, Node* v); // helper function
        void _erase_fix(Node* position); // helper function)
        void _delete(Node *node); // helper function

    public:
        red_black_tree() : red_black_tree(Compare()) {}
        explicit red_black_tree(const Compare &comp, const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        red_black_tree(InputIter first,
                 InputIter last,
                 const Compare &comp = Compare(),
                 const Allocator &alloc = Allocator());
        //template <container-compatible-range<Key> R>
        //red_black_tree(from_range_t, R &&range, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
        red_black_tree(const red_black_tree &x);
        red_black_tree(red_black_tree &&x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
        explicit red_black_tree(const Allocator &alloc);
        red_black_tree(const red_black_tree &x, const Allocator &alloc);
        red_black_tree(red_black_tree &&x, const Allocator &alloc);
        red_black_tree(std::initializer_list<value_type> il,
                 const Compare &comp = Compare(),
                 const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        red_black_tree(InputIter first, InputIter last, const Allocator &alloc);
        //template <container-compatible-range<Key> R>
        //red_black_tree(from_range_t, R &&range, const Allocator &alloc)
        //    : red_black_tree(from_range, std::forward<R>(range), Compare(), alloc) {}
        red_black_tree(std::initializer_list<value_type> il, const Allocator &alloc);
        ~red_black_tree();
        red_black_tree &operator=(const red_black_tree &x);
        red_black_tree &operator=(red_black_tree &&x) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value &&
            std::is_nothrow_assignable_v<Compare, Compare>);
        red_black_tree &operator=(std::initializer_list<value_type> il);
        allocator_type get_allocator() const noexcept;

        // Iterators
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
        const_iterator crbegin() const noexcept;
        const_iterator crend() const noexcept;

        // Capacity
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

        // Modifiers
        template <class... Args>
        iterator emplace(Args &&... args);
        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args &&... args);
        iterator insert(const value_type &x);
        iterator insert(value_type &&x);
        template <class K>
        iterator insert(K &&x);
        iterator insert(const_iterator hint, const value_type &x);
        iterator insert(const_iterator hint, value_type &&x);
        template <class K>
        iterator insert(const_iterator hint, K &&x);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert(InputIter first, InputIter last);
        //template <container-compatible-range<Key> R>
        //void insert_range(R &&rg);
        void insert(std::initializer_list<value_type> il);

        template <class... Args>
        std::pair<iterator, bool> emplace_unique(Args &&... args);
        template <class... Args>
        iterator emplace_hint_unique(const_iterator hint, Args &&... args);
        std::pair<iterator, bool> insert_unique(const value_type &x);
        std::pair<iterator, bool> insert_unique(value_type &&x);
        template <class K>
        std::pair<iterator, bool> insert_unique(K &&x);
        iterator insert_unique(const_iterator hint, const value_type &x);
        iterator insert_unique(const_iterator hint, value_type &&x);
        template <class K>
        iterator insert_unique(const_iterator hint, K &&x);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert_unique(InputIter first, InputIter last);
        //template <container-compatible-range<Key> R>
        //void insert_range(R &&rg);
        void insert_unique(std::initializer_list<value_type> il);

        node_type extract(const_iterator position);
        node_type extract(const key_type &x);
        template <class K>
        node_type extract(K &&x);
        insert_return_type insert(node_type &&node);
        insert_return_type insert(const_iterator position, node_type &&node);
        insert_return_type insert_unique(node_type &&node);
        insert_return_type insert_unique(const_iterator position, node_type &&node);

        iterator erase(iterator position)
            requires (!std::is_same_v<iterator, const_iterator>);
        iterator erase(const_iterator position);
        size_type erase(const key_type &x);
        template <class K>
        size_type erase(K &&x);
        iterator erase(const_iterator first, const_iterator last);
        void swap(red_black_tree &x) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value &&
            std::is_nothrow_assignable_v<Compare, Compare>);
        void unique();
        void clear() noexcept;

        template <class C2>
        void merge(red_black_tree<key_type, C2, allocator_type> &source);
        template <class C2>
        void merge(red_black_tree<key_type, C2, allocator_type> &&source);
        template <class C2>
        void unique_merge(red_black_tree<key_type, C2, allocator_type> &source);
        template <class C2>
        void unique_merge(red_black_tree<key_type, C2, allocator_type> &&source);

        // Observers
        key_compare key_comp() const;
        value_compare value_comp() const;

        // Operations
        iterator find(const key_type &x);
        const_iterator find(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        iterator find(const K &x);
        template <class K>
        requires std::convertible_to<K, Key>
        const_iterator find(const K &x) const;

        size_type count(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        size_type count(const K &x) const;

        bool contains(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        bool contains(const K &x) const;

        iterator lower_bound(const key_type &x);
        const_iterator lower_bound(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        iterator lower_bound(const K &x);
        template <class K>
        requires std::convertible_to<K, Key>
        const_iterator lower_bound(const K &x) const;

        iterator upper_bound(const key_type &x);
        const_iterator upper_bound(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        iterator upper_bound(const K &x);
        template <class K>
        requires std::convertible_to<K, Key>
        const_iterator upper_bound(const K &x) const;

        std::pair<iterator, iterator> equal_range(const key_type &x);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
        template <class K>
        requires std::convertible_to<K, Key>
        std::pair<iterator, iterator> equal_range(const K &x);
        template <class K>
        requires std::convertible_to<K, Key>
        std::pair<const_iterator, const_iterator> equal_range(const K &x) const;
    };

    template <class InputIter,
              class Compare = std::less<typename std::iterator_traits<InputIter>::value_type>,
              class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
    red_black_tree(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
        -> red_black_tree<typename std::iterator_traits<InputIter>::value_type, Compare, Allocator>;

    //template <ranges::input_range R,
    //          class Compare = std::less<std::ranges::range_value_t<R>>,
    //          class Allocator = std::allocator<std::ranges::range_value_t<R>>>
    //red_black_tree(from_range_t, R &&, Compare = Compare(), Allocator = Allocator())
    //    -> red_black_tree<std::ranges::range_value_t<R>, Compare, Allocator>;

    template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    red_black_tree(std::initializer_list<Key>, Compare = Compare(), Allocator = Allocator())
        -> red_black_tree<Key, Compare, Allocator>;

    template <class InputIter, class Allocator>
    red_black_tree(InputIter, InputIter, Allocator)
        -> red_black_tree<typename std::iterator_traits<InputIter>::value_type, std::less<>, Allocator>;

    //template <ranges::input_range R, class Allocator>
    //red_black_tree(from_range_t, R &&, Allocator)
    //    -> red_black_tree<std::ranges::range_value_t<R>, std::less<>, Allocator>;

    template <class Key, class Allocator>
    red_black_tree(std::initializer_list<Key>, Allocator)
        -> red_black_tree<Key, std::less<Key>, Allocator>;

    template <class Key, class Compare, class Allocator>
    struct red_black_tree<Key, Compare, Allocator>::value_compare  {
        Compare _comp;
        explicit value_compare(Compare c) : _comp(c) {}

        bool operator()(const value_type &x, const value_type &y) const {
            if constexpr (IsPair<Key>) {
                return _comp(x.first, y.first);
            } else {
                return _comp(x, y);
            }
        }
    };

    template <class Key, class Compare, class Allocator>
    class red_black_tree<Key, Compare, Allocator>::_red_black_tree_node {
    friend class red_black_tree;
    friend class _iterator_base<false>;
    friend class _iterator_base<true>;

    protected:
        using node = _red_black_tree_node;
        key_type _key;
        enum color _color;
        node *_left;
        node *_right;
        node *_parent;

    public:
        _red_black_tree_node() : _key(key_type()), _left(nullptr), _right(nullptr), _parent(nullptr) {}
        _red_black_tree_node(const key_type &key)
            : _key(key), _left(nullptr), _right(nullptr), _parent(nullptr) {}
        _red_black_tree_node(key_type &&key)
            : _key(std::move(key)), _left(nullptr), _right(nullptr), _parent(nullptr) {}
        key_type& operator*() { return _key; }
        const key_type& operator*() const { return _key; }
        ~_red_black_tree_node() {
            _key.~key_type();
            _left = nullptr;
            _right = nullptr;
            _parent = nullptr;
        }
    };

    template <class Key, class Compare, class Allocator>
    class red_black_tree<Key, Compare, Allocator>::_nil_node : public _red_black_tree_node {
    public:
        _nil_node() {
            this->_left = this;
            this->_right = this;
            this->_parent = this;
        }
    };

    template <class Key, class Compare, class Allocator>
    class red_black_tree<Key, Compare, Allocator>::node_type {
        friend class red_black_tree;
    private:
        Node *_ptr;
        node_allocator_type _alloc;;
        explicit node_type(node_allocator_type alloc) : _ptr(nullptr), _alloc(alloc) {}
        explicit node_type(Node *node, node_allocator_type alloc ) : _ptr(node), _alloc(alloc) {}

    public:
        explicit node_type() : _ptr(nullptr), _alloc(Allocator()) {}
        node_type(const node_type&) = delete;
        node_type& operator=(const node_type&) = delete;
        node_type(node_type &&other) noexcept
            : _ptr(std::exchange(other._ptr, nullptr)), _alloc(other._alloc) {}
        node_type& operator=(node_type &&other) noexcept {
            if (this != &other) {
                _ptr = std::exchange(other._ptr, nullptr);
                _alloc = other._alloc;
            }
            return *this;
        }
        ~node_type() {
            if (_ptr) {
                std::destroy_at(_ptr);
                std::allocator_traits<node_allocator_type>::deallocate(_alloc, _ptr, 1);
                _ptr = nullptr;
            }
        } // destructor only node_type is out of tree

        bool empty() const noexcept { return _ptr == nullptr; }
        allocator_type get_allocator() const noexcept {
            using alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Key>;
            return alloc();
        }
        void swap(node_type &other) noexcept { std::swap(_ptr, other._ptr); }
        const auto& key() const {
            if (empty()) throw std::runtime_error("node_type is empty");
            if constexpr (IsPair<Key>) {
                return _ptr->_key.first;
            } else {
                return _ptr->_key;
            }
        }
        const auto& value() const {
            if (empty()) throw std::runtime_error("node_type is empty");
            if constexpr (IsPair<Key>) {
                return _ptr->_key.second;
            } else {
                static_assert(sizeof(Key) == 0, "value() is not available for non-pair key_type");
            }
        }
    };

    template <class Key, class Compare, class Allocator>
    template <bool IsConst>
    class red_black_tree<Key, Compare, Allocator>::_iterator_base {
        friend class red_black_tree;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = _red_black_tree_node*;
        using reference = typename reference_selector<Key, IsConst, IsPair<Key>>::type;

    protected:
        pointer _ptr;
        red_black_tree *_tree;
        explicit _iterator_base(pointer ptr = nullptr, red_black_tree *tree = nullptr) : _ptr(ptr), _tree(tree) {}

    public:
        explicit _iterator_base() : _ptr(nullptr), _tree(nullptr) {}
        template <bool OtherConst, typename = std::enable_if_t<!OtherConst && IsConst>>
        _iterator_base(const _iterator_base<OtherConst>& other) : _ptr(other._ptr), _tree(other._tree) {}
        _iterator_base(const _iterator_base& other) : _ptr(other._ptr), _tree(other._tree) {}
        _iterator_base& operator=(const _iterator_base& other) {
            _ptr = other._ptr;
            _tree = other._tree;
            return *this;
        }

        reference operator*() const { return **_ptr; }
        pointer operator->() const { return &**_ptr; }

        _iterator_base &operator++() {
            if (_ptr->_right != nullptr) {
                _ptr = _ptr->_right;
                if (_ptr == _tree->_nil) {
                    return *this;
                }
                while (_ptr->_left != nullptr) {
                    _ptr = _ptr->_left;
                }
            } else {
                pointer parent = _ptr->_parent;
                while (parent != _tree->_nil && _ptr == parent->_right) {
                    _ptr = parent;
                    parent = parent->_parent;
                }
                _ptr = parent;
            }
            return *this;
        }

        _iterator_base operator++(int) {
            _iterator_base temp = *this;
            ++(*this);
            return temp;
        }

        _iterator_base &operator--() {
            if (_ptr->_left != nullptr) {
                _ptr = _ptr->_left;
                while (_ptr->_right != _tree->_nil || _ptr->_right != nullptr) {
                    _ptr = _ptr->_right;
                }
            } else {
                pointer parent = _ptr->_parent;
                while (parent != _tree->_nil && _ptr == parent->_left) {
                    _ptr = parent;
                    parent = parent->_parent;
                }
                _ptr = parent;
            }
            return *this;
        }

        _iterator_base operator--(int) {
            _iterator_base temp = *this;
            --(*this);
            return temp;
        }

        template <bool OtherConst>
        bool operator==(const _iterator_base<OtherConst> &other) const { return _ptr == other._ptr; }
        template <bool OtherConst>
        bool operator!=(const _iterator_base<OtherConst> &other) const { return _ptr != other._ptr; }
    };
}

namespace j {
    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const Compare& comp, const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    red_black_tree<Key, Compare, Allocator>::red_black_tree(InputIter first, InputIter last, const Compare& comp,
        const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const red_black_tree& x) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(
        red_black_tree&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const red_black_tree& x, const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(red_black_tree&& x, const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(std::initializer_list<value_type> il, const Compare& comp,
        const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    red_black_tree<Key, Compare, Allocator>::red_black_tree(InputIter first, InputIter last, const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(std::initializer_list<value_type> il,
        const Allocator& alloc) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::~red_black_tree() {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>& red_black_tree<Key, Compare, Allocator>::operator
    =(const red_black_tree& x) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>& red_black_tree<Key, Compare, Allocator>::operator=(
        red_black_tree&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::
        is_nothrow_assignable_v<Compare, Compare>) {
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>& red_black_tree<Key, Compare, Allocator>::operator=(
        std::initializer_list<value_type> il) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::allocator_type red_black_tree<Key, Compare, Allocator>::
    get_allocator() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    begin() noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    begin() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::end() noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    end() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::reverse_iterator red_black_tree<Key, Compare, Allocator>::
    rbegin() noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_reverse_iterator red_black_tree<Key, Compare, Allocator>::
    rbegin() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::reverse_iterator red_black_tree<Key, Compare, Allocator>::
    rend() noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_reverse_iterator red_black_tree<Key, Compare, Allocator>::
    rend() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    cbegin() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    cend() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    crbegin() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    crend() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    bool red_black_tree<Key, Compare, Allocator>::empty() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::
    size() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::
    max_size() const noexcept {
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    emplace(Args&&... args) {
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::emplace_hint(
        const_iterator hint, Args&&... args) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(
        const value_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(
        value_type&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(
        const_iterator hint, const value_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(
        const_iterator hint, value_type&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(
        const_iterator hint, K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void red_black_tree<Key, Compare, Allocator>::insert(InputIter first, InputIter last) {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::insert(std::initializer_list<value_type> il) {
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool> red_black_tree<Key, Compare, Allocator>
    ::emplace_unique(Args&&... args) {
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    emplace_hint_unique(const_iterator hint, Args&&... args) {
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool> red_black_tree<Key, Compare, Allocator>
    ::insert_unique(const value_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool> red_black_tree<Key, Compare, Allocator>
    ::insert_unique(value_type&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool> red_black_tree<Key, Compare, Allocator>
    ::insert_unique(K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert_unique(
        const_iterator hint, const value_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert_unique(
        const_iterator hint, value_type&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert_unique(
        const_iterator hint, K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void red_black_tree<Key, Compare, Allocator>::insert_unique(InputIter first, InputIter last) {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::insert_unique(std::initializer_list<value_type> il) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::node_type red_black_tree<Key, Compare, Allocator>::extract(
        const_iterator position) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::node_type red_black_tree<Key, Compare, Allocator>::extract(
        const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename red_black_tree<Key, Compare, Allocator>::node_type red_black_tree<Key, Compare, Allocator>::
    extract(K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type red_black_tree<Key, Compare, Allocator>::
    insert(node_type&& node) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type red_black_tree<Key, Compare, Allocator>::
    insert(const_iterator position, node_type&& node) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type red_black_tree<Key, Compare, Allocator>::
    insert_unique(node_type&& node) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type red_black_tree<Key, Compare, Allocator>::
    insert_unique(const_iterator position, node_type&& node) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    erase(iterator position) requires (!std::is_same_v<_iterator_base<false>, _iterator_base<true>>) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::erase(
        const_iterator position) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::erase(
        const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::erase(K&& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::erase(
        const_iterator first, const_iterator last) {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::swap(
        red_black_tree& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value && std::
        is_nothrow_assignable_v<Compare, Compare>) {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::unique() {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::clear() noexcept {
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::merge(red_black_tree<key_type, C2, allocator_type>& source) {
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::merge(red_black_tree<key_type, C2, allocator_type>&& source) {
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::unique_merge(red_black_tree<key_type, C2, allocator_type>& source) {
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::unique_merge(red_black_tree<key_type, C2, allocator_type>&& source) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::key_compare red_black_tree<Key, Compare, Allocator>::
    key_comp() const {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::value_compare red_black_tree<Key, Compare, Allocator>::
    value_comp() const {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::find(
        const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::find(
        const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    find(const K& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::find(
        const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::count(
        const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::count(
        const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    bool red_black_tree<Key, Compare, Allocator>::contains(const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    bool red_black_tree<Key, Compare, Allocator>::contains(const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::lower_bound(
        const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    lower_bound(const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    lower_bound(const K& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    lower_bound(const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::upper_bound(
        const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    upper_bound(const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::
    upper_bound(const K& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator red_black_tree<Key, Compare, Allocator>::
    upper_bound(const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, typename red_black_tree<Key, Compare,
    Allocator>::iterator> red_black_tree<Key, Compare, Allocator>::equal_range(const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::const_iterator, typename red_black_tree<Key, Compare,
    Allocator>::const_iterator> red_black_tree<Key, Compare, Allocator>::equal_range(const key_type& x) const {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, typename red_black_tree<Key, Compare,
    Allocator>::iterator> red_black_tree<Key, Compare, Allocator>::equal_range(const K& x) {
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::const_iterator, typename red_black_tree<Key, Compare,
    Allocator>::const_iterator> red_black_tree<Key, Compare, Allocator>::equal_range(const K& x) const {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_left_rotate(Node* x) {
        Node* right = x->_right;
        x->_right = right->_left;
        if (right->_left != _nil) {
            right->_left->_parent = x;
        }
        right->_parent = x->_parent;
        if (x->_parent == _nil) {
            _root = right;
        } else if (x == x->_parent->_left) {
            x->_parent->_left = right;
        } else {
            x->_parent->_right = right;
        }
        right->_left = x;
        x->_parent = right;
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_right_rotate(Node* x) {
        Node* left = x->_left;
        x->_left = left->_right;
        if (left->right != _nil) {
            left->_right->_parent = x;
        }
        left->_parent = x->_parent;
        if (x->_parent == _nil) {
            _root = left;
        } else if (x == x->_parent->_right) {
            x->_parent->_right = left;
        } else {
            x->_parent->_left = left;
        }
        left->_right = x;
        x->_parent = left;
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::const_iterator, bool>
    red_black_tree<Key, Compare, Allocator>::find_insert_position(const_iterator hint, const key_type& x) {
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_insert_up(Node* node) {
        while (node->_parent != _nil && node->_parent->_color == color::RED) {
            Node* parent = node->_parent;
            Node* grandparent = parent->_parent;
            if (grandparent == _nil) {
                break;
            }
            Node* uncle = (parent == grandparent->_left) ? grandparent->_right : grandparent->_left;
            auto uncle_color = (uncle && uncle != _nil) ? uncle->_color : color::BLACK;
            if (uncle_color == color::RED) {
                _recolor_insert(node);
                uncle->_color = color::BLACK;
                node = grandparent;
            } else {
                if (_comp(parent->_key, grandparent->_key)) {
                    if (_comp(parent->_key, node->_key)) {
                        _left_rotate(parent);
                        node = parent;
                    }
                    _right_rotate(grandparent);
                } else {
                    if (_comp(node->_key, parent->_key)) {
                        _right_rotate(parent);
                        node = parent;
                    }
                    _left_rotate(grandparent);
                }
                parent->_color = color::BLACK;
                grandparent->_color = color::RED;
            }
        }
        _root->_color = color::BLACK;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::Node*
    red_black_tree<Key, Compare, Allocator>::find_for_min(Node* position) {
        Node* node = position;
        while (node->_left != _nil) {
            node = node->_left;
        }
        return node;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::Node*
    red_black_tree<Key, Compare, Allocator>::_erase(Node* node) {
        Node* erased = node;
        Node* fix = nullptr;
        auto erased_original_color = erased->_color;
        if (node->_left == _nil) {
            fix = node->_right;
            _transplant(node, node->_right);
            if (node == _nil->_left) {
                _nil->_left = fix;
            }
        } else if (node->_right == _nil) {
            fix = node->_left;
            _transplant(node, node->_left);
            if (node == _nil->_parent) {
                _nil->_parent = fix;
            }
        } else {
            erased = find_for_min(node->_right);
            erased_original_color = erased->_color;
            fix = erased->_right;
            if (erased->_parent == node) {
                if (fix) {
                    fix->_parent = erased;  // if fix is nil, it's ok to set parent to nil
                }
            } else {
                _transplant(erased, erased->_right);
                erased->_right = node->_right;
                erased->_right->_parent = erased;
            }
            _transplant(node, erased);
            erased->_left = node->_left;
            erased->_left->_parent = erased;
            erased->_color = node->_color;
        }
        if (erased_original_color == color::BLACK && fix != _nil) {
            _erase_fix(fix);
        }
        return node;
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_transplant(Node* u, Node* v) {
        if (u->_parent == _nil) {
            _root = v;
        } else if (u == u->_parent->_left) {
            u->_parent->_left = v;
        } else {
            u->_parent->_right = v;
        }
        if (v) {
            v->_parent = u->_parent;
        }

    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_erase_fix(Node* position) {
        Node* node = position;
        Node* sibling = nullptr;
        while (node->_parent != _nil && node->_color == color::BLACK) {
            if (node == node->_parent->_left) {
                sibling = node->_parent->_right;
                if (sibling->_color == color::RED) {
                    sibling->_color = color::BLACK;
                    node->_parent->_color = color::RED;
                    _left_rotate(node->_parent);
                    sibling = node->_parent->_right;
                }
                if (sibling != _nil && sibling->_left->_color == color::BLACK &&
                                       sibling->_right->_color == color::BLACK) {
                    sibling->_color = color::RED;
                    node = node->_parent;
                } else {
                    if (sibling != _nil && sibling->_right->_color == color::BLACK) {
                        sibling->_left->_color = color::BLACK;
                        sibling->_color = color::RED;
                        _right_rotate(sibling);
                        sibling = node->_parent->_right;
                    }
                    sibling->_color = node->_parent->_color;
                    node->_parent->_color = color::BLACK;
                    sibling->_right->_color = color::BLACK;
                    _left_rotate(node->_parent);
                    node = _root;
                }
            } else {
                sibling = node->_parent->_left;
                if (sibling->_color == color::RED) {
                    sibling->_color = color::BLACK;
                    node->_parent->_color = color::RED;
                    _right_rotate(node->_parent);
                    sibling = node->_parent->_left;
                }
                if (sibling != _nil && sibling->_right->_color == color::BLACK &&
                                       sibling->_left->_color == color::BLACK) {
                    sibling->_color = color::RED;
                    node = node->_parent;
                } else {
                    if (sibling != _nil && sibling->_left->_color == color::BLACK) {
                        sibling->_right->_color = color::BLACK;
                        sibling->_color = color::RED;
                        _left_rotate(sibling);
                        sibling = node->_parent->_left;
                    }
                    sibling->_color = node->_parent->_color;
                    node->_parent->_color = color::BLACK;
                    sibling->_left->_color = color::BLACK;
                    _right_rotate(node->_parent);
                    node = _root;
                }
            }
        }
        node->_color = color::BLACK;
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_delete(Node* node) {
    }
}
