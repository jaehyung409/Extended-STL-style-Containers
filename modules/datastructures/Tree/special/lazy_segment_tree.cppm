/*
 * @ Created by jaehyung409 on 25. 1. 31.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License.
 */

module;
#include <span>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <iostream> // for debug

export module j.lazy_segment_tree;

import j.basics;

namespace j {
    export template<class T,
            class Allocator = std::allocator<T>,
            class Operator = plus <T>> // operator (monoid)
    class lazy_segment_tree {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using allocator_type = Allocator;

    private:
        size_type _size;            // size of elements
        size_type _array_size;      // size of segment tree array
        value_type _identity;       // identity element
        allocator_type _alloc;
        Operator _op;
        std::function<T(T, T)> _lazy_op;
        std::function<T(T, T)> _lazy_op_inv;
        value_type _lazy_identity;
        pointer _tree;
        pointer _lazy;              // lazy propagation
        bool _is_built = false;     // built flag

        size_type _resize(size_type size);
        void _push(size_type pos, const size_type length) const {
            if (_lazy[pos] != _lazy_identity) {
                _apply(pos << 1, _lazy[pos], length >> 1);
                _apply(pos << 1 | 1, _lazy[pos], length >> 1);
                _lazy[pos] = _lazy_identity;
            }
        } // push lazy propagation
        void _apply(size_type pos, const T& value, const size_type length) const {
            _tree[pos] = _lazy_op(_tree[pos], value);
            if (pos < _size) {
                for (size_type i = 1; i < length; ++i) {
                    _tree[pos] = _lazy_op_inv(_tree[pos], value);
                }
                _lazy[pos] = _lazy_op_inv(_lazy[pos], value);
            }
        } // update value
        void _pull(size_type pos) const {
            _tree[pos] = _op(_tree[pos << 1], _tree[pos << 1 | 1]);
        }

