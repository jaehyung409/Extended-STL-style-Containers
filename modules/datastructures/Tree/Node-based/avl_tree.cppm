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

namespace j {
    template <class Key>
    concept IsPair = requires {
        typename Key::first_type;
        typename Key::second_type;
    };

    export template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
    class avl_tree {
    private:
        class _avl_tree_node;
        class _nil_node;
        template <bool IsConst>
        class _iterator_base;

    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        struct value_compare;
        using allocator_type = Allocator;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using difference_type = std::ptrdiff_t;
        class iterator : public _iterator_base<false> {};
        class const_iterator : public _iterator_base<true> {};
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

        Node* _build(vector<value_type> &v, size_type left, size_type right, Node *head);
        Node* _build(vector<Node*> &v, size_type left, size_type right, Node *head);
        Node* _find_begin() const;
        void _set_nil() const;
        void _left_rotate(const_iterator x);
        void _right_rotate(const_iterator x);
        void _rotate_up(const_iterator x);
        std::pair<iterator, bool> find_insert_position(const_iterator hint, const key_type &x); // helper function
        insert_return_type _insert(node_type &&node, const_iterator position); // helper function
        Node* find_for_erase(Node *position); // helper function
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
        template <class K>
        iterator find(K &&x);
        template <class K>
        const_iterator find(K &&x) const;

        size_type count(const key_type &x) const;
        template <class K>
        size_type count(K &&x) const;

        bool contains(const key_type &x) const;
        template <class K>
        bool contains(K &&x) const;

        iterator lower_bound(const key_type &x);
        const_iterator lower_bound(const key_type &x) const;
        template <class K>
        iterator lower_bound(K &&x);
        template <class K>
        const_iterator lower_bound(K &&x) const;

        iterator upper_bound(const key_type &x);
        const_iterator upper_bound(const key_type &x) const;
        template <class K>
        iterator upper_bound(K &&x);
        template <class K>
        const_iterator upper_bound(K &&x) const;

        std::pair<iterator, iterator> equal_range(const key_type &x);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
        template <class K>
        std::pair<iterator, iterator> equal_range(K &&x);
        template <class K>
        std::pair<const_iterator, const_iterator> equal_range(K &&x) const;
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

        bool operator()(const value_type &x, const value_type &y) const {
            if constexpr (IsPair<Key>) {
                return _comp(x.first, y.first);
            } else {
                return _comp(x, y);
            }
        }
    };

    template <class Key, class Compare, class Allocator>
    class avl_tree<Key, Compare, Allocator>::_avl_tree_node {
    protected:
        using node = _avl_tree_node;
        key_type _key = key_type();
        node *_left = nullptr;
        node *_right = nullptr;
        node *_parent = nullptr;
        size_type _height = 0;

    public:
        Key& operator*() { return _key; }
        const Key& operator*() const { return _key; }
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
    private:
        _avl_tree_node *_ptr;

    public:
        explicit node_type(_avl_tree_node *node = nullptr) : _ptr(node) {}
        node_type(const node_type&) = delete;
        node_type& operator=(const node_type&) = delete;
        node_type(node_type &&other) noexcept : _ptr(std::exchange(other._ptr, nullptr)) {}
        node_type& operator=(node_type &&other) noexcept {
            if (this != &other) {
                _ptr = std::exchange(other._ptr, nullptr);
            }
            return *this;
        }
        ~node_type() {
            if (_ptr) {
                std::destroy_at(std::addressof(_ptr->_key));
                std::allocator_traits<node_allocator_type>::deallocate(_ptr->_alloc, _ptr, 1);
            }
        }

        bool empty() const noexcept { return _ptr == nullptr; }
        allocator_type get_allocator() const noexcept { return allocator_type(); }
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
    class avl_tree<Key, Compare, Allocator>::_iterator_base {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = _avl_tree_node*;
        using reference = std::conditional_t<IsPair<Key>,
                                             std::conditional_t<IsConst,
                                                                const Key&,
                                                                std::pair<const typename Key::first_type,
                                                                          typename Key::second_type>&>,
                                             const value_type&>;
    private:
        pointer _ptr;
        avl_tree *_tree;
        explicit _iterator_base(pointer ptr = nullptr, avl_tree *tree = nullptr) : _ptr(ptr), _tree(tree) {}

