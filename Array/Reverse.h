//
// Created by jaehy on 2024-06-13.
//

#ifndef ARRAY_REVERSE_H
#define ARRAY_REVERSE_H

#include "Array.h"

template <typename T, size_t N>
void Array<T, N>::reverse_array_extra_array(){
    T temp[size()];
    for(int i = 0; i < size(); i++){
        temp[i] = data[size()-1-i];
    }
    for(int i = 0; i < size(); i++){
        data[i] = temp[i];
    }
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
void Array<T, N>::reverse_array_two_pointer(){
    int left = 0;
    int right = size() - 1;
    while(left < right){
        T temp = data[left];
        data[left++] = data[right];
        data[right--] = temp;
    }
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
void Array<T, N>::reverse_array_recursive(int left, int right) {
    if (left >= right) return;
    T temp = data[left];
    data[left] = data[right];
    data[right] = temp;
    reverse_array_recursive(left+1, right-1);
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
void Array<T, N>::reverse_array_stack() {
    T stack[size()];
    int top = -1;
    for(int i = 0; i < size(); i++){
        stack[++top] = data[i];
    }
    for(int i = 0; i < size(); i++){
        data[i] = stack[top--];
    }
}//시간복잡도 O(N) 공간복잡도 O(N)
#endif //ARRAY_REVERSE_H
