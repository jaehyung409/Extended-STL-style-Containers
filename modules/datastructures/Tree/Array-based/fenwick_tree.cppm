/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>
#include <memory>
#include <span>
#include <type_traits>
#include <initializer_list>

export module fenwick_tree;

import array;
import vector;
import basics;

namespace j {
    export template<class T,
                    std::size_t Extent = std::dynamic_extent,
                    class Allocator = std::allocator<T>>
    class fenwick_tree {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using allocator_type = Allocator;

    private:
        pointer _tree;
        plus<T> _op = plus<T>();
        Allocator _alloc;
        size_type _dynamic_size; // real size is _dynamic_size + 1 (1-based index)
        value_type _identity = _op.identity();
        void _init_update() {
            for (size_type i = 1; i <= _dynamic_size; i++) {
                size_type parent = i + (i & -i);
                if (parent <= _dynamic_size) {
                    _tree[parent] = _op(_tree[parent], _tree[i]);
                }
            }
        }

    public:
        fenwick_tree() requires (Extent != std::dynamic_extent)
            : fenwick_tree(Allocator()) {}
        fenwick_tree(const Allocator& alloc)
        requires (Extent != std::dynamic_extent);

        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        fenwick_tree(InputIter first, InputIter last, const Allocator& alloc = Allocator());

        template <class Container>
        requires (std::ranges::sized_range<Container> &&
                  std::convertible_to<std::ranges::range_value_t<Container>, T>)
        fenwick_tree(const Container& container, const Allocator& alloc = Allocator());

        template <class Container>
        requires (std::ranges::sized_range<Container> &&
                  std::convertible_to<std::ranges::range_value_t<Container>, T>)
        fenwick_tree(Container&& container, const Allocator& alloc = Allocator());

        fenwick_tree(std::initializer_list<T> il, const Allocator& alloc = Allocator());

        //template<container-compatible-range<T> R>
        //fenwick_tree(from_range_t, R&& range, const Allocator& = Allocator());

        fenwick_tree(const fenwick_tree& other);
        fenwick_tree(fenwick_tree&& other) noexcept(
                std::allocator_traits<Allocator>::is_always_equal::value);
        ~fenwick_tree();
        allocator_type get_allocator() const noexcept;

        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void assign(InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //void assign_range(R&& rg);
        void assign(std::initializer_list<T> il);

        size_type size() const;
        value_type query(size_type index) const;
        value_type query(size_type left, size_type right) const;
        void update(size_type index, const value_type& value);
        void swap(fenwick_tree& other) noexcept(
                std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                std::allocator_traits<Allocator>::is_always_equal::value &&
                std::is_nothrow_swappable_v<Allocator>) {
            using std::swap;
            swap(_tree, other._tree);
            swap(_alloc, other._alloc);
            swap(_dynamic_size, other._dynamic_size);
            swap(_identity, other._identity);
        }
        void clear() noexcept;
    };

    template <class Container>
    fenwick_tree(Container&&)
        -> fenwick_tree<typename std::ranges::range_value_t<Container>>;

    template <class T, std::size_t N>
    fenwick_tree(array<T, N>&) -> fenwick_tree<T, N>;

    template <class T, std::size_t N>
    fenwick_tree(const array<T, N>&) -> fenwick_tree<T, N>;

    template <class InputIter, class Sentinel>
    fenwick_tree(InputIter, Sentinel)
        -> fenwick_tree<std::iter_value_t<InputIter>>;

