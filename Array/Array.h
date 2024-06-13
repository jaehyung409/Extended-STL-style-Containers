//
// Created by user on 24. 5. 29.
//

#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <stdexcept>
#include <algorithm>

template <typename T, size_t N>
struct Array {
    T data[N];

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
    int size(){ return N; }
    void print(){
        for(const auto& element : data)
            std::cout << element << ' ';
        std::cout << '\n';
    }
    void sorting(int index){
        std::sort(data, data+index);
    }
    int find(const T element);
    void insert(const T element, const int index);
    void remove(const T element);
    int sort_find(const T element, const int low, const int high);
    void sort_insert(const T element);
    void sort_remove(const T element);
};


#endif //ARRAY_ARRAY_H

