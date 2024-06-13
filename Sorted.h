//
// Created by user on 24. 5. 29.
//

#ifndef JAEHYUNG_SORTED_H
#define JAEHYUNG_SORTED_H

#include <Array.h>

template <typename T, size_t N>
int Array<T, N>::sort_find(const T element, const int low, const int high){
    if (high < low)
        return -1;
    int mid = (low + high) / 2;
    if (element == data[mid]) return mid;
    if (element > data[mid])
        return sort_find(element, mid + 1, high);
    return sort_find(element, low, mid - 1);
}//시간복잡도 O(logN) 공간복잡도 O(logN)

void Array<T, N>::sort_insert(const T element){
    if(data[N-1] != 0) {
        throw std::out_of_range("array is full");
    }
    int i;
    for(i = N-2; (i >= 0 && data[i] > element); i--){
        data[i+1] = data[i];
    }
    data[i+1] = element;
}//시간복잡도 O(N) 공간복잡도 O(1)

void Array<T, N>::sort_remove(const T element){
    int index = sort_find(element, 0, N);
    if(index == -1) return;
    for(int i = index; i < N-1; i++){
        data[i] = data[i+1];
    }
    data[N-1] = 0;
}//시간복잡도 O(N) 공간복잡도 O(1)


#endif //JAEHYUNG_SORTED_H
