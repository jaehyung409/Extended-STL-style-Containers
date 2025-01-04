//
// Created by user on 24. 5. 29.
//

#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include "Default_Value.h"

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
    int find(T element){
        int i;
        for(i = 0; i < N; i++){
            if (this->data[i] == element)
                break;
        }
        return i != N ? i : -1;
    }// time complexity O(N) space complexity O(1)
    void insert(T element, int index){
        if(index >= N) {
            throw std::out_of_range("index out_of_range");
        }
        for(int i = N-2; i >= index; i--){
            this->data[i+1] = this->data[i];
        }
        this->data[index] = element;
    }// time complexity O(N) space complexity O(1)
    void remove(T element){
        const int index = this->find(element);
        if(index == -1) return;
        for(int i = index; i < N-1; i++){
            this->data[i] = this->data[i+1];
        }
        this->data[N-1] = DefaultValue<T>::value;
    }// time complexity O(N) space complexity O(1)
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
};

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
    if (element < this->data[mid1])
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

#endif // ARRAY_H

