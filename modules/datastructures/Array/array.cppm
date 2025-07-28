/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <stdexcept>
#include <algorithm>

export module j.array;

namespace j {
    export template <class T, std::size_t N>
    class array {
    public:
        class iterator;
        class const_iterator;

        using value_type            = T;
        using pointer               = T*;
        using const_pointer         = const T*;
        using reference             = T&;
        using const_reference       = const T&;
        using size_type             = std::size_t;
        using difference_type       = std::ptrdiff_t;
        using iterator              = iterator;
        using const_iterator        = const_iterator;
        using reverse_iterator      = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Aggregate Type (no explicit construct/copy/destroy)
        T _data[N];

        // Accessor
        constexpr reference operator[](size_type index);
        constexpr const_reference operator[](size_type index) const;
        constexpr reference at(std::size_t pos);
        constexpr const_reference at(size_type pos) const;
        constexpr value_type front() const;
        constexpr value_type back() const;
        constexpr pointer data() noexcept;
        constexpr const_pointer data() const noexcept;

        // Iterators
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

        // Capacity
        constexpr size_type max_size() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr bool empty() const noexcept;

        // modifiers
        constexpr void fill(const T& value);
        constexpr void swap(array& other) noexcept( std::is_nothrow_swappable_v<T>)
        requires (std::is_swappable_v<T>);
    };

    template<class T, class... U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;

    // Non-member functions
    export template <std::size_t I, class T, std::size_t N>
    constexpr T& get(array<T, N>& a) noexcept;

    export template <std::size_t I, class T, std::size_t N>
    constexpr T&& get(array<T, N>&& a) noexcept;

    export template <std::size_t I, class T, std::size_t N>
    constexpr const T& get(const array<T, N>& a) noexcept;

    export template <std::size_t I, class T, std::size_t N>
    constexpr const T&& get(const array<T, N>&& a) noexcept;

    export template <typename T, std::size_t N>
    constexpr void swap(array<T, N>&a, array<T, N>& b) noexcept(
        N == 0 || std::is_nothrow_swappable_v<T>
    ) requires (N == 0 || std::is_swappable_v<T>);

    export template <class T, std::size_t N>
    constexpr j::array<std::remove_cv_t<T>, N> to_array (T (&arr)[N]);

    export template <class T, std::size_t N>
    constexpr j::array<std::remove_cv_t<T>, N> to_array (T (&&arr)[N]);

    template <class T, std::size_t N>
    class array<T, N>::iterator {
        friend class array;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        pointer _ptr;

    public:
        explicit iterator(pointer ptr = nullptr) : _ptr(ptr) {}
        explicit iterator(const const_iterator &other)
                : _ptr(other._ptr) {}
        iterator &operator=(const const_iterator &other) {
            _ptr = other._ptr;
            return *this;
        }

        reference operator*() { return *_ptr; }
        pointer operator->() { return &*_ptr; }

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
        bool operator!=(const iterator &other) const { return _ptr != other._ptr; }
        bool operator<(const iterator &other) const { return _ptr < other._ptr; }
        bool operator>(const iterator &other) const { return _ptr > other._ptr; }
        bool operator<=(const iterator &other) const { return _ptr <= other._ptr; }
        bool operator>=(const iterator &other) const { return _ptr >= other._ptr; }
        operator const_iterator() const { return const_iterator(_ptr); }
    };

    template <class T, std::size_t N>
    class array<T, N>::const_iterator {
        friend class array;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using const_pointer = const T*;
        using const_reference = const T&;

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
        const_pointer operator->() const { return _ptr; }

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
        bool operator!=(const const_iterator &other) const { return _ptr != other._ptr; }
        bool operator<(const const_iterator &other) const { return _ptr < other._ptr; }
        bool operator>(const const_iterator &other) const { return _ptr > other._ptr; }
        bool operator<=(const const_iterator &other) const { return _ptr <= other._ptr; }
        bool operator>=(const const_iterator &other) const { return _ptr >= other._ptr; }
    };

}

// implementation

namespace j {
    template <typename T, std::size_t N>
    constexpr T& array<T, N>::operator[](const size_type index) {
        return _data[index];
    }

