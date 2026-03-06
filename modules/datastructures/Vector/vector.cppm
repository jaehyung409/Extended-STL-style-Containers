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

export module j:vector;

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
    pointer _first;
    pointer _last;
    size_type _capacity;
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
    constexpr void pop_back();

    template <class... Args> constexpr iterator emplace(const_iterator position, Args &&...args);
    constexpr iterator insert(const_iterator position, const T &x);
    constexpr iterator insert(const_iterator position, T &&x);
    constexpr iterator insert(const_iterator position, size_type n, const T &x);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    constexpr iterator insert(const_iterator position, InputIter first, InputIter last);
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
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename vector::value_type;
    using difference_type = typename vector::difference_type;
    using pointer = typename vector::pointer;
    using reference = typename vector::reference;

  private:
    pointer _ptr;

  public:
    explicit iterator(pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    iterator(const iterator &other) = default;

    reference operator*() const noexcept {
        return *_ptr;
    }
    pointer operator->() const noexcept {
        return &(*_ptr);
    }

    iterator &operator++() noexcept {
        ++_ptr;
        return *this;
    }

    iterator operator++(int) noexcept {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    iterator &operator--() noexcept {
        --_ptr;
        return *this;
    }

    iterator operator--(int) noexcept {
        iterator temp = *this;
        --(*this);
        return temp;
    }

    iterator &operator+=(difference_type n) noexcept {
        _ptr += n;
        return *this;
    }

    iterator operator+(difference_type n) const noexcept {
        iterator temp = *this;
        return temp += n;
    }

    friend iterator operator+(difference_type n, const iterator &it) noexcept {
        return it + n;
    }

    iterator &operator-=(difference_type n) noexcept {
        _ptr -= n;
        return *this;
    }

    iterator operator-(difference_type n) const noexcept {
        iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const iterator &other) const noexcept {
        return _ptr - other._ptr;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const iterator &other) const noexcept {
        return _ptr == other._ptr;
    }
    auto operator<=>(const iterator &other) const noexcept {
        return _ptr <=> other._ptr;
    }
    operator const_iterator() const noexcept {
        return const_iterator(_ptr);
    }
};

template <class T, class Allocator> class vector<T, Allocator>::const_iterator {
    friend vector;

  public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = typename vector::value_type;
    using difference_type = typename vector::difference_type;
    using pointer = typename vector::pointer;
    using reference = typename vector::reference;

  private:
    pointer _ptr;

  public:
    explicit const_iterator(pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    explicit const_iterator(const iterator &other) noexcept : _ptr(other._ptr) {}
    const_iterator(const const_iterator &other) = default;
    const_iterator &operator=(const const_iterator &other) = default;

    reference operator*() const noexcept {
        return *_ptr;
    }
    pointer operator->() const noexcept {
        return &(*_ptr);
    }

    const_iterator &operator++() noexcept {
        ++_ptr;
        return *this;
    }

    const_iterator operator++(int) noexcept {
        const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    const_iterator &operator--() noexcept {
        --_ptr;
        return *this;
    }

    const_iterator operator--(int) noexcept {
        const_iterator temp = *this;
        --(*this);
        return temp;
    }

    const_iterator &operator+=(difference_type n) noexcept {
        _ptr += n;
        return *this;
    }

    const_iterator operator+(difference_type n) const noexcept {
        const_iterator temp = *this;
        return temp += n;
    }

    const_iterator &operator-=(difference_type n) noexcept {
        _ptr -= n;
        return *this;
    }

    const_iterator operator-(difference_type n) const noexcept {
        const_iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const const_iterator &other) const noexcept {
        return _ptr - other._ptr;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const const_iterator &other) const noexcept {
        return _ptr == other._ptr;
    }

    auto operator<=>(const const_iterator &other) const noexcept {
        return _ptr <=> other._ptr;
    }
};

} // namespace j

namespace j {

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const Allocator &alloc) noexcept
    : _first(nullptr), _last(nullptr), _capacity(0),
      _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type n, const Allocator &alloc)
    : vector(n, T(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type n, const T &value, const Allocator &alloc)
    : _capacity(n), _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    if (n == 0) {
        _first = _last = nullptr;
        return;
    }
    _first = std::allocator_traits<Allocator>::allocate(_alloc, n);
    try {
        std::uninitialized_fill_n(_first, n, value);
        _last = _first + n;
    } catch (...) {
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, n);
        _first = _last = nullptr;
        _capacity = 0;
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
        _first = std::allocator_traits<Allocator>::allocate(_alloc, dist);
        _capacity = dist;
        try {
            if constexpr (std::is_trivially_copy_constructible_v<T> && std::contiguous_iterator<InputIter>) {
                std::memcpy(_first, std::to_address(first), dist * sizeof(T));
            } else {
                std::uninitialized_copy(first, last, _first);
            }
            _last = _first + dist;
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, _first, dist);
            _first = _last = nullptr;
            _capacity = 0;
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
    : _first(x._first), _last(x._last), _capacity(x._capacity), _alloc(std::move(x._alloc)) {
    x._first = nullptr;
    x._last = nullptr;
    x._capacity = 0;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector &x, const std::type_identity_t<Allocator> &alloc)
    : vector(x.begin(), x.end(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(vector &&x, const std::type_identity_t<Allocator> &alloc)
    : _first(nullptr), _last(nullptr), _capacity(x._capacity),
      _alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    if (alloc == x._alloc) {
        _first = x._first;
        _last = x._last;

        x._first = nullptr;
        x._last = nullptr;
        x._capacity = 0;
    } else {
        const size_type moved_size = x.size();
        _first = moved_size == 0 ? nullptr : std::allocator_traits<Allocator>::allocate(_alloc, moved_size);
        try {
            if constexpr (std::is_trivially_move_constructible_v<T>) {
                std::memcpy(_first, x._first, moved_size * sizeof(T));
            } else {
                std::uninitialized_move(x._first, x._last, _first);
            }
            _last = _first + moved_size;
            _capacity = moved_size;

            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(x._first, x._last);
            }
            std::allocator_traits<Allocator>::deallocate(x._alloc, x._first, x._capacity);
            x._first = nullptr;
            x._last = nullptr;
            x._capacity = 0;
        } catch (...) {
            if (_first != nullptr) {
                std::allocator_traits<Allocator>::deallocate(_alloc, _first, moved_size);
            }
            _first = _last = nullptr;
            _capacity = 0;
            throw;
        }
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(std::initializer_list<T> il, const Allocator &alloc)
    : vector(il.begin(), il.end(), alloc) {}

template <class T, class Allocator> constexpr vector<T, Allocator>::~vector() {
    clear();
    std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
}

template <class T, class Allocator> constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &x) {
    if (this != std::addressof(x)) {
        if (x.size() > _capacity) {
            vector<T, Allocator> tmp(x);
            swap(tmp);
        } else {
            clear();
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(_first, x._first, x.size() * sizeof(T));
            } else {
                std::uninitialized_copy(x._first, x._last, _first);
            }
            _last = _first + (x._last - x._first);
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
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            _alloc = std::move(x._alloc);
            _first = x._first;
            _last = x._last;
            _capacity = x._capacity;

            x._first = nullptr;
            x._last = nullptr;
            x._capacity = 0;
        } else {
            if (_alloc == x._alloc) {
                _first = x._first;
                _last = x._last;
                _capacity = x._capacity;
                x._first = x._last = nullptr;
                x._capacity = 0;
            } else {
                const size_type moved_size = x.size();
                _first = moved_size == 0 ? nullptr : std::allocator_traits<Allocator>::allocate(_alloc, moved_size);
                try {
                    if constexpr (std::is_trivially_move_constructible_v<T>) {
                        std::memmove(_first, x._first, moved_size * sizeof(T));
                    } else {
                        std::uninitialized_move(x._first, x._last, _first);
                    }
                    _last = _first + moved_size;
                    _capacity = moved_size;
                } catch (...) {
                    if (_first != nullptr) {
                        std::allocator_traits<Allocator>::deallocate(_alloc, _first, moved_size);
                    }
                    _first = _last = nullptr;
                    _capacity = 0;
                    throw;
                }
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    std::destroy(x._first, x._last);
                }
                std::allocator_traits<Allocator>::deallocate(x._alloc, x._first, x._capacity);
                x._first = x._last = nullptr;
                x._capacity = 0;
            }
        }
    }
    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(std::initializer_list<T> il) {
    clear();
    if (_capacity < il.size()) {
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = std::allocator_traits<Allocator>::allocate(_alloc, il.size());
        _capacity = il.size();
    }
    _last = _first + il.size();
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(_first, il.begin(), il.size() * sizeof(T));
    } else {
        std::uninitialized_copy(il.begin(), il.end(), _first);
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
            std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
            _first = std::allocator_traits<Allocator>::allocate(_alloc, dist);
            std::uninitialized_copy(first, last, _first);
            _last = _first + dist;
            _capacity = dist;
        } else {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(_first, std::to_address(first), dist * sizeof(T));
            } else {
                if (!std::is_trivially_destructible_v<T>) {
                    std::destroy(_first, _last);
                }
                std::uninitialized_copy(first, last, _first);
            }
            _last = _first + dist;
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
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = std::allocator_traits<Allocator>::allocate(_alloc, n);
        _last = _first + n;
        _capacity = n;
        std::uninitialized_fill_n(_first, n, u);
    } else {
        if constexpr (std::is_trivially_destructible_v<T>) {
            std::fill_n(_first, n, u);
        } else {
            std::destroy(_first, _last);
            std::uninitialized_fill_n(_first, n, u);
        }
        _last = _first + n;
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
    return iterator(_first);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept {
    return const_iterator(_first);
}

template <class T, class Allocator> constexpr vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
    return iterator(_last);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept {
    return const_iterator(_last);
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
    return const_iterator(_first);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept {
    return const_iterator(_last);
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
    return _first == _last;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept {
    return _first == _last ? 0 : static_cast<size_type>(_last - _first);
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
    if (sz < size()) {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first + sz, _last);
        }
    } else if (sz > size()) {
        if (sz > _capacity) {
            reserve(std::max(sz, _capacity * 2));
        }
        std::uninitialized_fill_n(_last, sz - size(), c);
    }
    _last = _first + sz;
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::reserve(size_type n) {
    if (n > _capacity) {
        const size_type old_size = size();
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, n);
        try {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memmove(new_data, _first, old_size * sizeof(T));
            } else {
                std::uninitialized_move(_first, _last, new_data);
            }
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, n);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first, _last);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = new_data;
        _last = _first + old_size;
        _capacity = n;
    }
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::shrink_to_fit() {
    if (size() < _capacity) {
        const size_type old_size = size();
        pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, old_size);
        try {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memmove(new_data, _first, old_size * sizeof(T));
            } else {
                std::uninitialized_move(_first, _last, new_data);
            }
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_data, old_size);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first, _last);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = new_data;
        _last = _first + old_size;
        _capacity = old_size;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type n) {
    return _first[n];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type n) const {
    return _first[n];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n) {
    if (n >= size()) {
        throw std::out_of_range("vector::at() : index is out of range");
    }
    return _first[n];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const {
    if (n >= size()) {
        throw std::out_of_range("vector::at() : index is out of range");
    }
    return _first[n];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::front() {
    return _first[0];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
    return _first[0];
}

template <class T, class Allocator> constexpr vector<T, Allocator>::reference vector<T, Allocator>::back() {
    return *(_last - 1);
}

template <class T, class Allocator> constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
    return *(_last - 1);
}

template <class T, class Allocator> constexpr T *vector<T, Allocator>::data() noexcept {
    return _first;
}

template <class T, class Allocator> constexpr const T *vector<T, Allocator>::data() const noexcept {
    return _first;
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&...args) {
    if (size() == _capacity) {
        reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    std::construct_at(std::addressof(*_last), std::forward<Args>(args)...);
    return *(_last++);
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::push_back(const T &x) {
    emplace_back(x);
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::push_back(T &&x) {
    emplace_back(std::move(x));
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::pop_back() {
    --_last;
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::destroy_at(std::addressof(*_last));
    }
}

template <class T, class Allocator>
template <class... Args>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator position, Args &&...args) {
    const difference_type offset = position - begin();
    const size_type old_size = size();
    if (old_size == _capacity) {
        const size_type new_capacity = _capacity == 0 ? 1 : _capacity * 2;
        pointer new_first = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_first, _first + offset, new_first);
            std::construct_at(std::addressof(new_first[offset]), std::forward<Args>(args)...);
            std::uninitialized_move(_first + offset, _last, new_first + offset + 1);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_first, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first, _last);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = new_first;
        _last = _first + old_size;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_first + offset + 1, _first + offset, (old_size - offset) * sizeof(T));
            std::construct_at(std::addressof(_first[offset]), std::forward<Args>(args)...);
        } else {
            std::construct_at(end(), std::move(*(_last - 1)));
            std::move_backward(_first + offset, _last - 1, _last);
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy_at(std::addressof(_first[offset]));
            }
            std::construct_at(std::addressof(_first[offset]), std::forward<Args>(args)...);
        }
    }
    ++_last;
    return iterator(_first + offset);
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
    const size_type old_size = size();
    if (n == 0) {
        return iterator(_first + offset);
    }

    if (old_size + n > _capacity) {
        const size_type new_capacity = _capacity == 0 ? std::max(1ul, n) : std::max(_capacity * 2, old_size + n);
        pointer new_first = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_first, _first + offset, new_first);
            std::uninitialized_fill_n(new_first + offset, n, x);
            std::uninitialized_move(_first + offset, _last, new_first + offset + n);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_first, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first, _last);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = new_first;
        _last = _first + old_size;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_first + offset + n, _first + offset, (old_size - offset) * sizeof(T));
            std::uninitialized_fill_n(_first + offset, n, x);
        } else {
            std::uninitialized_move(_last - std::min(n, old_size - offset), _last,
                                    _last + n - std::min(n, old_size - offset));
            if (old_size - offset > n) {
                std::move_backward(_first + offset, _last - n, _last);
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(_first + offset, _first + offset + std::min(n, old_size - offset));
            }
            std::uninitialized_fill_n(_first + offset, n, x);
        }
    }
    _last += n;
    return iterator(_first + offset);
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, InputIter first,
                                                                      InputIter last) {
    const difference_type offset = position - begin();
    const size_type old_size = size();

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
        return iterator(_first + offset);
    }

    if (old_size + dist > _capacity) {
        const size_type new_capacity = _capacity == 0 ? std::max(1ul, dist) : std::max(_capacity * 2, old_size + dist);
        pointer new_first = std::allocator_traits<Allocator>::allocate(_alloc, new_capacity);
        try {
            std::uninitialized_move(_first, _first + offset, new_first);
            std::uninitialized_copy(first, last, new_first + offset);
            std::uninitialized_move(_first + offset, _last, new_first + offset + dist);
        } catch (...) {
            std::allocator_traits<Allocator>::deallocate(_alloc, new_first, new_capacity);
            throw;
        }
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first, _last);
        }
        std::allocator_traits<Allocator>::deallocate(_alloc, _first, _capacity);
        _first = new_first;
        _last = _first + old_size;
        _capacity = new_capacity;
    } else {
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memmove(_first + offset + dist, _first + offset, (old_size - offset) * sizeof(T));
            if constexpr (std::contiguous_iterator<InputIter>) {
                std::memcpy(_first + offset, std::to_address(first), dist * sizeof(T));
            } else {
                std::copy(first, last, _first + offset);
            }
        } else {
            std::uninitialized_move(_last - std::min(dist, old_size - offset), _last,
                                    _last + dist - std::min(dist, old_size - offset));
            if (old_size - offset > dist) {
                std::move_backward(_first + offset, _last - dist, _last);
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(_first + offset, _first + offset + std::min(dist, old_size - offset));
            }
            std::uninitialized_copy(first, last, _first + offset);
        }
    }
    _last += dist;
    return iterator(_first + offset);
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
        std::memmove(_first + offset, _first + offset + 1, (size() - offset - 1) * sizeof(T));
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy_at(std::addressof(_first[offset]));
        }
        std::move(begin() + offset + 1, end(), begin() + offset);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy_at(std::addressof(*(_last - 1)));
        }
    }
    --_last;
    return iterator(_first + offset);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
    const difference_type offset = first - begin();
    const difference_type len = last - first;
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memmove(_first + offset, _first + offset + len, (size() - offset - len) * sizeof(T));
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_first + offset, _first + offset + len);
        }
        std::move(begin() + offset + len, end(), begin() + offset);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy(_last - len, _last);
        }
    }
    _last -= len;
    return iterator(_first + offset);
}

template <class T, class Allocator>
constexpr void
vector<T, Allocator>::swap(vector &x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                                               std::allocator_traits<Allocator>::is_always_equal::value) {
    using std::swap;
    swap(_first, x._first);
    swap(_last, x._last);
    swap(_capacity, x._capacity);
    swap(_alloc, x._alloc);
}

template <class T, class Allocator> constexpr void vector<T, Allocator>::clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::destroy(_first, _last);
    }
    _last = _first;
}
} // namespace j