    public:
        explicit _iterator_base() : _ptr(nullptr), _tree(nullptr) {}
        template <bool OtherConst, typename = std::enable_if_t<OtherConst && !IsConst>>
        explicit _iterator_base(const _iterator_base<OtherConst>& other) : _ptr(other._ptr), _tree(other._tree) {}
        explicit _iterator_base(const _iterator_base& other) : _ptr(other._ptr), _tree(other._tree) {}
        _iterator_base& operator=(const _iterator_base& other) {
            _ptr = other._ptr;
            _tree = other._tree;
            return *this;
        }

        reference operator*() const { return *_ptr; }
        pointer operator->() const { return &*_ptr; }

        _iterator_base &operator++() {
            if (_ptr->right != nullptr) {
                _ptr = _ptr->right;
                if (_ptr == _tree->_nil) {
                    return *this;
                }
                while (_ptr->left != nullptr) {
                    _ptr = _ptr->left;
                }
            } else {
                pointer parent = _ptr->parent;
                while (parent != _tree->_nil && _ptr == parent->right) {
                    _ptr = parent;
                    parent = parent->parent;
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
            if (_ptr->left != nullptr) {
                _ptr = _ptr->left;
                while (_ptr->right != _tree->_nil) {
                    _ptr = _ptr->right;
                }
            } else {
                pointer parent = _ptr->parent;
                while (parent != _tree->_nil && _ptr == parent->left) {
                    _ptr = parent;
                    parent = parent->parent;
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

        bool operator==(const _iterator_base &other) const { return _ptr == other._ptr; }
        bool operator!=(const _iterator_base &other) const { return _ptr != other._ptr; }
    };
}

namespace j {
    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(const Compare& comp, const Allocator& alloc)
        : _size(0), _comp(_comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        _nil->_right = _nil;
        _nil->_parent = _nil;
        _root = _nil;
        _nil->_left = _root;
    }

    template <class Key, class Compare, class Allocator>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    avl_tree<Key, Compare, Allocator>::avl_tree(InputIter first, InputIter last, const Compare& comp,
        const Allocator& alloc) : _comp(comp), _alloc(alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        _nil->_right = _nil;
        _nil->_parent = _nil;
        vector<value_type> v;
        for (InputIter it = first; it != last; ++it) {
            v.push_back(*it);
        }
        _root = _build(v, 0, v.size() - 1, _nil);
        _nil->_left = _root;
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
        std::construct_at(_nil);
        _root = _nil;
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>::avl_tree(const avl_tree& x, const Allocator& alloc)
        : _comp(x._comp), _alloc(_alloc) {
        _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(_nil);
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
        std::construct_at(x._nil);
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
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, _nil, 1);
    }

    template <class Key, class Compare, class Allocator>
    avl_tree<Key, Compare, Allocator>& avl_tree<Key, Compare, Allocator>::operator=(const avl_tree& x) {
        if (this != &x) {
            clear();
            _comp = x._comp;
            _alloc = x._alloc;
            _nil = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
            std::construct_at(_nil);
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
                _root= x._root;
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
    avl_tree<Key, Compare, Allocator>& avl_tree<Key, Compare, Allocator>::operator=(
        std::initializer_list<value_type> il) {
        clear();
        vector<key_type> v(il);
        _root = _build(v, 0, v.size() - 1, _nil);
        _set_nil();
        _size = v.size();
        return *this;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::allocator_type avl_tree<Key, Compare, Allocator>::get_allocator()
    const noexcept {
        return typename std::allocator_traits<Allocator>::template rebind_alloc<Key>;
    }


    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::begin() noexcept {
        return iterator(_nil->_left);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::begin() const noexcept {
        return const_iterator(_nil->_left);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::end() noexcept {
        return iterator(_nil);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::end() const noexcept {
        return const_iterator(_nil);
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
        return const_iterator(_nil->_left);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::cend() const noexcept {
        return const_iterator(_nil);
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
        auto find_pos = find_insert_position(hint, key_type(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(std::addressof(new_node->_key), std::forward<Args>(args)...);
        if (is_unique == false) {
            if (_comp(node->key, args)) {
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
            return iterator(new_node);
        }
        return _insert(node_type(new_node), find_pos.first).first;
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
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::insert(K&& x) {
        return emplace_hint(begin(), std::move(x));
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
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::insert(const_iterator hint, K&& x) {
        return emplace_hint(hint, std::move(x));
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
        auto find_pos = find_insert_position(hint, key_type(std::forward<Args>(args)...));
        auto node = find_pos.first._ptr;
        auto is_unique = find_pos.second;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(std::addressof(new_node->_key), std::forward<Args>(args)...);
        new_node->_left = _nil;
        new_node->_right = _nil;
        if (is_unique == false) {
            return iterator(node);
        }
        return _insert(node_type(new_node), find_pos.first).first;
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
    template <class K>
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
    template <class K>
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
            return node_type();
        }
        return node_type(_erase(position._ptr));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::node_type
    avl_tree<Key, Compare, Allocator>::extract(const key_type& x) {
        return extract(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
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
        if (node.empty()) {
            return std::make_pair(end(), node);
        }
        auto find_pos = find_insert_position(position, node->key);
        auto pos = find_pos.first;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            if (_comp(find_pos->key, node->key)) {
                node->_right = find_pos->_right;
                node->_parent = find_pos;
                find_pos->_right = node;
                if (node->_right == _nil) {
                    _nil->_parent = node;
                }
            } else {
                node->_left = find_pos->_left;
                node->_parent = find_pos;
                find_pos->_left = node;
                if (find_pos == _nil->_left) {
                    _nil->_left = node;
                }
            }
            _rotate_up(node);
            ++_size;
            return std::make_pair(iterator(node), node_type());
        }
        return _insert(node, pos);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert_unique(node_type&& node) {
        return insert_unique(begin(), std::move(node));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::insert_unique(const_iterator position, node_type&& node) {
        if (node.empty()) {
            return std::make_pair(end(), node);
        }
        auto find_pos = find_insert_position(position, node->key);
        auto pos = find_pos.first;
        auto is_unique = find_pos.second;
        if (is_unique == false) {
            return std::make_pair(pos, node_type());
        }
        return _insert(node, pos);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::erase(iterator position)
        requires (!std::is_same_v<iterator, const_iterator>) {
        return erase(const_iterator(position));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::erase(const_iterator position) {
        if (position == end()) {
            return end();
        }
        Node* erase_node = position._ptr;
        Node* next_node = std::next(erase_node);
        erase_node = _erase(erase_node);
        std::destroy_at(std::addressof(erase_node->_key));
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, erase_node, 1);
          return iterator(next_node);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::erase(const key_type& x) {
        return erase(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::erase(K&& x) {
        return erase(find(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(last._ptr);
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
        _nil->_left = _root;
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
                std::construct_at(std::addressof(new_node->_key), *src);
                v.push_back(new_node);
                ++src;
            } else {
                v.push_back(it._ptr);
                ++it;
            }
        }
        for (;src != source.end(); ++src) {
            Node* new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
            std::construct_at(std::addressof(new_node->_key), *src);
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
        _size = v.size();
    }

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
                    std::construct_at(std::addressof(new_node->_key), *src);
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
                std::construct_at(std::addressof(new_node->_key), *src);
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
        _size = v.size();
    }

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
                return iterator(finder);
            }
            if (_comp(finder->_key, x)) {
                finder = finder->_right;
            } else {
                finder = finder->_left;
            }
        }
        return iterator(_nil);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::find(const key_type& x) const {
        return const_iterator(find(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::find(K&& x) {
        return find(std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::const_iterator avl_tree<Key, Compare, Allocator>::find(K&& x) const {
        return const_iterator(find(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::size_type
    avl_tree<Key, Compare, Allocator>::count(const key_type& x) const {
        size_type count = 0;
        auto it = find(x);
        for (; it != end() && *it == x; ++it) {
            ++count;
        }
        return count;
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::size_type avl_tree<Key, Compare, Allocator>::count(K&& x) const {
        return count(std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    bool avl_tree<Key, Compare, Allocator>::contains(const key_type& x) const {
        return find(x) != end();
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    bool avl_tree<Key, Compare, Allocator>::contains(K&& x) const {
        return contains(std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) {
        Node *lower = _root;
        Node *result = _nil;
        while (lower != _nil) {
            if (!_comp(lower->_key, x)) {
                result = lower;
                lower = lower->_left;
            } else {
                lower = lower->_right;
            }
        }
        return  iterator(result);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(const key_type& x) const {
        return const_iterator(lower_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::lower_bound(K&& x) {
        return lower_bound(std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::lower_bound(K&& x) const {
        return const_iterator(lower_bound(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) {
        Node *upper = _root;
        Node *result = _nil;
        while (upper != _nil) {
            if (_comp(x, upper->_key)) {
                result = upper;
                upper = upper->_left;
            } else {
                upper = upper->_right;
            }
        }
        return iterator(result);
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(const key_type& x) const {
        return const_iterator(upper_bound(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::iterator avl_tree<Key, Compare, Allocator>::upper_bound(K&& x) {
        return upper_bound(std::forward<K>(x));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    typename avl_tree<Key, Compare, Allocator>::const_iterator
    avl_tree<Key, Compare, Allocator>::upper_bound(K&& x) const {
        return const_iterator(upper_bound(std::forward<K>(x)));
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
    template <class K>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator,
              typename avl_tree<Key, Compare, Allocator>::iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(K&& x) {
        return std::make_pair(lower_bound(std::forward<K>(x)), upper_bound(std::forward<K>(x)));
    }

    template <class Key, class Compare, class Allocator>
    template <class K>
    std::pair<typename avl_tree<Key, Compare, Allocator>::const_iterator,
              typename avl_tree<Key, Compare, Allocator>::const_iterator>
    avl_tree<Key, Compare, Allocator>::equal_range(K&& x) const {
        return std::make_pair(lower_bound(std::forward<K>(x)), upper_bound(std::forward<K>(x)));
    }

    // Private member functions
    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node*
    avl_tree<Key, Compare, Allocator>::_build(vector<value_type>& v, size_type left, size_type right, Node* head) {
        if (left > right) {
            return nullptr;
        }
        size_type mid = left + (right - left) / 2;
        Node *new_node = std::allocator_traits<node_allocator_type>::allocate(_alloc, 1);
        std::construct_at(std::addressof(new_node->_key), v[mid]);
        new_node->head = head;
        new_node->left = _build(v, left, mid - 1, new_node);
        new_node->right = _build(v, mid + 1, right, new_node);
        return new_node;
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::Node*
    avl_tree<Key, Compare, Allocator>::_build(vector<Node*>& v, size_type left, size_type right, Node* head) {
        if (left > right) {
            return nullptr;
        }
        size_type mid = left + (right - left) / 2;
        v[mid]->head = head;
        v[mid]->left = _build(v, left, mid - 1, v[mid]);
        v[mid]->right = _build(v, mid + 1, right, v[mid]);
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
    void avl_tree<Key, Compare, Allocator>::_left_rotate(const_iterator x) {
        Node *right = x._ptr->_right;
        right->head = x._ptr->head;
        if (x._ptr->head == _nil) {
            _root = right;
        } else if (x._ptr == x._ptr->head->left) {
            x._ptr->head->left = right;
        } else {
            x._ptr->head->right = right;
        }
        x._ptr->right = right->left;
        if (right->left != nullptr) {
            right->left->head = x._ptr;
        }
        right->left = x._ptr;
        x._ptr->head = right;
        x._ptr->height = std::max(x._ptr->left->height, x._ptr->right->height) + 1;
        right->height = std::max(right->left->height, right->right->height) + 1;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_right_rotate(const_iterator x) {
        Node *left = x._ptr->left;
        left->head = x._ptr->head;
        if (x._ptr->head == _nil) {
            _root = left;
        } else if (x._ptr == x._ptr->head->left) {
            x._ptr->head->left = left;
        } else {
            x._ptr->head->right = left;
        }
        x._ptr->left = left->right;
        if (left->right != _nil) {
            left->right->head = x._ptr;
        }
        left->right = x._ptr;
        x._ptr->head = left;
        x._ptr->height = std::max(x._ptr->left->height, x._ptr->right->height) + 1;
        left->height = std::max(left->left->height, left->right->height) + 1;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_rotate_up(const_iterator x) {
        while (x._ptr != _nil) {
            x._ptr->height = std::max(x._ptr->left->height, x._ptr->right->height) + 1;
            if (x._ptr->left->height > x._ptr->right->height + 1) {
                if (x._ptr->left->right->height > x._ptr->left->left->height) {
                    _left_rotate(x._ptr->left);
                }
                _right_rotate(x._ptr);
            } else if (x._ptr->right->height > x._ptr->left->height + 1) {
                if (x._ptr->right->left->height > x._ptr->right->right->height) {
                    _right_rotate(x._ptr->right);
                }
                _left_rotate(x._ptr);
            }
            x = x._ptr->head;
        }
    }

    template <class Key, class Compare, class Allocator>
    std::pair<typename avl_tree<Key, Compare, Allocator>::iterator, bool>
    avl_tree<Key, Compare, Allocator>::find_insert_position(const_iterator hint, const key_type& x) {
        if (*hint == x) {
            return std::make_pair(hint, false);
        }
        if ((hint == end() || _comp(x, *hint)) && (hint == begin() || _comp(*std::prev(hint), x))) {
            return std::make_pair(hint, true);
        }
        Node *finder = _root;
        if (finder == _nil) {
            return std::make_pair(hint, true);
        }
        while (true) {
            if (finder->key == x) {
                return std::make_pair(iterator(finder), false);
            }
            if (_comp(finder->key, x)) {
                if (finder->right == nullptr || finder->right == _nil) {
                    return std::make_pair(iterator(finder), true);
                }
                finder = finder->right;
            } else {
                if (finder->left == nullptr) {
                    return std::make_pair(iterator(finder), true);
                }
                finder = finder->left;
            }
        }
    }

    template <class Key, class Compare, class Allocator>
    typename avl_tree<Key, Compare, Allocator>::insert_return_type
    avl_tree<Key, Compare, Allocator>::_insert(node_type&& node, const_iterator position) {
        if (position == end()) {
            _nil->_left = node._ptr;
            _nil->_parnet = node._ptr;
            node._ptr->_right = _nil;
            node._ptr->_parent = _nil;
            _root = node._ptr;
        } else {
            if (_comp(node->key, *position)) {
                if (position._ptr == _nil->_left) {
                    _nil->_left = node._ptr;
                }
                position._ptr->_left = node._ptr;
            } else {
                if (position._ptr == _nil->_parent) {
                    _nil->_parent = node._ptr;
                    node._ptr->_right = _nil;
                }
                position._ptr->_right = node._ptr;
            }
            _rotate_up(position);
        }
        ++_size;
        return std::make_pair(iterator(node), node_type());
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
        const bool is_right = _comp(position->_parent->key, position->key);
        if (position->_left == nullptr) {
            if (is_right) {
                position->_parent->_right = (position == _nil->_parent) ? _nil : nullptr;
                if (position == _nil->_parent) {
                    _nil->_parent = position->_parent;
                }
            } else {
                position->_parent->_left = nullptr;
                if (position == _nil->left) {
                    _nil->_left = position->_parent;
                }
            }
        } else {
            Node* target = find_for_erase(position);
            if (target == position) {
                target = position->_left;
                if (is_right) {
                    position->_parent->_right = target;
                } else {
                    position->_parent->_left = target;
                }
            } else {
                if (_comp(target->_parent->key, target->key)) {
                    target->_parent->_right = (target == _nil->_parent) ? _nil : target->_right;
                    if (target == _nil->_parent) {
                        _nil->_parent = target->_parent;
                    }
                } else {
                    target->_parent->_left = nullptr;
                }
                if (is_right) {
                    position->_parent->_right = target;
                    target->_parent = position->_parent;
                    target->_left = position->_left;
                    target->_right = position->_right;
                } else {
                    position->_parent->_left = target;
                    target->_parent = position->_parent;
                    target->_left = position->_left;
                    target->_right = position->_right;
                }
            }
            if (_root == position) {
                _root = target;
            }
        }
        _size--;
        return position;
    }

    template <class Key, class Compare, class Allocator>
    void avl_tree<Key, Compare, Allocator>::_delete(Node* node) {
        if (node == _nil) {
            return;
        }
        _delete(node->_left);
        _delete(node->_right);
        std::destroy_at(std::addressof(node->_key));
        std::allocator_traits<node_allocator_type>::deallocate(_alloc, node, 1);
        node = nullptr;
    }
}