    template <typename T, std::size_t N>
    constexpr const T& array<T, N>::operator[](const size_type index) const {
        return _data[index];
    }

    template <typename T, std::size_t N>
    constexpr T& array<T, N>::at(std::size_t pos) {
        if (pos >= N){
            throw std::out_of_range("index out of range");
        }
        return _data[pos];
    }

    template <typename T, std::size_t N>
    constexpr const T& array<T, N>::at(size_type pos) const {
        if (pos >= N){
            throw std::out_of_range("index out of range");
        }
        return _data[pos];
    }

    template <typename T, std::size_t N>
    constexpr T array<T, N>::front() const{
        return _data[0];
    }

    template <typename T, std::size_t N>
    constexpr T array<T, N>::back() const{
        return _data[N-1];
    }

    template <typename T, std::size_t N>
    constexpr T* array<T, N>::data() noexcept {
        return _data;
    }

    template <typename T, std::size_t N>
    constexpr const T* array<T, N>::data() const noexcept {
        return _data;
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::iterator array<T, N>::begin() noexcept {
        return iterator(_data);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_iterator array<T, N>::begin() const noexcept {
        return const_iterator(_data);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::iterator array<T, N>::end() noexcept {
        return iterator(_data + N);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_iterator array<T, N>::end() const noexcept {
        return const_iterator(_data + N);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::reverse_iterator array<T, N>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_reverse_iterator array<T, N>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::reverse_iterator array<T, N>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_reverse_iterator array<T, N>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_iterator array<T, N>::cbegin() const noexcept {
        return const_iterator(_data);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_iterator array<T, N>::cend() const noexcept {
        return const_iterator(_data + N);
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_reverse_iterator array<T, N>::crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    template <class T, std::size_t N>
    constexpr typename array<T, N>::const_reverse_iterator array<T, N>::crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    template <typename T, std::size_t N>
    constexpr std::size_t array<T, N>::max_size() const noexcept{
        return N;
    }

    template <typename T, std::size_t N>
    constexpr std::size_t array<T, N>::size() const noexcept{
        return N; // can use std::distance
    }

    template <typename T, std::size_t N>
    constexpr bool array<T, N>::empty() const noexcept{
        return size() == 0;
    }

    template <typename T, std::size_t N>
    constexpr void array<T, N>::fill(const T& value){
        for (std::size_t i = 0; i < N; i++){
            _data[i] = value;
        }
    }

    template <class T, std::size_t N>
    constexpr void array<T, N>::swap(array& other) noexcept(std::is_nothrow_swappable_v<T>) requires (std::
        is_swappable_v<T>) {
        if constexpr (N > 0) {
            std::swap_ranges(_data, _data + N, other._data);
        }
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr T& get(array<T, N>& a) noexcept {
        static_assert(I < N, "Index out of bounds in array get");
        return a._data[I];
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr T&& get(array<T, N>&& a) noexcept {
        static_assert(I < N, "Index out of bounds in array get");
        return a._data[I];
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr const T& get(const array<T, N>& a) noexcept {
        static_assert(I < N, "Index out of bounds in array get");
        return a._data[I];
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr const T&& get(const array<T, N>&& a) noexcept {
        static_assert(I < N, "Index out of bounds in array get");
        return a._data[I];
    }

    template <typename T, std::size_t N>
    constexpr void swap(array<T, N>&a, array<T, N>& b) noexcept(
        N == 0 || std::is_nothrow_swappable_v<T>
    ) requires (N == 0 || std::is_swappable_v<T>
    ) {
        a.swap(b);
    }

    template <class T, std::size_t N>
    constexpr array<std::remove_cv_t<T>, N> to_array(T(& arr)[N]) {
        array<std::remove_cv_t<T>, N> ret;
        for (std::size_t i = 0; i < N; i++) {
            ret[i] = arr[i];
        }
        return ret;
    }

    template <class T, std::size_t N>
    constexpr array<std::remove_cv_t<T>, N> to_array(T(&& arr)[N]) {
        array<std::remove_cv_t<T>, N> ret;
        for (std::size_t i = 0; i < N; i++) {
            ret[i] = std::move(arr[i]);
        }
        return ret;
    }
}
