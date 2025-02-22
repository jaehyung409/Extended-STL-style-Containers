/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>
#include <memory>

export module j.avl_tree;

import j.vector;
import j.basics;
import j.tree_helper;

namespace j {
    export template <class Key, class Compare = _default_compare<Key>, class Allocator = std::allocator<Key>>
    class avl_tree {
    private:
        class _avl_tree_node;
        class _nil_node;
        template <bool IsConst>
        class _iterator_base;
        using key_mapper = _key_mapper<Key>;
        using key_extractor = _key_extractor<Key>;

    public:
        using key_type = typename key_mapper::key_type;
        using mapped_type = typename key_mapper::mapped_type;
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
        using Node = _avl_tree_node;
        using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node *_root;
        Node *_nil;// _nil is a dummy node (sentinel), using Nil struct
        size_type _size;
        key_compare _comp;
        node_allocator_type _alloc;

        Node* _build(vector<value_type> &v, int left, int right, Node *head);
        Node* _build(vector<Node*> &v, int left, int right, Node *head);
        Node* _find_begin() const;
        void _set_nil() const;
        void _left_rotate(Node* x);
        void _right_rotate(Node* x);
        void _rotate_up(Node* x);
        std::pair<iterator, bool> _find_insert_position(const key_type &x); // helper function
        std::pair<iterator, bool> _find_insert_position(const_iterator hint, const key_type &x); // helper function
        iterator _insert(Node* node, const_iterator position); // helper function
        Node* _find_for_min(Node *position); // helper function
        Node* _erase(Node *position); // helper function
        void _delete(Node *node); // helper function

