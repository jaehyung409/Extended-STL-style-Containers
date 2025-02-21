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
        void _insert_child(Node* node, Node* new_node); // helper function
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
        template <class K> requires std::convertible_to<K, Key>
        iterator insert(K &&x);
        iterator insert(const_iterator hint, const value_type &x);
        iterator insert(const_iterator hint, value_type &&x);
        template <class K> requires std::convertible_to<K, Key>
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
        template <class K> requires std::convertible_to<K, Key>
        std::pair<iterator, bool> insert_unique(K &&x);
        iterator insert_unique(const_iterator hint, const value_type &x);
        iterator insert_unique(const_iterator hint, value_type &&x);
        template <class K> requires std::convertible_to<K, Key>
        iterator insert_unique(const_iterator hint, K &&x);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert_unique(InputIter first, InputIter last);
        //template <container-compatible-range<Key> R>
        //void insert_range(R &&rg);
        void insert_unique(std::initializer_list<value_type> il);

        node_type extract(const_iterator position);
        node_type extract(const key_type &x);
        template <class K> requires std::convertible_to<K, Key>
        node_type extract(K &&x);
        insert_return_type insert(node_type &&node);
        insert_return_type insert(const_iterator position, node_type &&node);
        insert_return_type insert_unique(node_type &&node);
        insert_return_type insert_unique(const_iterator position, node_type &&node);

        iterator erase(iterator position)
            requires (!std::is_same_v<iterator, const_iterator>);
        iterator erase(const_iterator position);
        size_type erase(const key_type &x);
        template <class K> requires std::convertible_to<K, Key>
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
        color _color;
        node *_left;
        node *_right;
        node *_parent;

    public:
        _red_black_tree_node() : _red_black_tree_node(color::RED, nullptr) {}
        _red_black_tree_node(color color, node* node) : _red_black_tree_node(key_type(), color, node) {}
        _red_black_tree_node(const key_type &key, color color, node* node)
            : _key(key), _color(color), _left(node), _right(node), _parent(node) {}
        _red_black_tree_node(key_type &&key, color color, node* node)
            : _key(std::move(key)), _color(color), _left(node), _right(node), _parent(node) {}
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
            if (_ptr->_right != _tree->_nil || _ptr == _tree->_nil->_right) {
                _ptr = _ptr->_right;
                if (_ptr == _tree->_nil) {
                    return *this;
                }
                while (_ptr->_left != _tree->_nil) {
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
            if (_ptr->_left != _tree->_nil) {
                _ptr = _ptr->_left;
                while (_ptr->_right != _tree->_nil) {
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
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const Compare& comp, const Allocator& alloc)
        : _size(0), _comp(comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(_nil, color::BLACK, _nil);
        _root = _nil;
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    red_black_tree<Key, Compare, Allocator>::
    red_black_tree(InputIter first, InputIter last, const Compare& comp, const Allocator& alloc)
        : _comp(comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        _root = _nil;
        std::construct_at(_nil, color::BLACK, _nil);
        insert(first, last);
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const red_black_tree& x) : red_black_tree(x, x._alloc) {}

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(red_black_tree&& x)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value) : red_black_tree(std::move(x), x._alloc) {}

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const Allocator& alloc) : _size(0), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(_nil, color::BLACK, _nil);
        _root = _nil;
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(const red_black_tree& x, const Allocator& alloc)
        : _comp(x._comp), _alloc(_alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(_nil, color::BLACK, _nil);
        _root = _nil;
        auto it = this->begin();
        for (const auto& elem : x) {
            it = this->emplace_hint(elem, it);
        }
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::red_black_tree(red_black_tree&& x, const Allocator& alloc)
        : _root(x._root), _nil(x._nil), _size(x._size), _comp(std::move(x._comp)), _alloc(alloc) {
        x._nil = std::allocator_traits<node_allocator_type>::allocate(x._alloc, 1);
        std::construct_at(x._nil, color::BLACK, x._nil);
        x._root = x._nil;
        x._size = 0;
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::
    red_black_tree(std::initializer_list<value_type> il, const Compare& comp, const Allocator& alloc)
        : red_black_tree(il.begin(), il.end(), comp, alloc) {}

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    red_black_tree<Key, Compare, Allocator>::red_black_tree(InputIter first, InputIter last, const Allocator& alloc)
        : red_black_tree(first, last, Compare(), alloc) {}

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::
    red_black_tree(std::initializer_list<value_type> il, const Allocator& alloc)
        : red_black_tree(il.begin(), il.end(), Compare(), alloc) {}

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>::~red_black_tree() {
        _delete(_root);
        std::destroy_at(_nil);
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, _nil, 1);
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>&
    red_black_tree<Key, Compare, Allocator>::operator=(const red_black_tree& x) {
        if (this != &x) {
            clear();
            _comp = x._comp;
            _alloc = x._alloc;
            _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
            std::construct_at(_nil, color::BLACK, _nil);
            _root = _nil;
            auto it = begin();
            for (const auto& elem : x) {
                it = emplace_hint(elem, it);
            }
        }
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>&
    red_black_tree<Key, Compare, Allocator>::operator=(red_black_tree&& x)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
             std::is_nothrow_assignable_v<Compare, Compare>) {
        if (this != x) {
            clear();
            std::destroy_at(_nil);
            std::allocator_traits<node_allocator_type>::deallocate(_alloc, _nil, 1);
            _root = x._root;
            _nil = x._nil;
            _size = x._size;
            _comp = std::move(x._comp);
            _alloc = x._alloc;
            x._nil = std::allocator_traits<node_allocator_type>::allocate(x._alloc, 1);
            std::construct_at(x._nil, color::BLACK, x._nil);
            x._root = x._nil;
            x._size = 0;
        }
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    red_black_tree<Key, Compare, Allocator>&
    red_black_tree<Key, Compare, Allocator>::operator=(std::initializer_list<value_type> il) {
        clear();
        insert(il);
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::allocator_type
    red_black_tree<Key, Compare, Allocator>::get_allocator() const noexcept {
        using alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Key>;
        return alloc();
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::begin() noexcept {
        return iterator(_nil->_left, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::begin() const noexcept {
        return const_iterator(_nil->_left, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::end() noexcept {
        return iterator(_nil, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::end() const noexcept {
        return const_iterator(_nil, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::reverse_iterator
    red_black_tree<Key, Compare, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_reverse_iterator
    red_black_tree<Key, Compare, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::reverse_iterator
    red_black_tree<Key, Compare, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_reverse_iterator
    red_black_tree<Key, Compare, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::cbegin() const noexcept {
        return const_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::cend() const noexcept {
        return const_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    bool red_black_tree<Key, Compare, Allocator>::empty() const noexcept {
        return _size == 0;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type
    red_black_tree<Key, Compare, Allocator>::size() const noexcept {
        return _size;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type
    red_black_tree<Key, Compare, Allocator>::max_size() const noexcept {
        return std::allocator_traits<node_allocator_type>::max_size(_alloc) - 1;
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::emplace(Args&&... args) {
        return emplace_hint(begin(), std::forward<Args>(args)...);
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::emplace_hint(const_iterator hint, Args&&... args) {
        auto find_pos = find_insert_position(hint, key_type(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)..., color::RED, _nil);
        _insert_child(node, new_node);
        _insert_up(new_node);
        return iterator(new_node, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert(const value_type& x) {
        return emplace_hint(begin(), x);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert(value_type&& x) {
        return emplace_hint(begin(), std::forward<value_type>(x));
    }

    template <class Key, class Compare, class Allocator>
        template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::insert(K&& x) {
        return emplace_hint(begin(), std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert(const_iterator hint, const value_type& x) {
        return emplace_hint(hint, x);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert(const_iterator hint, value_type&& x) {
        return emplace_hint(hint, std::forward<value_type>(x));
    }

    template <class Key, class Compare, class Allocator>
        template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert(const_iterator hint, K&& x) {
        return emplace_hint(hint, std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void red_black_tree<Key, Compare, Allocator>::insert(InputIter first, InputIter last) {
        for (InputIter it = first; it != last; ++it) {
            insert(*it);
        }
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::insert(std::initializer_list<value_type> il) {
        insert(il.begin(), il.end());
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool>
    red_black_tree<Key, Compare, Allocator>::emplace_unique(Args&&... args) {
        return emplace_hint_unique(begin(), std::forward<Args>(args)...);
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::emplace_hint_unique(const_iterator hint, Args&&... args) {
        auto find_pos = find_insert_position(hint, key_type(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            return iterator(node, this);
        }
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)..., color::RED, _nil);
        _insert_child(node, new_node);
        _insert_up(new_node);
        return iterator(new_node, this);
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool>
    red_black_tree<Key, Compare, Allocator>::insert_unique(const value_type& x) {
        return emplace_hint_unique(begin(), x);
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool>
    red_black_tree<Key, Compare, Allocator>::insert_unique(value_type&& x) {
        return emplacee_hint_unique(begin(), std::move(x));
    }

    template <class Key, class Compare, class Allocator>
        template <class K> requires std::convertible_to<K, Key>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator, bool>
    red_black_tree<Key, Compare, Allocator>::insert_unique(K&& x) {
        return emplacee_hint_unique(begin(), std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, const value_type& x) {
        return emplace_hint_unique(hint, x);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, value_type&& x) {
        return emplace_hint_unique(hint, std::move(x));
    }

    template <class Key, class Compare, class Allocator>
        template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, K&& x) {
        return emplace_hint_unique(hint, std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void red_black_tree<Key, Compare, Allocator>::insert_unique(InputIter first, InputIter last) {
        for (InputIter it = first; it != last; ++it) {
            insert_unique(*it);
        }
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::insert_unique(std::initializer_list<value_type> il) {
        insert_unique(il.begin(), il.end());
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::node_type
    red_black_tree<Key, Compare, Allocator>::extract(const_iterator position) {
        if (position == end()) {
            return node_type(_alloc);
        }
        return node_type(_erase(position._ptr), _alloc);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::node_type
    red_black_tree<Key, Compare, Allocator>::extract(const key_type& x) {
        return extract(find(x));
    }

    template <class Key, class Compare, class Allocator>
        template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::node_type
    red_black_tree<Key, Compare, Allocator>::extract(K&& x) {
        return extract(find(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type
    red_black_tree<Key, Compare, Allocator>::insert(node_type&& node) {
        return insert(begin(), std::move(node));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type
    red_black_tree<Key, Compare, Allocator>::insert(const_iterator position, node_type&& node) {
        auto find_pos = find_insert_position(position, node.key());
        auto parent = find_pos.first._ptr;
        auto new_node= node._ptr;
        node._ptr = nullptr;
        _insert_child(parent, new_node);
        _insert_up(new_node);
        return std::make_pair(iterator(new_node, this), node);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type
    red_black_tree<Key, Compare, Allocator>::insert_unique(node_type&& node) {
        return insert_unique(begin(), std::move(node));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::insert_return_type
    red_black_tree<Key, Compare, Allocator>::insert_unique(const_iterator position, node_type&& node) {
        auto find_pos = find_insert_position(position, node.key());
        auto parent = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            return std::make_pair(find_pos.first, node);
        }
        auto new_node= node._ptr;
        node._ptr = nullptr;
        _insert_child(parent, new_node);
        _insert_up(new_node);
        return std::make_pair(iterator(new_node, this), node);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::erase(iterator position)
        requires (!std::is_same_v<_iterator_base<false>, _iterator_base<true>>) {
        return erase(const_iterator(position, this));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::erase(const_iterator position) {
        if (position == end()) {
            return end();
        }
        Node* next_node = std::next(position)._ptr;
        Node* erase_node = _erase(position._ptr);
        std::destroy_at(erase_node);
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, erase_node, 1);
        return iterator(next_node, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type
    red_black_tree<Key, Compare, Allocator>::erase(const key_type& x) {
        const size_type old_size = _size;
        erase(lower_bound(x), upper_bound(x));
        return old_size - _size;
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::size_type red_black_tree<Key, Compare, Allocator>::erase(K&& x) {
        const size_type old_size = _size;
        erase(lower_bound(std::forward<K>(x)), upper_bound(std::forward<K>(x)));
        return old_size - _size;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator red_black_tree<Key, Compare, Allocator>::erase(
        const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(last._ptr, this);
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::swap(red_black_tree& x)
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
        std::is_nothrow_assignable_v<Compare, Compare>) {
        using std::swap;
        swap(_root, x._root);
        swap(_nil, x._nil);
        swap(_size, x._size);
        swap(_comp, x._comp);
        swap(_alloc, x._alloc);
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::unique() {
        if (empty()) return;
        for (auto it = std::next(begin()), temp = begin(); it != end();) {
            if (*it == *temp) {
                it = erase(it);
            } else {
                temp = it++;
            }
        }
    }

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::clear() noexcept {
        _delete(_root);
        _nil->_left =_nil;
        _nil->_right = _nil;
        _root = _nil;
        _size = 0;
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::merge(red_black_tree<key_type, C2, allocator_type>& source) {
        insert(source.begin(), source.end());
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::merge(red_black_tree<key_type, C2, allocator_type>&& source) {
        insert(std::make_move_iterator(source.begin()), std::make_move_iterator(source.end()));
        source.clear();
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::unique_merge(red_black_tree<key_type, C2, allocator_type>& source) {
        insert_unique(source.begin(), source.end());
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void red_black_tree<Key, Compare, Allocator>::unique_merge(red_black_tree<key_type, C2, allocator_type>&& source) {
        insert_unique(std::make_move_iterator(source.begin()), std::make_move_iterator(source.end()));
        source.clear();
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::key_compare
    red_black_tree<Key, Compare, Allocator>::key_comp() const {
        return _comp;
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::value_compare
    red_black_tree<Key, Compare, Allocator>::value_comp() const {
        return value_compare(_comp);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::find(const key_type& x) {
        Node *finder = _root;
        while (finder != _nil) {
            if (finder->_key == x) {
                return iterator(finder, this);
            }
            if (_comp(finder->_key, x)) {
                finder = finder->_right;
            } else {
                finder = finder->_left;
            }
        }
        return end();
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::find(const key_type& x) const {
        return const_iterator(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::find(const K& x) {
        return find(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::find(const K& x) const {
        return find(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::size_type
    red_black_tree<Key, Compare, Allocator>::count(const key_type& x) const {
        return std::distance(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::size_type
    red_black_tree<Key, Compare, Allocator>::count(const K& x) const {
        return count(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    bool red_black_tree<Key, Compare, Allocator>::contains(const key_type& x) const {
        return find(x) != end();
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    bool red_black_tree<Key, Compare, Allocator>::contains(const K& x) const {
        return contains(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) {
        Node *lower = _root;
        Node *result = _nil;
        while (lower && lower != _nil) {
            if (!_comp(lower->_key, x)) {
                result = lower;
                lower = lower->_left;
            } else {
                lower = lower->_right;
            }
        }
        return  iterator(result, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) const {
        return const_iterator(lower_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::lower_bound(const K& x) {
        return lower_bound(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::lower_bound(const K& x) const {
        return const_iterator(lower_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) {
        Node *upper = _root;
        Node *result = _nil;
        while (upper && upper != _nil) {
            if (_comp(x, upper->_key)) {
                result = upper;
                upper = upper->_left;
            } else {
                upper = upper->_right;
            }
        }
        return iterator(result, this);
    }

    template <class Key, class Compare, class Allocator>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) const {
        return const_iterator(upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::iterator
    red_black_tree<Key, Compare, Allocator>::upper_bound(const K& x) {
        return upper_bound(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename red_black_tree<Key, Compare, Allocator>::const_iterator
    red_black_tree<Key, Compare, Allocator>::upper_bound(const K& x) const {
        return const_iterator(upper_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator,
              typename red_black_tree<Key, Compare, Allocator>::iterator>
    red_black_tree<Key, Compare, Allocator>::equal_range(const key_type& x) {
        return std::make_pair(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::const_iterator,
              typename red_black_tree<Key, Compare, Allocator>::const_iterator>
    red_black_tree<Key, Compare, Allocator>::equal_range(const key_type& x) const {
        return std::make_pair(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::iterator,
              typename red_black_tree<Key, Compare, Allocator>::iterator>
    red_black_tree<Key, Compare, Allocator>::equal_range(const K& x) {
        return std::make_pair(lower_bound(static_cast<const key_type&>(x)),
                              upper_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename red_black_tree<Key, Compare, Allocator>::const_iterator,
              typename red_black_tree<Key, Compare, Allocator>::const_iterator>
    red_black_tree<Key, Compare, Allocator>::equal_range(const K& x) const {
        return std::make_pair(lower_bound(static_cast<const key_type&>(x)),
                              upper_bound(static_cast<const key_type&>(x)));
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
        if (left->_right != _nil) {
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
        if (_size == 0) {
            return std::make_pair(end(), true);
        }
        if ((hint == end() || _comp(x, *hint)) && (hint == begin() || _comp(*std::prev(hint), x))) {
            return std::make_pair(hint, true);
        }
        Node *finder = _root;
        if (finder == _nil) {
            return std::make_pair(const_iterator(finder, this), true);
        }
        bool unique = true;
        while (true) {
            if (finder->_key == x) {
                unique = false;
            }
            if (_comp(finder->_key, x)) {
                if (finder->_right == _nil) {
                    return std::make_pair(const_iterator(finder, this), unique);
                }
                finder = finder->_right;
            } else {
                if (finder->_left == _nil) {
                    return std::make_pair(const_iterator(finder, this), unique);
                }
                finder = finder->_left;
            }
        }
    } // return leaf position

    template <class Key, class Compare, class Allocator>
    void red_black_tree<Key, Compare, Allocator>::_insert_child(Node* node, Node* new_node) {
        _size++;
        if (node == _nil) {
            _root = new_node;
            new_node->_parent = node;
            _nil->_left = new_node;
            _nil->_right = new_node;
            return;
        } // empty tree
        if (_comp(node->_key, new_node->_key)) {
            node->_right = new_node;
            if (node == _nil->_right) {
                _nil->_right = new_node;
            }
        } else {
            node->_left = new_node;
            if (node == _nil->_left) {
                _nil->_left = new_node;
            }
        }
        new_node->_parent = node;
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
                parent->_color = color::BLACK;
                grandparent->_color = color::RED;
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
            if (node == _nil->_right) {
                _nil->_right = fix;
            }
        } else {
            erased = find_for_min(node->_right);
            erased_original_color = erased->_color;
            fix = erased->_right;
            if (erased->_parent == node) {
                fix->_parent = erased;
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
        if (erased_original_color == color::BLACK) {
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
        v->_parent = u->_parent;
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
        if (node != _nil) {
            _delete(node->_left);
            _delete(node->_right);
            std::destroy_at(node);
            std::allocator_traits<node_allocator_type>::deallocate(_alloc, node, 1);
        }
        _nil->_left = _nil;
        _nil->_right = _nil;
        _size = 0;
    }
}
