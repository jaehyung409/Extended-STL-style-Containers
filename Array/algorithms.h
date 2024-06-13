//
// Created by jaehy on 2024-06-13.
//

#ifndef ARRAY_ALGORITHMS_H
#define ARRAY_ALGORITHMS_H

#include "Array.h"

template <typename T, size_t N>
void Array<T, N>::get_leader() {
    if(size() == 0) return;
    T leader[size()];
    int idx = 0;
    leader[idx] = data[size()-1];
    for(int i = size()-2; i >= 0; i--){
        if(data[i] > leader[idx]){
            leader[++idx] = data[i];
        }
    }
    for(; idx >= 0;){
        std::cout << leader[idx--] << ' ';
    }
    std::cout << '\n';
}

#endif //ARRAY_ALGORITHMS_H
