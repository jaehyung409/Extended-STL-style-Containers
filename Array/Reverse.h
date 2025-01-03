//
// Created by jaehy on 2024-06-13.
//

#ifndef ARRAY_REVERSE_H
#define ARRAY_REVERSE_H

#include "Array.h"

template <typename T, size_t N>
void Array<T, N>::reverse_array_extra_array(){
    T temp[this->size()];
    for(int i = 0; i < this->size(); i++){
        temp[i] = this->data[this->size()-1-i];
    }
    for(int i = 0; i < this->size(); i++){
        this->data[i] = temp[i];
    }
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
void Array<T, N>::reverse_array_two_pointer(){
    int left = 0;
    int right = this->size() - 1;
    while(left < right){
        T temp = this->data[left];
        this->data[left++] = this->data[right];
        this->data[right--] = temp;
    }
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
void Array<T, N>::reverse_array_recursive(int left, int right) {
    if (left >= right) return;
    T temp = this->data[left];
    this->data[left] = this->data[right];
    this->data[right] = temp;
    reverse_array_recursive(left+1, right-1);
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
void Array<T, N>::reverse_array_stack() {
    T stack[this->size()];
    int top = -1;
    for(int i = 0; i < this->size(); i++){
        stack[++top] = this->data[i];
    }
    for(int i = 0; i < this->size(); i++){
        this->data[i] = stack[top--];
    }
}//시간복잡도 O(N) 공간복잡도 O(N)
#endif //ARRAY_REVERSE_H
