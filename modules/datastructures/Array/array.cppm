/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <stdexcept>

export module array;
// need to make create func, operators,iterator, accessor.., tuple ..?
namespace j {
    export template <class T, std::size_t N>
    class Array {
    public:
        using value_type = T;
        using size_type = std::size_t;

        // Aggregate Type (no explicit construct/copy/destroy)
        T data[N];

        // Accessor
        constexpr T& operator[](size_type index);
        constexpr const T& operator[](size_type index) const;
        constexpr T& at(std::size_t pos);
        constexpr const T& at(size_type pos) const;
        constexpr T front() const;
        constexpr T back() const;

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
    };

    template<class T, class... U>
    Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

    export template <typename T, std::size_t N>
    constexpr void swap(Array<T, N>&a, Array<T, N>& b) noexcept;
}

// implementation

namespace j {
    template <typename T, std::size_t N>
    constexpr T& Array<T, N>::operator[](const size_type index) {
        return data[index];
    }

    template <typename T, std::size_t N>
    constexpr const T& Array<T, N>::operator[](const size_type index) const {
        return data[index];
    }

    template <typename T, std::size_t N>
    constexpr T& Array<T, N>::at(std::size_t pos) {
        if (pos >= N){
            throw std::out_of_range("index out of range");
        }
        return data[pos];
    }

    template <typename T, std::size_t N>
    constexpr const T& Array<T, N>::at(size_type pos) const {
        if (pos >= N){
            throw std::out_of_range("index out of range");
        }
        return data[pos];
    }

    template <typename T, std::size_t N>
    constexpr T Array<T, N>::front() const{
        return data[0];
    }

    template <typename T, std::size_t N>
    constexpr T Array<T, N>::back() const{
        return data[N-1];
    }

    template <typename T, std::size_t N>
    constexpr std::size_t Array<T, N>::max_size() const noexcept{
        return N;
    }

    template <typename T, std::size_t N>
    constexpr std::size_t Array<T, N>::size() const noexcept{
        return N;
    }

    template <typename T, std::size_t N>
    constexpr bool Array<T, N>::empty() const noexcept{
        return size() == 0;
    }

    template <typename T, std::size_t N>
    constexpr void Array<T, N>::fill(T value){
        for (std::size_t i = 0; i < N; i++){
            data[i] = value;
        }
    }

    template <typename T, std::size_t N>
    constexpr void swap(Array<T, N>&a, Array<T, N>& b) noexcept
    {
        for (std::size_t i = 0; i < N; i++){
            std::swap(a.data[i], b.data[i]);
        }
    }
}
