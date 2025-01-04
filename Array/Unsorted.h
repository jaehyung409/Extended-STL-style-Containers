//
// Created by user on 24. 5. 29.
//

#ifndef ARRAY_UNSORTED_H
#define ARRAY_UNSORTED_H

#include "Array.h"

template <typename T, size_t N>
int Array<T, N>::find(const T element){
    int i;
    for(i = 0; i < N; i++){
        if (this->data[i] == element)
            break;
    }
    return i != N ? i : -1;
}// 시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
void Array<T, N>::insert(const T element, const int index){
    if(index >= N) {
        throw std::out_of_range("index out_of_range");
    }
    for(int i = N-2; i >= index; i--){
        this->data[i+1] = this->data[i];
    }
    this->data[index] = element;
}// 시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
void Array<T, N>::remove(const T element){
    const int index = this->find(element);
    if(index == -1) return;
    for(int i = index; i < N-1; i++){
        this->data[i] = this->data[i+1];
    }
    this->data[N-1] = DefaultValue<T>::value;
}//시간복잡도 O(N) 공간복잡도 O(1)


#endif //ARRAY_UNSORTED_H
