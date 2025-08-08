/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 */

module;
#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>

export module j.vector;

import j.range_basics;

namespace j {
// not-yet specialization for bool ...
export template <class T, class Allocator = std::allocator<T>> class vector {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    class iterator;
    class const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    size_type _size;
    size_type _capacity;
    pointer _data;
    allocator_type _alloc;

  public:
    // constructor/copy/destructor
    constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
    constexpr explicit vector(const Allocator &alloc) noexcept;
    constexpr explicit vector(size_type n, const Allocator &alloc = Allocator());
    constexpr vector(size_type n, const T &value, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter>
    constexpr vector(InputIter first, InputIter last, const Allocator &alloc = Allocator());
    template <container_compatible_range<T> R>
    constexpr vector(std::ranges::from_range_t, R &&range, const Allocator & = Allocator()) = delete;
    constexpr vector(const vector &x);
    constexpr vector(vector &&x) noexcept;
    constexpr vector(const vector &x, const std::type_identity_t<Allocator> &alloc);
    constexpr vector(vector &&x, const std::type_identity_t<Allocator> &alloc);
    constexpr vector(std::initializer_list<T> il, const Allocator &alloc = Allocator());
    constexpr ~vector();

    constexpr vector &operator=(const vector &x);
    constexpr vector &
    operator=(vector &&x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
                                   std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr vector &operator=(std::initializer_list<T> il);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    constexpr void assign(InputIter first, InputIter last);
    template <container_compatible_range<T> R> constexpr void assign_range(R &&rg) = delete;
    constexpr void assign(size_type n, const T &u);
    constexpr void assign(std::initializer_list<T> il);
    constexpr allocator_type get_allocator() const noexcept;

    // iterators
    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;

    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept;
    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;
    constexpr void resize(size_type sz);
    constexpr void resize(size_type sz, const T &c);
    constexpr void reserve(size_type n);
    constexpr void shrink_to_fit();

    // element access
    constexpr reference operator[](size_type n);
    constexpr const_reference operator[](size_type n) const;
    constexpr reference at(size_type n);
    constexpr const_reference at(size_type n) const;
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;

    // data access
    constexpr T *data() noexcept;
    constexpr const T *data() const noexcept;

    // modifiers
    template <class... Args> constexpr reference emplace_back(Args &&...args);
    constexpr void push_back(const T &x);
    constexpr void push_back(T &&x);
    template <container_compatible_range<T> R> constexpr void append_range(R &&rg) = delete;
    constexpr void pop_back();

    template <class... Args> constexpr iterator emplace(const_iterator position, Args &&...args);
    constexpr iterator insert(const_iterator position, const T &x);
    constexpr iterator insert(const_iterator position, T &&x);
    constexpr iterator insert(const_iterator position, size_type n, const T &x);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    constexpr iterator insert(const_iterator position, InputIter first, InputIter last);
    template <container_compatible_range<T> R> constexpr void insert_range(const_iterator position, R &&rg) = delete;
    constexpr iterator insert(const_iterator position, std::initializer_list<T> il);
    constexpr iterator erase(const_iterator position);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr void swap(vector &x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                                            std::allocator_traits<Allocator>::is_always_equal::value);
    constexpr void clear() noexcept;
};

template <class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
vector(InputIter, InputIter, Allocator = Allocator())
    -> vector<typename std::iterator_traits<InputIter>::value_type, Allocator>;

template <std::ranges::input_range R, class Allocator = std::allocator<std::ranges::range_value_t<R>>>
vector(std::ranges::from_range_t, R &&, Allocator = Allocator()) -> vector<std::ranges::range_value_t<R>, Allocator>;

export template <class T, class Allocator>
constexpr bool operator==(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class T, class Allocator>
constexpr auto operator<=>(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class T, class Allocator>
constexpr void swap(vector<T, Allocator> &x, vector<T, Allocator> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class T, class Allocator, class U>
constexpr vector<T, Allocator>::size_type erase(vector<T, Allocator> &c, const U &value) {
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}

template <class T, class Allocator> class vector<T, Allocator>::iterator {
    friend vector;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename vector::value_type;
    using difference_type = typename vector::difference_type;
    using pointer = typename vector::pointer;
    using reference = typename vector::reference;

  private:
    pointer _ptr;

  public:
    explicit iterator(pointer ptr = nullptr) : _ptr(ptr) {}
    iterator(const iterator &other) = default;

    reference operator*() {
        return *_ptr;
    }
    pointer operator->() {
        return &(*_ptr);
    }

    iterator &operator++() {
        ++_ptr;
        return *this;
    }

    iterator operator++(int) {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    iterator &operator--() {
        --_ptr;
        return *this;
    }

    iterator operator--(int) {
        iterator temp = *this;
        --(*this);
        return temp;
    }

    iterator &operator+=(difference_type n) {
        _ptr += n;
        return *this;
    }

    iterator operator+(difference_type n) const {
        iterator temp = *this;
        return temp += n;
    }

    friend iterator operator+(difference_type n, const iterator &it) {
        return it + n;
    }

    iterator &operator-=(difference_type n) {
        _ptr -= n;
        return *this;
    }

    iterator operator-(difference_type n) const {
        iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const iterator &other) const {
        return _ptr - other._ptr;
    }

    reference operator[](difference_type n) {
        return *(*this + n);
    }

    bool operator==(const iterator &other) const {
        return _ptr == other._ptr;
    }
    auto operator<=>(const iterator &other) const {
        return _ptr <=> other._ptr;
    }
    operator const_iterator() const {
        return const_iterator(_ptr);
    }
};

template <class T, class Allocator> class vector<T, Allocator>::const_iterator {
    friend vector;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename vector::value_type;
    using difference_type = typename vector::difference_type;
    using pointer = typename vector::const_pointer;
    using reference = typename vector::const_reference;
    using const_pointer = typename vector::const_pointer;
    using const_reference = typename vector::const_reference;

  private:
    pointer _ptr;

  public:
    explicit const_iterator(pointer ptr = nullptr) : _ptr(ptr) {}
    explicit const_iterator(const iterator &other) : _ptr(other._ptr) {}
    const_iterator(const const_iterator &other) = default;
    const_iterator &operator=(const const_iterator &other) = default;

    const_reference operator*() const {
        return *_ptr;
    }
    const_pointer operator->() const {
        return &(*_ptr);
    }

    const_iterator &operator++() {
        ++_ptr;
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    const_iterator &operator--() {
        --_ptr;
        return *this;
    }

    const_iterator operator--(int) {
        const_iterator temp = *this;
        --(*this);
        return temp;
    }

    const_iterator &operator+=(difference_type n) {
        _ptr += n;
        return *this;
    }

    const_iterator operator+(difference_type n) const {
        const_iterator temp = *this;
        return temp += n;
    }

    const_iterator &operator-=(difference_type n) {
        _ptr -= n;
        return *this;
    }

    const_iterator operator-(difference_type n) const {
        const_iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const const_iterator &other) const {
        return _ptr - other._ptr;
    }

    const_reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const const_iterator &other) const {
        return _ptr == other._ptr;
    }
    auto operator<=>(const const_iterator &other) const {
        return _ptr <=> other._ptr;
    }
};

} // namespace j

namespace j {

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const Allocator &alloc) noexcept
    : _size(0), _capacity(0), _data(nullptr),
      _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type n, const Allocator &alloc)
    : vector(n, T(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type n, const T &value, const Allocator &alloc)
    : _capacity(n), _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    if (n == 0) {
        _data = nullptr;
        _size = 0;
        return;
    }
    _data = std::allocator_traits<Allocator>::allocate(_alloc, n);
    try {
        std::uninitialized_fill_n(_data, n, value);
        _size = n;
    } catch (...) {
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, n);
        _size = _capacity = 0;
        _data = nullptr;
        throw;
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
constexpr vector<T, Allocator>::vector(InputIter first, InputIter last, const Allocator &alloc)
    : vector(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    if constexpr (std::forward_iterator<InputIter>) {
        auto dist = std::distance(first, last);
        if (dist == 0) {
            return;
        }
        _data = std::allocator_traits<Allocator>::allocate(_alloc, dist);
        _capacity = dist;
        try {
            if constexpr (std::is_trivially_copy_constructible_v<T> && std::contiguous_iterator<InputIter>) {
                std::memcpy(_data, std::to_address(first), dist * sizeof(T));
            } else {
                std::uninitialized_copy(first, last, _data);
            }
            _size = _capacity;
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, dist);
            _size = _capacity = 0;
            _data = nullptr;
            throw;
        }
    } else {
        for (; first != last; ++first) {
            this->emplace_back(*first);
        }
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector &x)
    : vector(x.begin(), x.end(), std::allocator_traits<Allocator>::select_on_container_copy_construction(x._alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(vector &&x) noexcept
    : _size(x._size), _capacity(x._capacity), _data(x._data), _alloc(std::move(x._alloc)) {
    x._data = nullptr;
    x._size = x._capacity = 0;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector &x, const std::type_identity_t<Allocator> &alloc)
    : vector(x.begin(), x.end(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(vector &&x, const std::type_identity_t<Allocator> &alloc)
    : _capacity(x._capacity), _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    if (alloc == x._alloc) {
        _data = x._data;
        _size = x._size;
        x._data = nullptr;
        x._size = x._capacity = 0;
    } else {
        _data = std::allocator_traits<Allocator>::allocate(_alloc, x.size());
        try {
            if constexpr (std::is_trivially_move_constructible_v<T>) {
                std::memcpy(_data, x._data, x.size() * sizeof(T));
            } else {
                std::uninitialized_move(x._data, x._data + x._size, _data);
            }
            _size = x._size;
            x._data = nullptr;
            x._size = x._capacity = 0;
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, x.size());
            _data = nullptr;
            _size = _capacity = 0;
            throw;
        }
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(std::initializer_list<T> il, const Allocator &alloc)
    : vector(il.begin(), il.end(), alloc) {}

template <class T, class Allocator> constexpr vector<T, Allocator>::~vector() {
    clear();
    std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
}

template <class T, class Allocator> constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &x) {
    if (this != std::addressof(x)) {
        if (x.size() > _capacity) {
            vector<T, Allocator> tmp(x);
            swap(tmp);
        } else {
            clear();
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(_data, x._data, x.size() * sizeof(T));
            } else {
                std::uninitialized_copy(x._data, x._data + x._size, _data);
            }
            _size = x._size;
        }
    }
    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&x) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this != std::addressof(x)) {
        clear();
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            _alloc = std::move(x._alloc);
            _data = x._data;
            _size = x._size;
            _capacity = x._capacity;

            x._data = nullptr;
            x._size = x._capacity = 0;
        } else {
            if (_alloc == x._alloc) {
                _data = x._data;
                _size = x._size;
                _capacity = x._capacity;
                x._data = nullptr;
                x._size = x._capacity = 0;
            } else {
                _data = std::allocator_traits<Allocator>::allocate(_alloc, x.size());
                try {
                    if constexpr (std::is_trivially_move_constructible_v<T>) {
                        std::memmove(_data, x._data, x.size() * sizeof(T));
                    } else {
                        std::uninitialized_move(x._data, x._data + x._size, _data);
                    }
                    _size = _capacity = x._size;
                } catch (...) {
                    std::allocator_traits<Allocator>::deallocate(_alloc, _data, x.size());
                    _data = nullptr;
                    _size = _capacity = 0;
                    throw;
                }
                std::allocator_traits<Allocator>::deallocate(x._alloc, x._data, x._capacity);
                x._data = nullptr;
                x._size = x._capacity = 0;
            }
        }
    }
    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(std::initializer_list<T> il) {
    clear();
    if (_capacity < il.size()) {
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = std::allocator_traits<Allocator>::allocate(_alloc, il.size());
        _capacity = il.size();
    }
    _size = il.size();
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(_data, il.begin(), il.size() * sizeof(T));
    } else {
        std::uninitialized_copy(il.begin(), il.end(), _data);
    }
    return *this;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
constexpr void vector<T, Allocator>::assign(InputIter first, InputIter last) {
    if constexpr (std::forward_iterator<InputIter>) {
        auto dist = std::distance(first, last);
        if (dist == 0) {
            clear();
            return;
        }
        if (dist > _capacity) {
            clear();
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = std::allocator_traits<Allocator>::allocate(_alloc, dist);
            std::uninitialized_copy(first, last, _data);
            _size = _capacity = dist;
        } else {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(_data, std::to_address(first), dist * sizeof(T));
            } else {
                if (!std::is_trivially_destructible_v<T>) {
                    std::destroy(_data, _data + _size);
                }
                std::uninitialized_copy(first, last, _data);
            }
            _size = dist;
        }
    } else {
        clear();
        for (; first != last; ++first) {
            this->emplace_back(*first);
        }
    }
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::assign(size_type n, const T &u) {
    if (n > _capacity) {
        clear();
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = std::allocator_traits<Allocator>::allocate(_alloc, n);
        _size = _capacity = n;
        std::uninitialized_fill_n(_data, n, u);
    } else {
        if constexpr (std::is_trivially_destructible_v<T>) {
            std::fill_n(_data, n, u);
        } else {
            std::destroy(_data, _data + _size);
            std::uninitialized_fill_n(_data, n, u);
        }
        _size = n;
    }
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::assign(std::initializer_list<T> il) {
    assign(il.begin(), il.end());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const noexcept {
    return _alloc;
}

template <class T, class Allocator> constexpr vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
    return iterator(_data);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept {
    return const_iterator(_data);
}

template <class T, class Allocator> constexpr vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
    return iterator(_data + _size);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept {
    return const_iterator(_data + _size);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const noexcept {
    return const_iterator(_data);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept {
    return const_iterator(_data + _size);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class T, class Allocator> constexpr bool vector<T, Allocator>::empty() const noexcept {
    return _size == 0;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept {
    return _size;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept {
    return std::allocator_traits<Allocator>::max_size(_alloc);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept {
    return _capacity;
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::resize(size_type sz) {
    resize(sz, T());
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::resize(size_type sz, const T &c) {
    if (sz < _size) {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data + sz, _data + _size);
        }
    } else if (sz > _size) {
        if (sz > _capacity) {
            reserve(std::max(sz, _capacity * 2));
        }
        std::uninitialized_fill_n(_data + _size, sz - _size, c);
    }
    _size = sz;
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::reserve(size_type n) {
    if (n > _capacity) {
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, n);
        try {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memmove(new_data, _data, _size * sizeof(T));
            } else {
                std::uninitialized_move(_data, _data + _size, new_data);
            }
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, _capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data, _data + _size);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = new_data;
        _capacity = n;
    }
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::shrink_to_fit() {
    if (_size < _capacity) {
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, _size);
        try {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memmove(new_data, _data, _size * sizeof(T));
            } else {
                std::uninitialized_move(_data, _data + _size, new_data);
            }
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, _size);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data, _data + _size);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = new_data;
        _capacity = _size;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type n) {
    return _data[n];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type n) const {
    return _data[n];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n) {
    if (n >= _size) {
        throw std::out_of_range("vector::at() : index is out of range");
    }
    return _data[n];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const {
    if (n >= _size) {
        throw std::out_of_range("vector::at() : index is out of range");
    }
    return _data[n];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::front() {
    return _data[0];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
    return _data[0];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::back() {
    return _data[_size - 1];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
    return _data[_size - 1];
}

template <class T, class Allocator> constexpr T *vector<T, Allocator>::data() noexcept {
    return _data;
}

template <class T, class Allocator> constexpr const T *vector<T, Allocator>::data() const noexcept {
    return _data;
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&...args) {
    if (_size == _capacity) {
        reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    std::construct_at(std::addressof(_data[_size]), std::forward<Args>(args)...);
    return _data[_size++];
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::push_back(const T &x) {
    emplace_back(x);
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::push_back(T &&x) {
    emplace_back(std::move(x));
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::pop_back() {
    --_size;
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::destroy_at(std::addressof(_data[_size]));
    }
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator position, Args &&...args) {
    const difference_type offset = position - begin();
    if (_size == _capacity) {
        const size_type new_capacity = _capacity == 0 ? 1 : _capacity * 2;
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_data, _data + offset, new_data);
            std::construct_at(std::addressof(new_data[offset]), std::forward<Args>(args)...);
            std::uninitialized_move(_data + offset, _data + _size, new_data + offset + 1);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data, _data + _size);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = new_data;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_data + offset + 1, _data + offset, (_size - offset) * sizeof(T));
            std::construct_at(std::addressof(_data[offset]), std::forward<Args>(args)...);
        } else {
            std::construct_at(end(), std::move(_data[_size - 1]));
            std::move_backward(_data + offset, _data + _size - 1, _data + _size);
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy_at(std::addressof(_data[offset]));
            }
            std::construct_at(std::addressof(_data[offset]), std::forward<Args>(args)...);
        }
    }
    ++_size;
    return iterator(_data + offset);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, const T &x) {
    return emplace(position, x);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, T &&x) {
    return emplace(position, std::move(x));
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, size_type n,
                                                                      const T &x) {
    const difference_type offset = position - begin();
    if (n == 0) {
        return iterator(_data + offset);
    }

    if (_size + n > _capacity) {
        const size_type new_capacity = _capacity == 0 ? std::max(1ul, n) : std::max(_capacity * 2, _size + n);
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_data, _data + offset, new_data);
            std::uninitialized_fill_n(new_data + offset, n, x);
            std::uninitialized_move(_data + offset, _data + _size, new_data + offset + n);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data, _data + _size);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = new_data;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_data + offset + n, _data + offset, (_size - offset) * sizeof(T));
            std::uninitialized_fill_n(_data + offset, n, x);
        } else {
            std::uninitialized_move(_data + _size - std::min(n, _size - offset), _data + _size,
                                    _data + _size + n - std::min(n, _size - offset));
            if (_size - offset > n) {
                std::move_backward(_data + offset, _data + _size - n, _data + _size);
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(_data + offset, _data + offset + std::min(n, _size - offset));
            }
            std::uninitialized_fill_n(_data + offset, n, x);
        }
    }
    _size += n;
    return iterator(_data + offset);
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, InputIter first,
                                                                      InputIter last) {
    const difference_type offset = position - begin();

    size_type dist;
    if constexpr (std::forward_iterator<InputIter>) {
        dist = std::distance(first, last);
    } else {
        vector<T, Allocator> buffer = vector(first, last, _alloc);
        dist = buffer.size();
        first = buffer.begin();
        last = buffer.end();
    }

    if (dist == 0) {
        return iterator(_data + offset);
    }

    if (_size + dist > _capacity) {
        const size_type new_capacity = _capacity == 0 ? std::max(1ul, dist) : std::max(_capacity * 2, _size + dist);
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_data, _data + offset, new_data);
            std::uninitialized_copy(first, last, new_data + offset);
            std::uninitialized_move(_data + offset, _data + _size, new_data + offset + dist);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data, _data + _size);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
        _data = new_data;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_data + offset + dist, _data + offset, (_size - offset) * sizeof(T));
            if constexpr (std::contiguous_iterator<InputIter>) {
                std::memcpy(_data + offset, std::to_address(first), dist * sizeof(T));
            } else {
                std::copy(first, last, _data + offset);
            }
        } else {
            std::uninitialized_move(_data + _size - std::min(dist, _size - offset), _data + _size,
                                    _data + _size + dist - std::min(dist, _size - offset));
            if (_size - offset > dist) {
                std::move_backward(_data + offset, _data + _size - dist, _data + _size);
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(_data + offset, _data + offset + std::min(dist, _size - offset));
            }
            std::uninitialized_copy(first, last, _data + offset);
        }
    }
    _size += dist;
    return iterator(_data + offset);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
                                                                      std::initializer_list<T> il) {
    return insert(position, il.begin(), il.end());
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator position) {
    const difference_type offset = position - begin();
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memmove(_data + offset, _data + offset + 1, (_size - offset - 1) * sizeof(T));
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy_at(std::addressof(_data[offset]));
        }
        std::move(begin() + offset + 1, end(), begin() + offset);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy_at(std::addressof(_data[_size - 1]));
        }
    }
    --_size;
    return iterator(_data + offset);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
    const difference_type offset = first - begin();
    const difference_type len = last - first;
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memmove(_data + offset, _data + offset + len, (_size - offset - len) * sizeof(T));
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data + offset, _data + offset + len);
        }
        std::move(begin() + offset + len, end(), begin() + offset);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_data + _size - len, _data + _size);
        }
    }
    _size -= len;
    return iterator(_data + offset);
}

template <class T, class Allocator>
constexpr void
vector<T, Allocator>::swap(vector &x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                                               std::allocator_traits<Allocator>::is_always_equal::value) {
    using std::swap;
    swap(_data, x._data);
    swap(_size, x._size);
    swap(_capacity, x._capacity);
    swap(_alloc, x._alloc);
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::destroy(_data, _data + _size);
    }
    _size = 0;
}
} // namespace j
