//
// Created by user on 24. 5. 29.
//

#ifndef ARRAY_SORTED_H
#define ARRAY_SORTED_H

#include "Array.h"

template <typename T, size_t N>
void Array<T, N>::sorting() {
    std::sort(this->data, this->data+this->size());
}

template <typename T, size_t N>
int Array<T, N>::sort_find_binary(const T element, int low, int high){
    if (high < low)
        return -1;
    int mid = (low + high) / 2;
    if (element == this->data[mid]) return mid;
    if (element > this->data[mid])
        return sort_find_binary(element, mid + 1, high);
    return sort_find_binary(element, low, mid - 1);
}//시간복잡도 O(logN) 공간복잡도 O(logN)

template <typename T, size_t N>
int Array<T, N>::sort_find_ternary(const T element, int low, int high){
    if (high < low)
        return -1;
    int mid1 = low + (high - low) / 3;
    int mid2 = high - (high - low) / 3;
    if (element == this->data[mid1]) return mid1;
    if (element == this->data[mid2]) return mid2;
    if (element < this. data[mid1])
        return sort_find_ternary(element, low, mid1 - 1);
    if (element > this->data[mid2])
        return sort_find_ternary(element, mid2 + 1, high);
    return sort_find_ternary(element, mid1 + 1, mid2 - 1);
}//시간복잡도 O(2log3N) 공간복잡도 O(log3N)

template <typename T, size_t N>
void Array<T, N>::sort_insert(const T element){
    if(this->data[N-1] != 0) {
        throw std::out_of_range("array is full");
    }
    int i;
    for(i = N-2; (i >= 0 && data[i] > element); i--){
        this->data[i+1] = this->data[i];
    }
    this->data[i+1] = element;
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
void Array<T, N>::sort_remove(const T element){
    int index = sort_find_binary(element, 0, N);
    if(index == -1) return;
    for(int i = index; i < N-1; i++){
        this->data[i] = this->data[i+1];
    }
    this->data[N-1] = DefaultValue<T>::value;
}//시간복잡도 O(N) 공간복잡도 O(1)


#endif //ARRAY_SORTED_H