    template <class T>
    fenwick_tree(std::initializer_list<T>) -> fenwick_tree<T>;
}

namespace j {
    template<class T, std::size_t Extent, class Allocator>
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(const Allocator &alloc)
    requires (Extent != std::dynamic_extent)
    : _alloc(alloc), _dynamic_size(Extent) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, Extent + 1);
        for (size_type i = 1; i <= Extent; ++i) {
            std::construct_at(std::addressof(_tree[i]), _identity);
        }
    }

    template<class T, std::size_t Extent, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(InputIter first, InputIter last, const Allocator &alloc)
    : _alloc(alloc), _dynamic_size(std::distance(first, last)) {
        if (Extent != std::dynamic_extent && Extent != _dynamic_size) {
            throw std::invalid_argument("fenwick_tree: size mismatch");
        }
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _dynamic_size + 1);
        for (size_type i = 1; i <= _dynamic_size; ++i) {
            std::construct_at(std::addressof(_tree[i]), *first++);
        }
        _init_update();
    }

    template<class T, std::size_t Extent, class Allocator>
    template<class Container>
    requires (std::ranges::sized_range<Container> &&
              std::convertible_to<std::ranges::range_value_t<Container>, T>)
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(const Container &container, const Allocator &alloc)
    : fenwick_tree(std::begin(container), std::end(container), alloc) {}

    template<class T, std::size_t Extent, class Allocator>
    template<class Container>
    requires (std::ranges::sized_range<Container> &&
              std::convertible_to<std::ranges::range_value_t<Container>, T>)
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(Container &&container, const Allocator &alloc)
    : _alloc(alloc), _dynamic_size(container.size()) {
        if (Extent != std::dynamic_extent && Extent != _dynamic_size) {
            throw std::invalid_argument("fenwick_tree: size mismatch");
        }
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _dynamic_size + 1);
        std::uninitialized_move(container.begin(), container.end(), _tree + 1);
        _init_update();
    }


    template<class T, std::size_t Extent, class Allocator>
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(std::initializer_list<T> il, const Allocator &alloc)
    : fenwick_tree(il.begin(), il.end(), alloc) {}

    template<class T, std::size_t Extent, class Allocator>
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(const fenwick_tree &other)
    : _alloc(other._alloc), _dynamic_size(other._dynamic_size) {
        _tree = std::allocator_traits<Allocator>::allocate(_alloc, _dynamic_size + 1);
        for (size_type i = 1; i <= _dynamic_size; ++i) {
            std::construct_at(std::addressof(_tree[i]), other._tree[i]);
        }
    }

    template<class T, std::size_t Extent, class Allocator>
    fenwick_tree<T, Extent, Allocator>::fenwick_tree(fenwick_tree &&other)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
    : _tree(other._tree), _alloc(std::move(other._alloc)), _dynamic_size(std::move(other._dynamic_size)) {
        other._tree = nullptr;
    }

    template<class T, std::size_t Extent, class Allocator>
    fenwick_tree<T, Extent, Allocator>::~fenwick_tree() {
        for (size_type i = 1; i <= _dynamic_size; ++i) {
            std::destroy_at(std::addressof(_tree[i]));
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _tree, _dynamic_size + 1);
    }

    template<class T, std::size_t Extent, class Allocator>
    typename fenwick_tree<T, Extent, Allocator>::allocator_type
    fenwick_tree<T, Extent, Allocator>::get_allocator() const noexcept {
        return _alloc;
    }

    template<class T, std::size_t Extent, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    void fenwick_tree<T, Extent, Allocator>::assign(InputIter first, InputIter last) {
        if (std::distance(first, last) != _dynamic_size) {
            throw std::invalid_argument("fenwick_tree::assign: size mismatch");
        }
        if (_tree == nullptr) {
            _tree = std::allocator_traits<Allocator>::allocate(_alloc, _dynamic_size + 1);
            for (size_type i = 1; i <= _dynamic_size; ++i) {
                std::construct_at(std::addressof(_tree[i]), _identity);
            }
        } else {
            clear();
        }
        std::copy(first, last, _tree + 1);
        _init_update();
    }

    template<class T, std::size_t Extent, class Allocator>
    void fenwick_tree<T, Extent, Allocator>::assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    template<class T, std::size_t Extent, class Allocator>
    typename fenwick_tree<T, Extent, Allocator>::size_type
    fenwick_tree<T, Extent, Allocator>::size() const {
        return _dynamic_size;
    }

    template<class T, std::size_t Extent, class Allocator>
    typename fenwick_tree<T, Extent, Allocator>::value_type
    fenwick_tree<T, Extent, Allocator>::query(fenwick_tree::size_type index) const {
        value_type result = _identity;
        for (size_type i = index; i > 0; i -= i & -i) {
            result = _op(result, _tree[i]);
        }
        return result;
    }

    template<class T, std::size_t Extent, class Allocator>
    typename fenwick_tree<T, Extent, Allocator>::value_type
    fenwick_tree<T, Extent, Allocator>::query(fenwick_tree::size_type left, fenwick_tree::size_type right) const {
        return query(right) - query(left - 1);
    }

    template<class T, std::size_t Extent, class Allocator>
    void fenwick_tree<T, Extent, Allocator>::update(fenwick_tree::size_type index, const value_type &value) {
        for (size_type i = index; i <= _dynamic_size; i += i & -i) {
            _tree[i] = _op(_tree[i], value);
        }
    }

    template<class T, std::size_t Extent, class Allocator>
    void fenwick_tree<T, Extent, Allocator>::clear() noexcept {
        std::fill(_tree + 1, _tree + _dynamic_size + 1, _identity);
    }
}