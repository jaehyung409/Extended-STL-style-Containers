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
        using value_type = T;
        using size_type = std::size_t;
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
        constexpr T& operator[](size_type index);
        constexpr const T& operator[](size_type index) const;
        constexpr T& at(std::size_t pos);
        constexpr const T& at(size_type pos) const;
        constexpr T front() const;
        constexpr T back() const;
        constexpr reference operator[](size_type index);
        constexpr const_reference operator[](size_type index) const;
        constexpr reference at(std::size_t pos);
        constexpr const_reference at(size_type pos) const;
        constexpr value_type front() const;
        constexpr value_type back() const;
        constexpr pointer data() noexcept;
        constexpr const_pointer data() const noexcept;

        // Capacity
        constexpr size_type max_size() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr bool empty() const noexcept;

        // modifiers
        constexpr void fill(T& value);

        /* Implementation with Algo modules. (later)
         * After They will be deleted
        void print();
        void print_reverse();
        void sorting();
        int sort_find_binary(T element, int low, int high);
        int sort_find_ternary(T element, int low, int high);
        void sort_insert(T element);
        void sort_remove(T element);
        void reverse_array_extra_array();
        void reverse_array_two_pointer();
        void reverse_array_recursive(int left, int right);
        void reverse_array_stack();

        void get_leader();
        bool pair_sum_unsorted(T sum);
        bool pair_sum_unsorted_using_remainders(T sum);
        bool pair_sum_sorted_two_point(T sum);
        bool pair_sum_sorted_binary_search(T sum);
        bool pair_sum_sorted_hashing(T sum);
        */
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
    constexpr void swap(array<T, N>&a, array<T, N>& b) noexcept;
    constexpr void swap(array<T, N>&a, array<T, N>& b) noexcept(
        N == 0 || std::is_nothrow_swappable_v<T>
    ) requires (N == 0 || std::is_swappable_v<T>);

    export template <class T, std::size_t N>
    constexpr j::array<std::remove_cv_t<T>, N> to_array (T (&arr)[N]);

    export template <class T, std::size_t N>
    constexpr j::array<std::remove_cv_t<T>, N> to_array (T (&&arr)[N]);
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

    template <typename T, std::size_t N>
    constexpr void swap(array<T, N>&a, array<T, N>& b) noexcept {
        std::swap_ranges(std::begin(a), std::end(a), std::begin(b));
    }
}