    public:
        lazy_segment_tree() : lazy_segment_tree(0, Operator()) {}
        explicit lazy_segment_tree(size_type size) : lazy_segment_tree(size, Operator()) {}
        explicit lazy_segment_tree(size_type size, const Operator &op) : lazy_segment_tree(size, op, Allocator()) {}
        lazy_segment_tree(size_type size, const Operator &op, const Allocator &alloc);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        lazy_segment_tree(InputIter first, InputIter last,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        lazy_segment_tree(InputIter first, InputIter last, size_type size_hint,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        //template <container-compatible-range<T> R>
        //lazy_segment_tree(from_range_t, R&& range, const Operator &op = Operator(), const Allocator &alloc = Allocator());
        lazy_segment_tree(const lazy_segment_tree &other);
        lazy_segment_tree(lazy_segment_tree &&other);
        lazy_segment_tree(const lazy_segment_tree &other, const std::type_identity_t<Allocator> &alloc);
        lazy_segment_tree(lazy_segment_tree &&other, const std::type_identity_t<Allocator> &alloc);
        lazy_segment_tree(std::initializer_list<T> il,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        ~lazy_segment_tree();
        lazy_segment_tree &operator=(const lazy_segment_tree &other);
        lazy_segment_tree &operator=(lazy_segment_tree &&other) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
        std::allocator_traits<Allocator>::is_always_equal::value);
        lazy_segment_tree &operator=(std::initializer_list<T> il);
        allocator_type get_allocator() const noexcept;

        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void assign(InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //void assign_range(R&& rg);
        void assign(std::initializer_list<T> il);

        bool empty() const;
        size_type size() const; // return the number of elements
        size_type tree_size() const; // return the size of the segment tree (real memory size)
        size_type resize(size_type size);

        void build();
        bool is_built() const;
        void update(size_type pos, const T &value);
        void update(size_type left, size_type right, const T &value);
        void flush() const;
        Operator get_range_operator() const;
        void set_range_operator(std::function<T(T, T)> op, std::function<T(T, T)> inv, T identity);

        size_type query(size_type pos);
        size_type query(size_type left, size_type right);
        std::span<const T> data() const;
        void swap(lazy_segment_tree& x) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
        std::allocator_traits<Allocator>::is_always_equal::value) {
            using std::swap;
            swap(_size, x._size);
            swap(_array_size, x._array_size);
            swap(_identity, x._identity);
            swap(_alloc, x._alloc);
            swap(_op, x._op);
            swap(_tree, x._tree);
            swap(_is_built, x._is_built);
        }
        void clear();
    };
}

namespace j {

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(lazy_segment_tree::size_type size, const Operator &op,
                                                       const Allocator &alloc)
            : _size(size), _array_size(size << 1), _identity(op.identity()), _alloc(alloc), _op(op) {
        _lazy = std::allocator_traits<Allocator>::allocate(_alloc, _size);
        _lazy_op = plus<value_type>();
        _lazy_op_inv = plus<value_type>();
        _lazy_identity = plus<value_type>().identity();
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        for (size_type i = 1; i < _size; ++i) {
            std::construct_at(std::addressof(_lazy[i]), _lazy_identity);
        }
        for (size_type i = 1; i < _array_size; ++i) {
            std::construct_at(std::addressof(_tree[i]), _identity);
        }
        _is_built = false;
    }

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(InputIter first, InputIter last, const Operator &op,
                                                       const Allocator &alloc)
            : lazy_segment_tree(first, last, std::distance(first, last), op, alloc) {}

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(InputIter first, InputIter last,
                                                       lazy_segment_tree::size_type size_hint, const Operator &op,
                                                       const Allocator &alloc)
            : _size(size_hint), _array_size(_size << 1), _identity(op.identity()),
              _alloc(alloc), _op(op) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        _lazy = std::allocator_traits<Allocator>::allocate(_alloc, _size);
        _lazy_op = plus<value_type>();
        _lazy_op_inv = plus<value_type>();
        _lazy_identity = plus<value_type>().identity();
        for (size_type i = 1; i < _size; ++i) {
            std::construct_at(std::addressof(_lazy[i]), _lazy_identity);
        }
        std::uninitialized_copy(first, last, _tree + _size);
        build();
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(const lazy_segment_tree &other)
            : lazy_segment_tree(other, other.get_allocator()) {}

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(lazy_segment_tree &&other)
            : lazy_segment_tree(std::move(other), std::move(other.get_allocator())) {}

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(const lazy_segment_tree &other,
                                                       const std::type_identity_t <Allocator> &alloc)
            : _size(other._size), _array_size(other._array_size), _alloc(alloc), _is_built(other._is_built) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        _lazy = std::allocator_traits<Allocator>::allocate(_alloc, _size);
        _lazy_op = plus<value_type>();
        _lazy_op_inv = plus<value_type>();
        _lazy_identity = plus<value_type>().identity();
        std::uninitialized_copy(other._lazy, other._lazy + _size, _lazy);
        std::uninitialized_copy(other._tree, other._tree + _array_size, _tree);
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(lazy_segment_tree &&other,
                                                       const std::type_identity_t <Allocator> &alloc)
            : _size(other._size), _array_size(other._array_size), _alloc(alloc), _is_built(other._is_built) {
        _tree = other._tree;
        _lazy = other._lazy;
        _lazy_op = other._lazy_op;
        _lazy_op_inv = other._lazy_op_inv;
        _lazy_identity = other._lazy_identity;
        other._tree = nullptr;
        other._lazy = nullptr;
        other._is_built = false;
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::lazy_segment_tree(std::initializer_list<T> il, const Operator &op,
                                                       const Allocator &alloc)
            : lazy_segment_tree(il.begin(), il.end(), op, alloc) {}

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::~lazy_segment_tree() {
        for (size_type i = 1; i < _array_size; ++i) {
            std::destroy_at(std::addressof(_tree[i]));
        }
        for (size_type i = 1; i < _size; ++i) {
            std::destroy_at(std::addressof(_lazy[i]));
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _tree, _array_size);
        std::allocator_traits<Allocator>::deallocate(_alloc, _lazy, _size);
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator> &lazy_segment_tree<T, Allocator, Operator>::operator=(const lazy_segment_tree &other) {
        if (this != &other) {
            _size = other._size;
            _array_size = other._array_size;
            _identity = other._identity;
            _resize(_size);
            _alloc = other._alloc;
            _is_built = other._is_built;
            _lazy_op = other._lazy_op;
            _lazy_op_inv = other._lazy_op_inv;
            _lazy_identity = other._lazy_identity;
            std::uninitialized_copy(other._tree, other._tree + _array_size, _tree);
            std::uninitialized_copy(other._lazy, other._lazy + _size, _lazy);
        }
        return *this;
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator> &lazy_segment_tree<T, Allocator, Operator>::operator=(lazy_segment_tree &&other)
    noexcept (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
              std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &other) {
            clear();
            _size = other._size;
            _array_size = other._array_size;
            _alloc = std::move(other._alloc);
            _is_built = other._is_built;
            _tree = other._tree;
            _lazy = other._lazy;
            _lazy_op = other._lazy_op;
            _lazy_op_inv = other._lazy_op_inv;
            _lazy_identity = other._lazy_identity;
            other._tree = nullptr;
            other._lazy = nullptr;
            other._is_built = false;
        }
        return *this;
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator> &lazy_segment_tree<T, Allocator, Operator>::operator=(std::initializer_list<T> il) {
        assign(il);
        return *this;
    }

    template<class T, class Allocator, class Operator>
    lazy_segment_tree<T, Allocator, Operator>::allocator_type lazy_segment_tree<T, Allocator, Operator>::get_allocator() const noexcept {
        return _alloc;
    }

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    void lazy_segment_tree<T, Allocator, Operator>::assign(InputIter first, InputIter last) {
        if (_tree == nullptr) {
            _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
            _lazy = std::allocator_traits<Allocator>::allocate(_alloc, _size);
            _lazy_op = plus<value_type>();
            _lazy_op_inv = plus<value_type>();
            _lazy_identity = plus<value_type>().identity();
            _size = std::distance(first, last);
            _array_size = _size << 1;
        } else {
            _resize(std::distance(first, last));
        }
        for (size_type i = 1; i < _size; ++i) {
            std::construct_at(std::addressof(_lazy[i]), _lazy_identity);
        }
        std::uninitialized_copy(first, last, _tree + _size);
        build();
    }

    template<class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    template<class T, class Allocator, class Operator>
    bool lazy_segment_tree<T, Allocator, Operator>::empty() const {
        return _size == 0;
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type lazy_segment_tree<T, Allocator, Operator>::size() const {
        return _size;
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type lazy_segment_tree<T, Allocator, Operator>::tree_size() const {
        return _array_size;
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type
    lazy_segment_tree<T, Allocator, Operator>::resize(lazy_segment_tree::size_type size) {
        size_type ret = _resize(size);
        if (ret != _size) {
            for (size_type i = 1; i < _size; ++i) {
                std::construct_at(std::addressof(_lazy[i]), _lazy_identity);
            }
            for (size_type i = 1; i < _array_size; ++i) {
                std::construct_at(std::addressof(_tree[i]), _identity);
            }
        }
        return ret;
    }

    template<class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::build() {
        for (size_type i = _size - 1; i > 0; --i) {
            _tree[i] = _op(_tree[i << 1], _tree[i << 1 | 1]);
        }
        _is_built = true;
    } // build segment tree, didn't build lazy propagation

    template<class T, class Allocator, class Operator>
    bool lazy_segment_tree<T, Allocator, Operator>::is_built() const {
        return _is_built;
    }

    template<class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::update(lazy_segment_tree::size_type pos, const T &value) {
        pos += _size;
        for (size_type i = 63 - __builtin_clzll(pos); i > 0; --i) {
            _push(pos >> i, 1 >> i);
        }
        _apply(pos, value, 1);
        for (size_type i = 1; i <= 63 - __builtin_clzll(pos); ++i) {
            _pull(pos >> i);
        }
    }

    template <class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::update(size_type left, size_type right, const T& value) {
        left += _size;
        right += _size;
        for (size_type i = 63 - __builtin_clzll(left); i > 0; --i) {
            if (left >> i << i != left) {
                _push(left >> i, 1 >> i);
            }
            if (right >> i << i != right + 1) {
                _push(right >> i, 1 >> i);
            }
        }
        for (size_type i = 1, L = left, R = right; L <= R; L >>= 1, R >>= 1, i <<= 1) {
            if (L & 1) {
                _apply(L++, value, i);
            }
            if (~R & 1) {
                _apply(R--, value, i);
            }
        }
        for (;; left >>= 1) {
            if (left & 1) {
                break;
            }
        }
        for (;; right >>= 1) {
            if (~right & 1) {
                break;
            }
        }
        for (left >>= 1; left > 0; left >>= 1) {
            _pull(left);
        }
        for (right >>= 1; right > 0; right >>= 1) {
            _pull(right);
        }
    } // [left, right]

    template <class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::flush() const {
        size_type size = _size;
        while (size > 1) {
            const size_type count = _size / size;
            for (size_type i = 0; i < count; ++i) {
                _push(i + count, size);
            }
            size >>= 1;
        }
    }

    template <class T, class Allocator, class Operator>
    Operator lazy_segment_tree<T, Allocator, Operator>::get_range_operator() const {
        return _lazy_op;
    }

    template <class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::set_range_operator(std::function<T(T, T)> op,
                                                                       std::function<T(T, T)> inv, T identity) {
        flush();
        _lazy_identity = identity;
        _lazy_op = std::move(op);
        _lazy_op_inv = std::move(inv);
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type
    lazy_segment_tree<T, Allocator, Operator>::query(lazy_segment_tree::size_type pos) {
        pos += _size;
        for (size_type i = 63 - __builtin_clzll(pos); i > 0; --i) {
            _push(pos >> i, 1 >> i);
        }
        return _tree[pos];
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type
    lazy_segment_tree<T, Allocator, Operator>::query(lazy_segment_tree::size_type left, lazy_segment_tree::size_type right) {
        size_type Left = _identity, Right = _identity;
        left += _size;
        right += _size;
        for (size_type i = 63 - __builtin_clzll(left); i > 0; --i) {
            if (left >> i << i != left) {
                _push(left >> i, 1 >> i);
            }
            if (right >> i << i != right + 1) {
                _push(right >> i, 1 >> i);
            }
        }
        for (; left <= right; left >>= 1, right >>= 1) {
            if (left & 1) {
                Left = _op(Left, _tree[left++]);
            }
            if (~right & 1) {
                Right = _op(_tree[right--], Right);
            }
        }
        return _op(Left, Right);
    } // [left, right]

    template<class T, class Allocator, class Operator>
    std::span<const T> lazy_segment_tree<T, Allocator, Operator>::data() const {
        flush();
        return std::span<const T>(_tree + _size, _size);
    }

    template<class T, class Allocator, class Operator>
    void lazy_segment_tree<T, Allocator, Operator>::clear() {
        for (size_type i = 1; i < _array_size; ++i) {
            std::destroy_at(std::addressof(_tree[i]));
        }
        for (size_type i = 1; i < _size; ++i) {
            std::destroy_at(std::addressof(_lazy[i]));
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _tree, _array_size);
        std::allocator_traits<Allocator>::deallocate(_alloc, _lazy, _size);
        _size = 0;
        _array_size = 0;
        _tree = nullptr;
        _is_built = false;
    }

    template<class T, class Allocator, class Operator>
    typename lazy_segment_tree<T, Allocator, Operator>::size_type
    lazy_segment_tree<T, Allocator, Operator>::_resize(size_type size) {
        if (size == _size) {
            return _size;
        }
        clear();
        _size = size;
        _array_size = _size << 1;
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        _lazy = std::allocator_traits<Allocator>::allocate(_alloc, _size);
        _is_built = false;
        return _size;
    }
}