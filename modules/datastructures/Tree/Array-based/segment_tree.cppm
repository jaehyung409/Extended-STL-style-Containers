/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <span>
#include <functional>
#include <initializer_list>

export module segment_tree;

import basics;

namespace j {
    export template<class T,
                    class Allocator = std::allocator<T>,
                    class Operator = plus<T>> // operator (monoid)
    class segment_tree {
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
        pointer _tree;
        bool _is_built = false;     // built flag

        size_type _resize(size_type size);

    public:
        segment_tree() : segment_tree(0, Operator()) {}
        explicit segment_tree(size_type size) : segment_tree(size, Operator()) {}
        explicit segment_tree(size_type size, const Operator &op) : segment_tree(size, op, Allocator()) {}
        segment_tree(size_type size, const Operator &op, const Allocator &alloc);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        segment_tree(InputIter first, InputIter last,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        segment_tree(InputIter first, InputIter last, size_type size_hint,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        //template <container-compatible-range<T> R>
        //segment_tree(from_range_t, R&& range, const Operator &op = Operator(), const Allocator &alloc = Allocator());
        segment_tree(const segment_tree &other);
        segment_tree(segment_tree &&other);
        segment_tree(const segment_tree &other, const std::type_identity_t<Allocator> &alloc);
        segment_tree(segment_tree &&other, const std::type_identity_t<Allocator> &alloc);
        segment_tree(std::initializer_list<T> il,
                     const Operator &op = Operator(), const Allocator &alloc = Allocator());
        ~segment_tree();
        segment_tree &operator=(const segment_tree &other);
        segment_tree &operator=(segment_tree &&other) noexcept(
                std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
                std::allocator_traits<Allocator>::is_always_equal::value);
        segment_tree &operator=(std::initializer_list<T> il);
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

        size_type query(size_type pos);
        size_type query(size_type left, size_type right);
        std::span<const T> data() const;
        void swap(segment_tree& x) noexcept(
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
    segment_tree<T, Allocator, Operator>::segment_tree(segment_tree::size_type size, const Operator &op,
                                                       const Allocator &alloc)
    : _size(size), _array_size(size << 1), _identity(op.identity()), _alloc(alloc), _op(op) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        for (size_type i = 1; i < _array_size; ++i) {
            std::construct_at(std::addressof(_tree[i]), _identity);
        }
        _is_built = false;
    }

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    segment_tree<T, Allocator, Operator>::segment_tree(InputIter first, InputIter last, const Operator &op,
                                                       const Allocator &alloc)
    : segment_tree(first, last, std::distance(first, last), op, alloc) {}

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    segment_tree<T, Allocator, Operator>::segment_tree(InputIter first, InputIter last,
                                                       segment_tree::size_type size_hint, const Operator &op,
                                                       const Allocator &alloc)
    : _size(size_hint), _array_size(_size << 1), _identity(op.identity()),
      _alloc(alloc), _op(op) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        std::uninitialized_copy(first, last, _tree + _size);
        build();
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::segment_tree(const segment_tree &other)
    : segment_tree(other, other.get_allocator()) {}

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::segment_tree(segment_tree &&other)
    : segment_tree(std::move(other), std::move(other.get_allocator())) {
        other._tree = nullptr;
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::segment_tree(const segment_tree &other,
                                                       const std::type_identity_t <Allocator> &alloc)
    : _size(other._size), _array_size(other._array_size), _alloc(alloc), _is_built(other._is_built) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        std::uninitialized_copy(other._tree, other._tree + _array_size, _tree);
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::segment_tree(segment_tree &&other,
                                                       const std::type_identity_t <Allocator> &alloc)
    : _size(other._size), _array_size(other._array_size), _alloc(alloc), _is_built(other._is_built) {
        _tree = other._tree;
        other._tree = nullptr;
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::segment_tree(std::initializer_list<T> il, const Operator &op,
                                                       const Allocator &alloc)
    : segment_tree(il.begin(), il.end(), op, alloc) {}

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator>::~segment_tree() {
        clear();
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator> &segment_tree<T, Allocator, Operator>::operator=(const segment_tree &other) {
        if (this != &other) {
            _size = other._size;
            _array_size = other._array_size;
            _identity = other._identity;
            resize(_size);
            _alloc = other._alloc;
            _is_built = other._is_built;
            std::uninitialized_copy(other._tree, other._tree + _array_size, _tree);
        }
        return *this;
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator> &segment_tree<T, Allocator, Operator>::operator=(segment_tree &&other)
    noexcept (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
              std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &other) {
            clear();
            std::allocator_traits<Allocator>::deallocate(_alloc, _tree, _array_size);
            _size = other._size;
            _array_size = other._array_size;
            _alloc = std::move(other._alloc);
            _is_built = other._is_built;
            _tree = other._tree;
            other.clear();
        }
        return *this;
    }

    template<class T, class Allocator, class Operator>
    segment_tree<T, Allocator, Operator> &segment_tree<T, Allocator, Operator>::operator=(std::initializer_list<T> il) {
        assign(il);
        return *this;
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::allocator_type segment_tree<T, Allocator, Operator>::get_allocator() const noexcept {
        return _alloc;
    }

    template<class T, class Allocator, class Operator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    void segment_tree<T, Allocator, Operator>::assign(InputIter first, InputIter last) {
        if (_tree == nullptr) {
            _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
            _size = std::distance(first, last);
            _array_size = _size << 1;
        } else {
            _resize(std::distance(first, last));
        }
        std::uninitialized_copy(first, last, _tree + _size);
        build();
    }

    template<class T, class Allocator, class Operator>
    void segment_tree<T, Allocator, Operator>::assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    template<class T, class Allocator, class Operator>
    bool segment_tree<T, Allocator, Operator>::empty() const {
        return _size == 0;
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type segment_tree<T, Allocator, Operator>::size() const {
        return _size;
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type segment_tree<T, Allocator, Operator>::tree_size() const {
        return _array_size;
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type
    segment_tree<T, Allocator, Operator>::resize(segment_tree::size_type size) {
        const size_type ret = _resize(size);
        if (ret != _size) {
            for (size_type i = 1; i < _array_size; ++i) {
                std::construct_at(std::addressof(_tree[i]), _identity);
            }
        }
        return ret;
    }

    template<class T, class Allocator, class Operator>
    void segment_tree<T, Allocator, Operator>::build() {
        for (size_type i = _size - 1; i > 0; --i) {
            _tree[i] = _op(_tree[i << 1], _tree[i << 1 | 1]);
        }
        _is_built = true;
    }

    template<class T, class Allocator, class Operator>
    bool segment_tree<T, Allocator, Operator>::is_built() const {
        return _is_built;
    }

    template<class T, class Allocator, class Operator>
    void segment_tree<T, Allocator, Operator>::update(segment_tree::size_type pos, const T &value) {
        for (_tree[pos += _size] = value; pos > 1; pos >>= 1) {
            _tree[pos >> 1] = _op(_tree[pos], _tree[pos ^ 1]);
        }
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type
    segment_tree<T, Allocator, Operator>::query(segment_tree::size_type pos) {
        return _tree[pos + _size];
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type
    segment_tree<T, Allocator, Operator>::query(segment_tree::size_type left, segment_tree::size_type right) {
        size_type result = _identity;
        for (left += _size, right += _size; left < right; left >>= 1, right >>= 1) {
            if (left & 1) {
                result = _op(result, _tree[left++]);
            }
            if (right & 1) {
                result = _op(result, _tree[--right]);
            }
        }
        return result;
    } // [left, right)

    template<class T, class Allocator, class Operator>
    std::span<const T> segment_tree<T, Allocator, Operator>::data() const {
        return std::span<const T>(_tree + _size, _size);
    }

    template<class T, class Allocator, class Operator>
    void segment_tree<T, Allocator, Operator>::clear() {
        for (size_type i = 0; i < _array_size; ++i) {
            std::destroy_at(std::addressof(_tree[i]));
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _tree, _array_size);
        _size = 0;
        _array_size = 0;
        _identity = Operator().identity();
        _tree = nullptr;
        _is_built = false;
    }

    template<class T, class Allocator, class Operator>
    typename segment_tree<T, Allocator, Operator>::size_type
    segment_tree<T, Allocator, Operator>::_resize(size_type size) {
        if (size == _size) {
            return _size;
        }
        clear();
        _size = size;
        _array_size = _size << 1;
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _array_size);
        _is_built = false;
        return _size;
    }
}