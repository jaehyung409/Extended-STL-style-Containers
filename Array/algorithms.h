//
// Created by jaehy on 2024-06-13.
//

#ifndef ARRAY_ALGORITHMS_H
#define ARRAY_ALGORITHMS_H

#include <unordered_set>
#include "Array.h"

// algorithm with T = <int>

template <typename T, size_t N>
void Array<T, N>::get_leader() {
    if(this->size() == 0) return;
    T leader[this->size()];
    int idx = 0;
    leader[idx] = this->data[size()-1];
    for(int i = this->size()-2; i >= 0; i--){
        if(this->data[i] > leader[idx]){
            leader[++idx] = this->data[i];
        }
    }
    for(; idx >= 0;){
        std::cout << leader[idx--] << ' ';
    }
    std::cout << '\n';
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
bool Array<T, N>::pair_sum_unsorted(T sum) {
    if(this->size() == 0) return false;
    for (int i = 0; i < size(); i++) {
        for (int j = i + 1; j < size(); j++) {
            if (this->data[i] + this->data[j] == sum) {
                std::cout << this->data[i] << ' ' << this->data[j] << '\n';
                return true;
            }
        }
    }
    return false;
}//시간복잡도 O(N^2) 공간복잡도 O(1)

template <typename T, size_t N>
bool Array<T, N>::pair_sum_unsorted_using_remainders(T sum) {
    if (size() == 0) return false;
    T remainders[sum];
    int i;
    for (i = 0; i < sum; i++){
        remainders[i] = 0;
    }
    for (i = 0; i < this->size(); i++) {
        if (this->data[i] < sum)
            remainders[this->data[i] % sum]++;
    }
    for (i = 1; i < sum /2; i++) {
        if (remainders[i] && remainders[sum - i]) {
            std::cout << i << ' ' << sum - i << '\n';
            return true;
        }
    }
    if (i >= sum/2){
        if(this->size() % 2 == 0 && remainders[sum/2] >= 2){
            std::cout << sum/2 << ' ' << sum/2 << '\n';
            return true;
        }
        else if(this->size() % 2 == 1 && remainders[sum/2] && remainders[sum - sum/2]){
            std::cout << sum/2 << ' ' << sum - sum/2 << '\n';
            return true;
        }
    }
    return false;
}//시간복잡도 O(N) 공간복잡도 O(N)

template <typename T, size_t N>
bool Array<T, N>::pair_sum_sorted_two_point(T sum) {
    if (this->size() == 0) return false;
    int left = 0;
    int right = this->size() - 1;
    while (left < right) {
        if (this->data[left] + this->data[right] == sum) {
            std::cout << this->data[left] << ' ' << this->data[right] << '\n';
            return true;
        } else if (this->data[left] + this->data[right] < sum) {
            left++;
        } else {
            right--;
        }
    }
    return false;
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T, size_t N>
bool Array<T, N>::pair_sum_sorted_binary_search(T sum) {
    if (this->size() == 0) return false;
    for(int i = 0; i < this->size(); i++){
        int index = sort_find_binary(sum - this->data[i], i + 1, this->size() - 1);
        if(index != -1){
            std::cout << this->data[i] << ' ' << this->data[index] << '\n';
            return true;
        }
    }
    return false;
}//시간복잡도 O(NlogN) 공간복잡도 O(1)

template <typename T, size_t N>
bool Array<T, N>::pair_sum_sorted_hashing(T sum) {
    if (this->size() == 0) return false;
    std::unordered_set<T> s;
    for(int i = 0; i < this->size(); i++){
        if(s.find(sum - this->data[i]) != s.end()){
            std::cout << this->data[i] << ' ' << sum - this->data[i] << '\n';
            return true;
        }
        s.insert(this->data[i]);
    }
    return false;
}//시간복잡도 O(N) 공간복잡도 O(N)

#endif //ARRAY_ALGORITHMS_H
