/*
 * @ Created by jaehyung409 on 25. 1. 15.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <stdexcept>

export module array;

namespace j {
    export template <typename T, std::size_t N>
    struct Array {
    public:
        using value_type =T;

        // Aggregate Type
        T data[N];

        // Accessor
        T& operator[](const std::size_t index);
        const T& operator[](const std::size_t index) const;
        constexpr T& at(std::size_t pos);
        constexpr const T& at(std::size_t pos) const;
        T front() const;
        T back() const;

        // Capacity
        std::size_t max_size() const;
        std::size_t size() const;
        bool empty() const;

        // modifiers
        void fill(T value);

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

    template <typename T, std::size_t N>
    void swap(Array<T, N>&a, Array<T, N>& b);
}

namespace j {
    template <typename T, std::size_t N>
    T& Array<T, N>::operator[](const std::size_t index) {
        return data[index];
    }

    template <typename T, std::size_t N>
    const T& Array<T, N>::operator[](const std::size_t index) const {
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
    constexpr const T& Array<T, N>::at(std::size_t pos) const {
        if (pos >= N){
            throw std::out_of_range("index out of range");
        }
        return data[pos];
    }

    template <typename T, std::size_t N>
    T Array<T, N>::front() const{
        return data[0];
    }

    template <typename T, std::size_t N>
    T Array<T, N>::back() const{
        return data[N-1];
    }

    template <typename T, std::size_t N>
    std::size_t Array<T, N>::max_size() const{
        return N;
    }

    template <typename T, std::size_t N>
    std::size_t Array<T, N>::size() const{
        return N;
    }

    template <typename T, std::size_t N>
    bool Array<T, N>::empty() const{
        return size() == 0;
    }

    template <typename T, std::size_t N>
    void Array<T, N>::fill(T value){
        for (std::size_t i = 0; i < N; i++){
            data[i] = value;
        }
    }

    template <typename T, std::size_t N>
    void swap(Array<T, N>&a, Array<T, N>& b){
        for (std::size_t i = 0; i < N; i++){
            std::swap(a.data[i], b.data[i]);
        }
    }
}