    public:
        avl_tree() : avl_tree(Compare()) {}
        explicit avl_tree(const Compare &comp, const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        avl_tree(InputIter first,
                 InputIter last,
                 const Compare &comp = Compare(),
                 const Allocator &alloc = Allocator());
        //template <container-compatible-range<Key> R>
        //avl_tree(from_range_t, R &&range, const Compare &comp = Compare(), const Allocator &alloc = Allocator());
        avl_tree(const avl_tree &x);
        avl_tree(avl_tree &&x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
        explicit avl_tree(const Allocator &alloc);
        avl_tree(const avl_tree &x, const Allocator &alloc);
        avl_tree(avl_tree &&x, const Allocator &alloc);
        avl_tree(std::initializer_list<value_type> il,
                 const Compare &comp = Compare(),
                 const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        avl_tree(InputIter first, InputIter last, const Allocator &alloc);
        //template <container-compatible-range<Key> R>
        //avl_tree(from_range_t, R &&range, const Allocator &alloc)
        //    : avl_tree(from_range, std::forward<R>(range), Compare(), alloc) {}
        avl_tree(std::initializer_list<value_type> il, const Allocator &alloc);
        ~avl_tree();
        avl_tree &operator=(const avl_tree &x);
        avl_tree &operator=(avl_tree &&x) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value &&
            std::is_nothrow_assignable_v<Compare, Compare>);
        avl_tree &operator=(std::initializer_list<value_type> il);
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
        void swap(avl_tree &x) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value &&
            std::is_nothrow_assignable_v<Compare, Compare>);
        void unique();
        void clear() noexcept;

        template <class C2>
        void merge(avl_tree<key_type, C2, allocator_type> &source);
        template <class C2>
        void merge(avl_tree<key_type, C2, allocator_type> &&source);
        template <class C2>
        void unique_merge(avl_tree<key_type, C2, allocator_type> &source);
        template <class C2>
        void unique_merge(avl_tree<key_type, C2, allocator_type> &&source);

        // Observers
        key_compare key_comp() const;
        value_compare value_comp() const;

        // Operations
        iterator find(const key_type &x);
        const_iterator find(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        iterator find(const K &x);
        template <class K>
        requires std::convertible_to<K, Key>
        const_iterator find(const K &x) const;

        size_type count(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        size_type count(const K &x) const;

        bool contains(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        bool contains(const K &x) const;

        iterator lower_bound(const key_type &x);
        const_iterator lower_bound(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        iterator lower_bound(const K &x);
        template <class K> requires std::convertible_to<K, Key>
        const_iterator lower_bound(const K &x) const;

        iterator upper_bound(const key_type &x);
        const_iterator upper_bound(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        iterator upper_bound(const K &x);
        template <class K> requires std::convertible_to<K, Key>
        const_iterator upper_bound(const K &x) const;

        std::pair<iterator, iterator> equal_range(const key_type &x);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
        template <class K> requires std::convertible_to<K, Key>
        std::pair<iterator, iterator> equal_range(const K &x);
        template <class K> requires std::convertible_to<K, Key>
        std::pair<const_iterator, const_iterator> equal_range(const K &x) const;
    };

    template <class InputIter,
              class Compare = std::less<typename std::iterator_traits<InputIter>::value_type>,
              class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
    avl_tree(InputIter, InputIter, Compare = Compare(), Allocator = Allocator())
        -> avl_tree<typename std::iterator_traits<InputIter>::value_type, Compare, Allocator>;

    //template <ranges::input_range R,
    //          class Compare = std::less<std::ranges::range_value_t<R>>,
    //          class Allocator = std::allocator<std::ranges::range_value_t<R>>>
    //avl_tree(from_range_t, R &&, Compare = Compare(), Allocator = Allocator())
    //    -> avl_tree<std::ranges::range_value_t<R>, Compare, Allocator>;

    template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    avl_tree(std::initializer_list<Key>, Compare = Compare(), Allocator = Allocator())
        -> avl_tree<Key, Compare, Allocator>;

    template <class InputIter, class Allocator>
    avl_tree(InputIter, InputIter, Allocator)
        -> avl_tree<typename std::iterator_traits<InputIter>::value_type, std::less<>, Allocator>;

    //template <ranges::input_range R, class Allocator>
    //avl_tree(from_range_t, R &&, Allocator)
    //    -> avl_tree<std::ranges::range_value_t<R>, std::less<>, Allocator>;

    template <class Key, class Allocator>
    avl_tree(std::initializer_list<Key>, Allocator)
        -> avl_tree<Key, std::less<Key>, Allocator>;

    template <class Key, class Compare, class Allocator>
    struct avl_tree<Key, Compare, Allocator>::value_compare  {
        Compare _comp;
        explicit value_compare(Compare c) : _comp(c) {}

        bool operator()(const value_type &lhs, const value_type &rhs) const {
            if constexpr (_is_std_pair_v<Key>) {
                return _comp(lhs.first, rhs.first);
            } else {
                return _comp(lhs, rhs);
            }
        }
    };

    template <class Key, class Compare, class Allocator>
    class avl_tree<Key, Compare, Allocator>::_avl_tree_node {
    friend class avl_tree;
    friend class _iterator_base<false>;
    friend class _iterator_base<true>;

    protected:
        using node = _avl_tree_node;
        key_type _key;
        node *_left;
        node *_right;
        node *_parent;
        size_type _height;

    public:
        _avl_tree_node() : _key(key_type()), _left(nullptr), _right(nullptr), _parent(nullptr), _height(0) {}
        _avl_tree_node(const key_type &key)
            : _key(key), _left(nullptr), _right(nullptr), _parent(nullptr), _height(0) {}
        _avl_tree_node(key_type &&key)
            : _key(std::move(key)), _left(nullptr), _right(nullptr), _parent(nullptr), _height(0) {}
        key_type& operator*() { return _key; }
        const key_type& operator*() const { return _key; }
        ~_avl_tree_node() {
            _key.~key_type();
            _left = nullptr;
            _right = nullptr;
            _parent = nullptr;
        }
    };

    template <class Key, class Compare, class Allocator>
    class avl_tree<Key, Compare, Allocator>::_nil_node : public _avl_tree_node {
    public:
        _nil_node() {
            this->_left = this;
            this->_right = this;
            this->_parent = this;
        }
    };

    template <class Key, class Compare, class Allocator>
    class avl_tree<Key, Compare, Allocator>::node_type {
        friend class avl_tree;
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
            if constexpr (is_std_pair_v<Key>) {
                return _ptr->_key.first;
            } else {
                return _ptr->_key;
            }
        }
        const auto& value() const {
            if (empty()) throw std::runtime_error("node_type is empty");
            if constexpr (is_std_pair_v<Key>) {
                return _ptr->_key.second;
            } else {
                static_assert(sizeof(Key) == 0, "value() is not available for non-pair key_type");
            }
        }
    };

    template <class Key, class Compare, class Allocator>
    template <bool IsConst>
    class avl_tree<Key, Compare, Allocator>::_iterator_base {
        friend class avl_tree;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = _avl_tree_node*;
        using reference = _reference_selector<Key, IsConst>;

    protected:
        pointer _ptr;
        avl_tree *_tree;
        explicit _iterator_base(pointer ptr = nullptr, avl_tree *tree = nullptr) : _ptr(ptr), _tree(tree) {}

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
    avl_tree<Key, Compare, Allocator>::avl_tree(const Compare& comp, const Allocator& alloc)
        : _size(0), _comp(comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(static_cast<_nil_node*>(_nil));
        _root = _nil;
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    avl_tree<Key, Compare, Allocator>::avl_tree(InputIter first, InputIter last, const Compare& comp,
        const Allocator& alloc) : _comp(comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(static_cast<_nil_node*>(_nil));
        vector<value_type> v;
        for (InputIter it = first; it != last; ++it) {
            v.push_back(*it);
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _set_nil();
        _size = v.size();
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(const avl_tree& x) : avl_tree(x, x._alloc) {}

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(avl_tree&& x)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
        : avl_tree(std::move(x), x._alloc) {}

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(const Allocator& alloc)
        : _size(0), _alloc(alloc){
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(static_cast<_nil_node*>(_nil));
        _root = _nil;
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(const avl_tree& x, const Allocator& alloc)
        : _comp(x._comp), _alloc(_alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(static_cast<_nil_node*>(_nil));
        vector<value_type> v;
        for (const auto& elem : x) {
            v.push_back(elem);
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _set_nil();
        _size = v.size();
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(avl_tree&& x, const Allocator& alloc)
        : _root(x._root), _nil(x._nil), _size(x._size), _comp(std::move(x._comp)), _alloc(alloc) {
        x._nil = std::allocator_traits<node_allocator_type>::allocate(x._alloc, 1);
        std::construct_at(static_cast<_nil_node*>(x._nil));
        x._root = x._nil;
        x._size = 0;
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(std::initializer_list<value_type> il, const Compare& comp,
        const Allocator& alloc) : avl_tree(il.begin(), il.end(), comp, alloc) {}

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    avl_tree<Key, Compare, Allocator>::avl_tree(InputIter first, InputIter last, const Allocator& alloc)
        : avl_tree(first, last, Compare(), alloc){}

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(std::initializer_list<value_type> il, const Allocator& alloc)
        : avl_tree(il, Compare(), alloc) {}

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::~avl_tree() {
        clear();
        std::destroy_at(static_cast<_nil_node*>(_nil));
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, _nil, 1);
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>& avl_tree<Key, Compare, Allocator>::operator=(const avl_tree& x) {
        if (this != &x) {
            clear();
            _comp = x._comp;
            _alloc = x._alloc;
            _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
            std::construct_at(static_cast<_nil_node*>(_nil));
            vector<value_type> v;
            for (const auto& elem : x) {
                v.push_back(elem);
            }
            _root = _build(v, 0, v.size() - 1, _nil);
            _set_nil();
            _size = v.size();
        }
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>& avl_tree<Key, Compare, Allocator>::operator=(avl_tree&& x)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value &&
             std::is_nothrow_assignable_v<Compare, Compare>) {
        if (this != x) {
            clear();
            std::destroy_at(static_cast<_nil_node*>(_nil));
            std::allocator_traits<node_allocator_type>::deallocate(_alloc, _nil, 1);
           _root = x._root;
           _nil = x._nil;
           _size = x._size;
           _comp = std::move(x._comp);
           _alloc = x._alloc;
           x._nil = std::allocator_traits<node_allocator_type>::allocate(x._alloc, 1);
           std::construct_at(x._nil);
           x._root = x._nil;
           x._size = 0;
        }
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>&
    avl_tree<Key, Compare, Allocator>::operator=(std::initializer_list<value_type> il) {
        clear();
        vector<key_type> v(il);
        _root = _build(v, 0, v.size() - 1, _nil);
        _set_nil();
        _size = v.size();
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::allocator_type
    avl_tree<Key, Compare, Allocator>::get_allocator() const noexcept {
        using alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Key>;
        return alloc();
    }


    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::begin() noexcept {
        return iterator(_nil->_left, this);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::begin() const noexcept {
        return const_iterator(_nil->_left, this);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::end() noexcept {
        return iterator(_nil, this);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::end() const noexcept {
        return const_iterator(_nil, this);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::reverse_iterator
    avl_tree<Key, Compare, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_reverse_iterator
    avl_tree<Key, Compare, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::reverse_iterator
    avl_tree<Key, Compare, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_reverse_iterator
    avl_tree<Key, Compare, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::cbegin() const noexcept {
        return const_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::cend() const noexcept {
        return const_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class Key, class Compare, class Allocator>
    bool avl_tree<Key, Compare, Allocator>::empty() const noexcept {
        return _size == 0;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::size() const noexcept {
        return _size;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::max_size() const noexcept {
        return std::allocator_traits<node_allocator_type>::max_size(_alloc) - 1;
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::emplace(Args&&... args) {
        return emplace_hint(begin(), std::forward<Args>(args)...);
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::emplace_hint(const_iterator hint, Args&&... args) {
        auto find_pos = _find_insert_position(hint, key_extractor(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        if (is_unique == false) {
            if (_comp(node->_key, new_node->_key)) {
                new_node->_right = node->_right;
                new_node->_parent = node;
                node->_right = new_node;
                if (new_node->_right == _nil) {
                    _nil->_parent = new_node;
                }
            } else {
                new_node->_left = node->_left;
                new_node->_parent = node;
                node->_left = new_node;
                if (node == _nil->_left) {
                    _nil->_left = new_node;
                }
            }
            _rotate_up(new_node);
            ++_size;
            return iterator(new_node, this);
        }
        return _insert(new_node, find_pos.first);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert(const value_type& x) {
        return emplace_hint(begin(), x);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::insert(value_type&& x) {
        return emplace_hint(begin(), std::forward<value_type>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::insert(K&& x) {
        return emplace_hint(begin(), std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert(const_iterator hint, const value_type& x) {
        return emplace_hint(hint, x);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert(const_iterator hint, value_type&& x) {
        return emplace_hint(hint, std::forward<value_type>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert(const_iterator hint, K&& x) {
        return emplace_hint(hint, std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void avl_tree<Key, Compare, Allocator>::insert(InputIter first, InputIter last) {
        for (InputIter it = first; it != last; ++it) {
            insert(*it);
        }
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::insert(std::initializer_list<value_type> il) {
        insert(il.begin(), il.end());
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::emplace_unique(Args&&... args) {
        return emplace_hint_unique(begin(), std::forward<Args>(args)...);
    }

    template <class Key, class Compare, class Allocator>
    template <class ... Args>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::emplace_hint_unique(const_iterator hint, Args&&... args) {
        auto find_pos = _find_insert_position(hint, key_extractor(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            return iterator(node, this);
        }
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        return _insert(new_node, find_pos.first);
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::insert_unique(const value_type& x) {
        return emplace_hint_unique(begin(), x);
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::insert_unique(value_type&& x) {
        return emplacee_hint_unique(begin(), std::move(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::insert_unique(K&& x) {
        return emplacee_hint_unique(begin(), std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, const value_type& x) {
        return emplace_hint_unique(hint, x);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, value_type&& x) {
        return emplace_hint_unique(hint, std::move(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert_unique(const_iterator hint, K&& x) {
        return emplace_hint_unique(hint, std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    void avl_tree<Key, Compare, Allocator>::insert_unique(InputIter first, InputIter last) {
        for (InputIter it = first; it != last; ++it) {
            insert_unique(*it);
        }
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::insert_unique(std::initializer_list<value_type> il) {
        insert_unique(il.begin(), il.end());
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::node_type
    avl_tree<Key, Compare, Allocator>::extract(const_iterator position) {
        if (position == end()) {
            return node_type(_alloc);
        }
        return node_type(_erase(position._ptr), _alloc);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::node_type
    avl_tree<Key, Compare, Allocator>::extract(const key_type& x) {
        return extract(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::node_type avl_tree<Key, Compare, Allocator>::extract(K&& x) {
        return extract(find(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert(node_type&& node) {
        return insert(begin(), std::move(node));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert(const_iterator position, node_type&& node) {
        auto find_pos = _find_insert_position(position, node.key());
        auto pos = find_pos.first;
        auto is_unique = find_pos.second;
        Node* new_node = node._ptr;
        node._ptr = nullptr;
        if (is_unique == false) {
            if (_comp(find_pos->_key, new_node->_key)) {
                new_node->_right = find_pos->_right;
                new_node->_parent = find_pos;
                find_pos->_right = new_node;
                if (new_node->_right == _nil) {
                    _nil->_parent = new_node;
                }
            } else {
                new_node->_left = find_pos->_left;
                new_node->_parent = find_pos;
                find_pos->_left = new_node;
                if (find_pos == _nil->_left) {
                    _nil->_left = new_node;
                }
            }
            _rotate_up(new_node);
            ++_size;
            return std::make_pair(iterator(new_node, this), node);
        }
        return std::make_pair(_insert(new_node, pos), node);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert_unique(node_type&& node) {
        return insert_unique(begin(), std::move(node));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert_unique(const_iterator position, node_type&& node) {
        auto find_pos = _find_insert_position(position, node.key());
        auto pos = find_pos.first;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            return std::make_pair(pos, node);
        }
        Node* new_node = node._ptr;
        node._ptr = nullptr;
        return std::make_pair(_insert(new_node, pos), node);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::erase(iterator position)
        requires (!std::is_same_v<iterator, const_iterator>) {
        return erase(const_iterator(position, this));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::erase(const_iterator position) {
        if (position == end()) {
            return end();
        }
        Node* erase_node = position._ptr;
        Node* next_node = std::next(position)._ptr;
        erase_node = _erase(erase_node);
        std::destroy_at(erase_node);
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, erase_node, 1);
        return iterator(next_node, this);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::erase(const key_type& x) {
        size_type old_size = _size;
        erase(lower_bound(x), upper_bound(x));
        return old_size - _size;
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::erase(K&& x) {
        size_type old_size = _size;
        erase(lower_bound(std::forward<K>(x)), upper_bound(std::forward<K>(x)));
        return old_size - _size;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(last._ptr, this);
    } // consideration : bulk delete

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::swap(avl_tree& x)
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
    void avl_tree<Key, Compare, Allocator>::unique() {
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
    void avl_tree<Key, Compare, Allocator>::clear() noexcept {
        _delete(_root);
        _nil->_left =_nil;
        _nil->_parent = _nil;
        _root = _nil;
        _size = 0;
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void avl_tree<Key, Compare, Allocator>::merge(avl_tree<key_type, C2, allocator_type>& source) {
        vector<Node*> v;
        auto src = source.begin();
        auto it = begin();
        while (src != source.end() && it != end()) {
            if (_comp(*src, *it)) {
                Node* new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
                std::construct_at(new_node, *src);
                v.push_back(new_node);
                ++src;
            } else {
                v.push_back(it._ptr);
                ++it;
            }
        }
        for (;src != source.end(); ++src) {
            Node* new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
            std::construct_at(new_node, *src);
            v.push_back(new_node);
        }
        for (; it != end(); ++it) {
            v.push_back(it._ptr);
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _nil->_left = _root;
        _size = v.size();
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void avl_tree<Key, Compare, Allocator>::merge(avl_tree<key_type, C2, allocator_type>&& source) {
        vector<Node*> v;
        auto src = source.begin();
        auto it = begin();
        while (src != source.end() && it != end()) {
            if (_comp(*src, *it)) {
                v.push_back(src._ptr);
                ++src;
            } else {
                v.push_back(it._ptr);
                ++it;
            }
        }
        for (;src != source.end(); ++src) {
            v.push_back(src._ptr);
        }
        for (; it != end(); ++it) {
            v.push_back(it._ptr);
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _nil->_left = _root;
        source._root = source._nil;
        source._size = 0;
        source._nil->_left = source._nil;
        source._nil->_parent = source._nil;
        _size = v.size();
    } // need to check allocator_type (same is ok, diff -> make new_node and deallocate)

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void avl_tree<Key, Compare, Allocator>::unique_merge(avl_tree<key_type, C2, allocator_type>& source) {
        vector<Node*> v;
        auto src = source.begin();
        auto it = begin();
        key_type last_key = _comp(*src, *it) ? *src : *it;
        while (src != source.end() && it != end()) {
            if (_comp(*src, *it)) {
                if (last_key != *src) {
                    Node* new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
                    std::construct_at(new_node, *src);
                    v.push_back(new_node);
                    last_key = *src;
                }
                ++src;
            } else {
                if (last_key != *it) {
                    v.push_back(it._ptr);
                    last_key = *it;
                }
                ++it;
            }
        }
        for (;src != source.end(); ++src) {
            if (last_key != *src) {
                Node* new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
                std::construct_at(new_node, *src);
                v.push_back(new_node);
                last_key = *src;
            }
        }
        for (; it != end(); ++it) {
            if (last_key != *it) {
                v.push_back(it._ptr);
                last_key = *it;
            }
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _nil->_left = _root;
        _size = v.size();
    }

    template <class Key, class Compare, class Allocator>
    template <class C2>
    void avl_tree<Key, Compare, Allocator>::unique_merge(avl_tree<key_type, C2, allocator_type>&& source) {
        vector<Node*> v;
        auto src = source.begin();
        auto it = begin();
        key_type last_key = _comp(*src, *it) ? *src : *it;
        while (src != source.end() && it != end()) {
            if (_comp(*src, *it)) {
                if (last_key != *src) {
                    v.push_back(src._ptr);
                    last_key = *src;
                }
                ++src;
            } else {
                if (last_key != *it) {
                    v.push_back(it._ptr);
                    last_key = *it;
                }
                ++it;
            }
        }
        for (;src != source.end(); ++src) {
            if (last_key != *src) {
                v.push_back(src._ptr);
                last_key = *src;
            }
        }
        for (; it != end(); ++it) {
            if (last_key != *it) {
                v.push_back(it._ptr);
                last_key = *it;
            }
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _nil->_left = _root;
        source._root = source._nil;
        source._size = 0;
        source._nil->_left = source._nil;
        source._nil->_parent = source._nil;
        _size = v.size();
    } // need to check allocator_type (same is ok, diff -> make new_node and deallocate)

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::key_compare avl_tree<Key, Compare, Allocator>::key_comp() const {
        return _comp;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::value_compare avl_tree<Key, Compare, Allocator>::value_comp() const {
        return value_compare(_comp);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::find(const key_type& x) {
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
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::find(const key_type& x) const {
        return const_iterator(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::find(const K& x) {
        return find(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::const_iterator avl_tree<Key, Compare, Allocator>::find(const K& x) const {
        return const_iterator(find(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type
    avl_tree<Key, Compare, Allocator>::count(const key_type& x) const {
        return std::distance(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::count(const K& x) const {
        return count(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    bool avl_tree<Key, Compare, Allocator>::contains(const key_type& x) const {
        return find(x) != end();
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    bool avl_tree<Key, Compare, Allocator>::contains(const K& x) const {
        return contains(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) {
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
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) const {
        return const_iterator(lower_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::lower_bound(const K& x) {
        return lower_bound(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(const K& x) const {
        return const_iterator(lower_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) {
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
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) const {
        return const_iterator(upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::upper_bound(const K& x) {
        return upper_bound(static_cast<const key_type&>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(const K& x) const {
        return const_iterator(upper_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator,
              typename avl_tree<Key, Compare, Allocator>::iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(const key_type& x) {
        return std::make_pair(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::const_iterator,
              typename avl_tree<Key, Compare, Allocator>::const_iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(const key_type& x) const {
        return std::make_pair(lower_bound(x), upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator,
              typename avl_tree<Key, Compare, Allocator>::iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(const K& x) {
        return std::make_pair(lower_bound(static_cast<const key_type&>(x)),
                              upper_bound(static_cast<const key_type&>(x)));
    }

    template <class Key, class Compare, class Allocator>
    template <class K> requires std::convertible_to<K, Key>
    std::pair<typename avl_tree<Key, Compare, Allocator>::const_iterator,
              typename avl_tree<Key, Compare, Allocator>::const_iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(const K& x) const {
        return std::make_pair(lower_bound(static_cast<const key_type&>(x)),
                              upper_bound(static_cast<const key_type&>(x)));
    }

    // Private member functions
    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node*
    avl_tree<Key, Compare, Allocator>::_build(vector<value_type>& v, int left, int right, Node* head) {
        if (left > right) {
            return nullptr;
        }
        int mid = left + (right - left) / 2;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(new_node, v[mid]);
        new_node->_parent = head;
        new_node->_left = _build(v, left, mid - 1, new_node);
        new_node->_right = _build(v, mid + 1, right, new_node);
        return new_node;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node*
    avl_tree<Key, Compare, Allocator>::_build(vector<Node*>& v, int left, int right, Node* head) {
        if (left > right) {
            return nullptr;
        }
        int mid = left + (right - left) / 2;
        v[mid]->_parent = head;
        v[mid]->_left = _build(v, left, mid - 1, v[mid]);
        v[mid]->_right = _build(v, mid + 1, right, v[mid]);
        return v[mid];
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node* avl_tree<Key, Compare, Allocator>::_find_begin() const {
        Node *finder = _root;
        while (finder->_left != nullptr) {
            finder = finder->_left;
        }
        return finder;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_set_nil() const {
        _nil->_left = _find_begin();
        Node* finder = _root;
        while (finder->_right != nullptr) {
            finder = finder->_right;
        }
        finder->_right = _nil;
        _nil->_root = finder;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_left_rotate(Node* x) {
        Node *right = x->_right;
        right->_parent = x->_parent;
        if (x->_parent == _nil) {
            _root = right;
        } else if (x == x->_parent->_left) {
            x->_parent->_left = right;
        } else {
            x->_parent->_right = right;
        }
        x->_right = right->_left;
        if (right->_left != nullptr) {
            right->_left->_parent = x;
        }
        right->_left = x;
        x->_parent = right;
        size_type left_height = (x->_left) ? x->_left->_height : 0;
        size_type right_height = (x->_right) ? x->_right->_height : 0;
        x->_height = std::max(left_height, right_height) + 1;
        right->_height = std::max(right->_left->_height, right->_right->_height) + 1;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_right_rotate(Node* x) {
        Node *left = x->_left;
        left->_parent = x->_parent;
        if (x->_parent == _nil) {
            _root = left;
        } else if (x == x->_parent->_left) {
            x->_parent->_left = left;
        } else {
            x->_parent->_right = left;
        }
        x->_left = left->_right;
        if (left->_right != _nil) {
            left->_right->_parent = x;
        }
        left->_right = x;
        x->_parent = left;
        size_type left_height = (x->_left) ? x->_left->_height : 0;
        size_type right_height = (x->_right && x->_right != _nil) ? x->_right->_height : 0;
        x->_height = std::max(left_height, right_height) + 1;
        left->_height = std::max(left->_left->_height, left->_right->_height) + 1;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_rotate_up(Node* x) {
        while (x != _nil) {
            size_type left_height = (x->_left) ? x->_left->_height : 0;
            size_type right_height = (x->_right && x->_right != _nil) ? x->_right->_height : 0;
            x->_height = std::max(left_height, right_height) + 1;
            if (left_height > right_height + 1) {
                left_height = (x->_left->_left) ? x->_left->_left->_height : 0;
                right_height = (x->_left->_right) ? x->_left->_right->_height : 0;
                if (right_height > left_height) {
                    _left_rotate(x->_left);
                }
                _right_rotate(x);
            } else if (right_height > left_height + 1) {
                left_height = (x->_right->_left) ? x->_right->_left->_height : 0;
                right_height = (x->_right->_right && x->_right->_right != _nil) ? x->_right->_right->_height : 0;
                if (left_height > right_height) {
                    _right_rotate(x->_right);
                }
                _left_rotate(x);
            }
            x = x->_parent;
        }
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::_find_insert_position(const key_type& x) {
        return _find_insert_position(begin(), x);
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::_find_insert_position(const_iterator hint, const key_type& x) {
        if (_size == 0) {
            return std::make_pair(end(), true);
        }
        if (_size != 0 && *hint == x) {
            return std::make_pair(hint, false);
        }
        if ((hint == end() || _comp(x, *hint)) && (hint == begin() || _comp(*std::prev(hint), x))) {
            return std::make_pair(hint, true);
        }
        Node *finder = _root;
        if (finder == _nil) {
            return std::make_pair(const_iterator(finder, this), true);
        }
        while (true) {
            if (finder->_key == x) {
                return std::make_pair(const_iterator(finder, this), false);
            }
            if (_comp(finder->_key, x)) {
                if (finder->_right == nullptr || finder->_right == _nil) {
                    return std::make_pair(const_iterator(finder, this), true);
                }
                finder = finder->_right;
            } else {
                if (finder->_left == nullptr) {
                    return std::make_pair(const_iterator(finder, this), true);
                }
                finder = finder->_left;
            }
        }
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::_insert(Node* node, const_iterator position) {
        if (position == end()) {
            node->_parent = _nil->_parent;
            node->_right = _nil;
            _nil->_parent = node;
            if (node->_parent == _nil) {
                _nil->_left = node;
                _root = node;
            } else {
                node->_parent->_right = node;
            }
        } else {
            if (_comp(node->_key, *position)) {
                if (position._ptr == _nil->_left) {
                    _nil->_left = node;
                }
                position._ptr->_left = node;
            } else {
                if (position._ptr == _nil->_parent) {
                    _nil->_parent = node;
                    node->_right = _nil;
                }
                position._ptr->_right = node;
            }
            node->_parent = position._ptr;
        }
        _rotate_up(node);
        ++_size;
        return iterator(node, this);
    } // insert node into position

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node*
    avl_tree<Key, Compare, Allocator>::find_for_erase(Node *position) {
        if (position->_right == nullptr || position->_right == _nil) {
            return position;
        }
        position = position->_right;
        while (position->_left != nullptr) {
            position = position->_left;
        }
        return position;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node* avl_tree<Key, Compare, Allocator>::_erase(Node* position) {
        if (_size == 1) {
            _nil->_left = _nil;
            _nil->_parent = _nil;
            _root = _nil;
            _size = 0;
            return position;
        }
        Node *target = find_for_erase(position);
        Node *rotate_target = (target->_parent == position || target == _root) ? target : target->_parent;
        if (position == target) {                   // no right child
            if (position->_left == nullptr) {       // no child -> position is not root (size > 1)
                if (_comp(position->_key, position->_parent->_key)) {
                    position->_parent->_left = position->_left;
                    if (position == _nil->_left) {
                        _nil->_left = position->_parent;
                    }
                } else {
                    position->_parent->_right = position->_left;
                    position->_left->_parent = position->_parent;
                    if (position == _nil->_parent) {
                        _nil->_parent = position->_left;
                        position->_left->_right = _nil;
                    }
                }
            } else {
                if (position == _root) {
                    _root = position->_left;
                } else if (_comp(position->_key, position->_parent->_key)) {
                    position->_parent->_left = position->_left;
                } else {
                    position->_parent->_right = position->_left;
                    if (position == _nil->_parent) {
                        _nil->_parent = position->_left;
                    }
                }
                position->_left->_parent = position->_parent;
            }
        } else {
            if (_comp(target->_key, target->_parent->_key)) {
                target->_parent->_left = target->_right;
            } else {
                target->_parent->_right = target->_right;
            }
            if (target->_right) {
                target->_right->_parent = target->_parent;
            }
            if (position == _nil->_left) {
                _nil->_left = target;
            }
            if (position == _root) {
                _root = target;
            } else if (_comp(position->_key, position->_parent->_key)) {
                position->_parent->_left = target;
            } else {
                position->_parent->_right = target;
            }
            target->_parent = position->_parent;
            target->_left = position->_left;
            target->_right = position->_right;
        }
        _rotate_up(rotate_target);
        _size--;
        return position;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_delete(Node* node) {
        if (node == _nil || node == nullptr) {
            return;
        }
        _delete(node->_left);
        _delete(node->_right);
        std::destroy_at(node);
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, node, 1);
    }
}
