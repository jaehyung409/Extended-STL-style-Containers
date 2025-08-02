/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */


module;
#include <limits>
#include <memory>
#include <iterator>
#include <initializer_list>
#include <iostream> // for debugging

export module j.vector;

import j.range_basics;

namespace j {
    // not-yet specialization for bool ...
    export template<class T, class Allocator = std::allocator<T>>
    class vector {
    public:
        using value_type                = T;
        using allocator_type            = Allocator;
        using pointer                   = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer             = typename std::allocator_traits<Allocator>::const_pointer;
        using reference                 = value_type&;
        using const_reference           = const value_type&;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;
        class iterator;
        class const_iterator;
        using reverse_iterator          = std::reverse_iterator<iterator>;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

    private:
        size_type _size;
        size_type _capacity;
        pointer _data;
        allocator_type _alloc;

    public:
        // constructor/copy/destructor
        constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
        constexpr explicit vector(const Allocator& alloc) noexcept;
        constexpr explicit vector(size_type n, const Allocator& alloc = Allocator());
        constexpr vector(size_type n, const T& value, const Allocator& alloc = Allocator());
        template<class InputIter>
        requires std::input_iterator<InputIter>
        constexpr vector(InputIter first, InputIter last, const Allocator& alloc = Allocator());
        template<container-compatible-range<T> R>
        constexpr vector(from_range_t, R&& range, const Allocator& = Allocator()) = delete;
        constexpr vector(const vector& x);
        constexpr vector(vector&& x) noexcept;
        constexpr vector(const vector& x, const std::type_identity_t<Allocator>& alloc);
        constexpr vector(vector&& x, const std::type_identity_t<Allocator>& alloc);
        constexpr vector(std::initializer_list<T> il, const Allocator& alloc = Allocator());
        constexpr ~vector();

        constexpr vector& operator=(const vector& x);
        constexpr vector& operator=(vector&& x) noexcept(
                std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
                std::allocator_traits<Allocator>::is_always_equal::value);
        constexpr vector& operator=(std::initializer_list<T> il);
        template<class InputIter>
        requires std::input_iterator<InputIter>
        constexpr void assign(InputIter first, InputIter last);
        template<container-compatible-range<T> R>
        constexpr void assign_range(R&& rg) = delete;
        constexpr void assign(size_type n, const T& u);
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
        constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr size_type capacity() const noexcept;
        constexpr void resize(size_type sz);
        constexpr void resize(size_type sz, const T& c);
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
        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        // modifiers
        template<class... Args>
        constexpr reference emplace_back(Args&&... args);
        constexpr void push_back(const T& x);
        constexpr void push_back(T&& x);
        template<container-compatible-range<T> R>
        constexpr void append_range(R&& rg) = delete;
        constexpr void pop_back();

        template<class... Args>
        constexpr iterator emplace(const_iterator position, Args&&... args);
        constexpr iterator insert(const_iterator position, const T& x);
        constexpr iterator insert(const_iterator position, T&& x);
        constexpr iterator insert(const_iterator position, size_type n, const T& x);
        template<class InputIter>
        requires std::input_iterator<InputIter>
        constexpr iterator insert(const_iterator position, InputIter first, InputIter last);
        template<container-compatible-range<T> R>
        constexpr void insert_range(const_iterator position, R&& rg) = delete;
        constexpr iterator insert(const_iterator position, std::initializer_list<T> il);
        constexpr iterator erase(const_iterator position);
        constexpr iterator erase(const_iterator first, const_iterator last);
        constexpr void swap(vector& x) noexcept(
                std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
                std::allocator_traits<Allocator>::is_always_equal::value);
        constexpr void clear() noexcept;
    };

    template <class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
    vector(InputIter, InputIter, Allocator = Allocator())
        -> vector<typename std::iterator_traits<InputIter>::value_type, Allocator>;

    template <std::ranges::input_range R, class Allocator = std::allocator<std::ranges::range_value_t<R>>>
    vector(std::ranges::from_range_t, R&&, Allocator = Allocator())
        -> vector<std::ranges::range_value_t<R>, Allocator>;


