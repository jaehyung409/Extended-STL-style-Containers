//
// Created by user on 24. 5. 29.
//

#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <stdexcept>
#include <algorithm>

#include <type_traits>

template <typename T>
struct DefaultValue {
    static constexpr T value = std::conditional_t<std::is_arithmetic<T>::value, std::integral_constant<T, 0>, std::integral_constant<T*, nullptr>>::value;
};

template <typename T, size_t N>
struct Array {
public:
    T data[N];

public:
    T& operator[](const int index) {
        if (index >= N){
            throw std::out_of_range("index out out_of_range");
        }
        return data[index];
    }
    const T& operator[](const int index) const {
        if (index >= N){
            throw std::out_of_range("index out out_of_range");
        }
        return data[index];
    }
    size_t max_size(){
        return N;
    }
    size_t size(){
        for(const auto& element : this->data) {
            if (element == DefaultValue<T>::value)
                return &element - this->data;
        }
        return max_size();
    }
    bool empty(){
        return this->size() == 0;
    }
    void print(){
        for(const auto& element : this->data) {
            if(element != DefaultValue<T>::value)
                std::cout << element << ' ';
        }
        std::cout << '\n';
    }
    void sorting(int index){
        std::sort(this->data, this->data+index);
    }
    int find(T element);
    void insert(T element, int index);
    void remove(T element);
    void push_back(T element){
        this->insert(element,this->size());
    }
    void push_front(T element){
        this->insert(element, 0);
    }
    void pop_back(){
        if (this->empty()) throw std::out_of_range("pop_back() called on empty array");
        this->data[this->size()-1] = DefaultValue<T>::value;
    }
    void pop_front(){
        if (this->empty()) throw std::out_of_range("pop_front() called on empty array");
        this->remove(data[0]);
    }
    T front(){
        if (this->empty()) throw std::out_of_range("front() called on empty array");
        return this->data[0];
    }
    T back(){
        if (this->empty()) throw std::out_of_range("back() called on empty array");
        return this->data[this->size()-1];
    }

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
};


#endif //ARRAY_ARRAY_H