    export template <class T, class Allocator>
    bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    export template <class T, class Allocator>
    auto operator<=>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                      rhs.begin(), rhs.end(),
                                                      std::compare_three_way{});
    }

    export template <class T, class Allocator>
    void swap(vector<T, Allocator>& x, vector<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    export template <class T, class Allocator, class U>
    typename vector<T, Allocator>::size_type remove(vector<T, Allocator>& c, const U& value) {
        return c.remove(value);
    }

    export template <class T, class Allocator, class Predicate>
    typename vector<T, Allocator>::size_type remove_if(vector<T, Allocator>& c, Predicate pred) {
        return c.remove_if(pred);
    }

    template <class T, class Allocator>
    class vector<T, Allocator>::iterator {
        friend vector;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = typename vector::value_type;
        using difference_type   = typename vector::difference_type;
        using pointer           = typename vector::pointer;
        using reference         = typename vector::reference;

    private:
        pointer _ptr;

    public:
        explicit iterator(pointer ptr = nullptr) : _ptr(ptr) {}
        iterator &operator=(const const_iterator &other) {
            _ptr = other._ptr;
            return *this;
        }

        reference operator*() { return *_ptr; }
        pointer operator->() { return &(*_ptr); }

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

        bool operator==(const iterator &other) const { return _ptr == other._ptr; }
        auto operator<=>(const iterator &other) const { return  _ptr <=> other._ptr; }
        operator const_iterator() const { return const_iterator(_ptr); }
    };

    template <class T, class Allocator>
    class vector<T, Allocator>::const_iterator {
        friend vector;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = typename vector::value_type;
        using difference_type   = typename vector::difference_type;
        using pointer           = typename vector::const_pointer;
        using reference         = typename vector::const_reference;
        using const_pointer     = typename vector::const_pointer;
        using const_reference   = typename vector::const_reference;

    private:
        pointer _ptr;

    public:
        explicit const_iterator(pointer ptr = nullptr) : _ptr(ptr) {}
        explicit const_iterator(const iterator &other)
                : _ptr(other._ptr) {}
        const_iterator &operator=(const iterator &other) {
            _ptr = other._ptr;
            return *this;
        }

        const_reference operator*() const { return *_ptr; }
        const_pointer operator->() const { return &(*_ptr); }

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

        bool operator==(const const_iterator &other) const { return _ptr == other._ptr; }
        auto operator<=>(const const_iterator &other) const { return _ptr <=> other._ptr; }
    };

}

namespace j {

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(const Allocator &alloc) noexcept
        : _size(0), _capacity(0), _data(nullptr), _alloc(alloc) {}

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(vector::size_type n, const Allocator &alloc)
        : vector(n, T(), alloc) {}

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(vector::size_type n, const T &value, const Allocator &alloc)
        : vector(alloc) {
            _size = n;
            _capacity = n;
            _data = std::allocator_traits<Allocator>::allocate(_alloc, n);
            for (size_type i = 0; i < n; ++i) {
                std::construct_at(std::addressof(_data[i]), value);
            }
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires std::input_iterator<InputIter>
    constexpr vector<T, Allocator>::vector(InputIter first, InputIter last, const Allocator &alloc)
        : vector(alloc) {
            this->reserve(std::distance(first, last));
            std::copy(first, last, std::back_inserter(*this));
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(const vector &x)
        : vector(x, std::allocator_traits<Allocator>::select_on_container_copy_construction(x.get_allocator())) {}

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(vector &&x) noexcept : vector(std::move(x), x.get_allocator()) {}

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(const vector &x, const std::type_identity_t <Allocator> &alloc)
        : vector(alloc) {
            this->reserve(x.size());
            std::copy(x.begin(), x.end(), std::back_inserter(*this));
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(vector &&x, const std::type_identity_t <Allocator> &alloc)
        : vector(alloc) {
        if (alloc == x.get_allocator()) {
            _data = x._data;
            _size = x._size;
            _capacity = x._capacity;
            x._set_data(nullptr);
            x._set_size(0);
            x._set_capacity(0);
        } else {
            this->reserve(x.size());
            this->insert(this->begin(), std::make_move_iterator(x.cbegin()),
                                      std::make_move_iterator(x.cend()));
            x.clear();
        }
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::vector(std::initializer_list<T> il, const Allocator &alloc)
        : vector(il.begin(), il.end(), alloc) {}

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::~vector() {
        clear();
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &x) {
        clear();
        if (_capacity < x.size()) {
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = std::allocator_traits<Allocator>::allocate(_alloc, x.size());
            _capacity = x.size();
        }
        _size = x.size();
        for (size_type i = 0; i < x.size(); ++i) {
             std::construct_at(std::addressof(_data[i]), x.begin()[i]);
        }
        return *this;
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&x) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
        clear();
        if (_alloc == get_allocator()) {
            _data = x._data;
            _size = x._size;
            _capacity = x._capacity;
            x._set_data(nullptr);
            x._set_size(0);
            x._set_capacity(0);
        } else {
            this->reserve(x.size());
            this->insert(this->begin(), std::make_move_iterator(x.cbegin()),
                                      std::make_move_iterator(x.cend()));
            x.clear();
        }
        return *this;
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator> &vector<T, Allocator>::operator=(std::initializer_list<T> il) {
        clear();
        if (_capacity < il.size()) {
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = std::allocator_traits<Allocator>::allocate(_alloc, il.size());
            _capacity = il.size();
        }
        _size = il.size();
        for (size_type i = 0; i < il.size(); ++i) {
            std::construct_at(std::addressof(_data[i]), il.begin()[i]);
        }
        return *this;
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires std::input_iterator<InputIter>
    constexpr void vector<T, Allocator>::assign(InputIter first, InputIter last) {
        clear();
        this->reserve(std::distance(first, last));
        this->insert(this->begin(), first, last);
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::assign(vector::size_type n, const T &u) {
        clear();
        this->reserve(n);
        this->insert(this->begin(), n, u);
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const noexcept {
        return _alloc;
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
        return iterator(_data);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept {
        return const_iterator(_data);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
        return iterator(_data + _size);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept {
        return const_iterator(_data + _size);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_data);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept {
        return const_iterator(_data + _size);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    template<class T, class Allocator>
    constexpr bool vector<T, Allocator>::empty() const noexcept {
        return _size == 0;
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept {
        return _size;
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept {
        return _capacity;
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::resize(vector::size_type sz) {
        resize(sz, T());
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::resize(vector::size_type sz, const T &c) {
        if (sz < _size) {
            for (size_type i = sz; i < _size; ++i) {
                std::destroy_at(std::addressof(_data[i]));
            }
            _size = sz;
        } else if (sz > _size) {
            if (sz > _capacity) {
                reserve(sz);
            }
            for (size_type i = _size; i < sz; ++i) {
                std::construct_at(std::addressof(_data[i]), c);
            }
            _size = sz;
        }
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::reserve(vector::size_type n) {
        if (n > max_size()) {
            throw std::length_error("vector::reserve() : requested size is too large");
        }
        if (n > _capacity) {
            pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, n);
            std::uninitialized_move(begin(), end(), new_data);
            std::destroy(begin(), end());
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = new_data;
            _capacity = n;
        }
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::shrink_to_fit() {
        if (_size < _capacity) {
            pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, _size);
            std::uninitialized_move(begin(), end(), new_data);
            std::destroy(begin(), end());
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = new_data;
            _capacity = _size;
        }
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](vector::size_type n) {
        return _data[n];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reference
    vector<T, Allocator>::operator[](vector::size_type n) const {
        return _data[n];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::at(vector::size_type n) {
        if (n >= _size) {
            throw std::out_of_range("vector::at() : index is out of range");
        }
        return _data[n];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(vector::size_type n) const {
        if (n >= _size) {
            throw std::out_of_range("vector::at() : index is out of range");
        }
        return _data[n];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
        return _data[0];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
        return _data[0];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
        return _data[_size - 1];
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
        return _data[_size - 1];
    }

    template<class T, class Allocator>
    constexpr T *vector<T, Allocator>::data() noexcept {
        return _data;
    }

    template<class T, class Allocator>
    constexpr const T *vector<T, Allocator>::data() const noexcept {
        return _data;
    }

    template<class T, class Allocator>
    template<class... Args>
    constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&... args) {
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        std::construct_at(std::addressof(_data[_size]), std::forward<Args>(args)...);
        return _data[_size++];
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::push_back(const T &x) {
        emplace_back(x);
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::push_back(T &&x) {
        emplace_back(std::forward<T>(x));
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::pop_back() {
        --_size;
        std::destroy_at(std::addressof(_data[_size]));
    }

    template<class T, class Allocator>
    template<class... Args>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::emplace(vector::const_iterator position, Args &&... args) {
        const difference_type offset = position - begin();
        if (_size == _capacity) {
            pointer new_data = std::allocator_traits<Allocator>::allocate(_alloc, _capacity == 0 ? 1 : _capacity * 2);
            std::uninitialized_move(begin(), begin() + offset, new_data);
            std::construct_at(std::addressof(new_data[offset]), std::forward<Args>(args)...);
            std::uninitialized_move(begin() + offset, end(), new_data + offset + 1);
            std::destroy(begin(), end());
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = new_data;
            _capacity = _capacity == 0 ? 1 : _capacity * 2;
        } else {
            std::move_backward(begin() + offset, end(), end() + 1);
            std::construct_at(std::addressof(_data[offset]), std::forward<Args>(args)...);
        }
        ++_size;
        return iterator(_data + offset);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(vector::const_iterator position, const T &x) {
        return emplace(position, x);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(vector::const_iterator position, T &&x) {
        return emplace(position, std::forward<T>(x));
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(vector::const_iterator position, vector::size_type n, const T &x) {
        const difference_type offset = position - begin();
        if (_size + n > _capacity) {
            pointer new_data = std::allocator_traits<Allocator>::allocate
                    (_alloc, _capacity == 0 ? std::max(1ul, n) : std::max(_capacity * 2, _capacity + n));
            std::uninitialized_move(begin(), begin() + offset, new_data);
            for (size_type i = 0; i < n; ++i) {
                std::construct_at(std::addressof(new_data[offset + i]), x);
            }
            std::uninitialized_move(begin() + offset, end(), new_data + offset + n);
            std::destroy(begin(), end());
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = new_data;
            _capacity = _capacity == 0 ? std::max(1ul, n) : std::max(_capacity * 2, _capacity + n);
        } else {
            std::move_backward(begin() + offset, end(), end() + n);
            for (size_type i = 0; i < n; ++i) {
                std::construct_at(std::addressof(_data[offset + i]), x);
            }
        }
        _size += n;
        return iterator(_data + offset);
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires std::input_iterator<InputIter>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(vector::const_iterator position, InputIter first, InputIter last) {
        const size_type len = std::distance(first, last);
        const difference_type offset = position - begin();
        if (_size + len > _capacity) {
            pointer new_data = std::allocator_traits<Allocator>::allocate
                    (_alloc, _capacity == 0 ? std::max(1ul, len) : std::max(_capacity * 2, _capacity + len));
            std::uninitialized_move(begin(), begin() + offset, new_data);
            for (size_type i = 0; i < len; ++i) {
                std::construct_at(std::addressof(new_data[offset + i]), *first++);
            }
            std::uninitialized_move(begin() + offset, end(), new_data + offset + len);
            std::destroy(begin(), end());
            std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
            _data = new_data;
            _capacity = _capacity == 0 ? std::max(1ul, len) : std::max(_capacity * 2, _capacity + len);
        } else {
            std::move_backward(begin() + offset, end(), end() + len);
            for (size_type i = 0; i < len; ++i) {
                std::construct_at(std::addressof(_data[offset + i]), *first++);
            }
        }
        _size += len;
        return iterator(_data + offset);
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(vector::const_iterator position, std::initializer_list<T> il) {
        return insert(position, il.begin(), il.end());
    }

    template<class T, class Allocator>
    constexpr typename vector<T, Allocator>::iterator
    vector<T, Allocator>::erase(vector::const_iterator position) {
        const difference_type offset = position - begin();
        std::destroy_at(std::addressof(*position));
        std::move(begin() + offset + 1, end(), begin() + offset);
        --_size;
        return iterator(position);
    }

    template<class T, class Allocator>
    constexpr vector<T, Allocator>::iterator
    vector<T, Allocator>::erase(vector::const_iterator first, vector::const_iterator last) {
        const difference_type offset = first - begin();
        const difference_type len = last - first;
        std::destroy(first, last);
        std::move(begin() + offset + len, end(), begin() + offset);
        _size -= len;
        return iterator(_data + offset);
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::swap(vector &x) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
        using std::swap;
        swap(_data, x._data);
        swap(_size, x._size);
        swap(_capacity, x._capacity);
        swap(_alloc, x._alloc);
    }

    template<class T, class Allocator>
    constexpr void vector<T, Allocator>::clear() noexcept {
        std::destroy(begin(), end());
        _size = 0;
    }

}