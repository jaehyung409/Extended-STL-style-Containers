//
// Created by jaehy on 2024-06-14.
//

#ifndef SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H

#include "Default_Value.h"
#include <limits>

template <typename T>
struct Node{
    T data;
    Node* next;
};

template <typename T>
struct SingleLinkedList{
private:
    Node<T>* head; // head->next is the first node
    Node<T>* tail; // tail is the last node
    int length;

public:
    SingleLinkedList(){
        head = new Node<T>;
        head->data = DefaultValue<T>::value;
        tail = nullptr;
        head->next = tail;
        length = 0;
    }
    size_t size(){
        return this->length;
    }
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max() / sizeof(Node<T>); }
    Node<T>* get_head(){
        return this->head;
    }
    Node<T>* get_tail(){
        return this->tail;
    }
    void print(){
        Node<T>* current = head->next;
        while(current != nullptr){
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }
    void push_front(T data);
    void push_back(T data);
    void pop_front();
    void pop_back();
    T front();
    T back();
    void insert(int index, T data);
    void erase(int index);
    bool empty(){
        return this->size() == 0;
    }

    bool search_iterative(T data);
    bool search_recursive(Node<T>* node, T data);
    int get_size_iterative();
    int get_size_recursive(Node<T>* node);
    int get_size_tail_recursive(Node<T>* node, int get_size = 0);
};

template <typename T>
int SingleLinkedList<T>::get_size_iterative() {
    Node<T>* temp = head;
    int get_size = 0;
    while(temp->next){
        temp = temp->next;
        get_size++;
    }
    return get_size;
}//시간복잡도 O(N), 공간복잡도 O(1)

template <typename T>
int SingleLinkedList<T>::get_size_recursive(Node<T>* node) {
    if (node == nullptr) return -1; //head 부터 세므로 -1
    return get_size_recursive(node->next) + 1;
}//시간복잡도 O(N), 공간복잡도 O(N)

template <typename T>
int SingleLinkedList<T>::get_size_tail_recursive(Node<T>* node, int get_size) {
    if (node == nullptr) return get_size - 1; //head 부터 세므로 -1
    return get_size_tail_recursive(node->next, get_size + 1);
}//시간복잡도 O(N), 공간복잡도 O(1) when tail call optimization (if not O(N))

template <typename T>
void SingleLinkedList<T>::push_front(T data){
    this->insert(0, data);
}//시간복잡도 O(1) 공간복잡도 O(1)

template <typename T>
void SingleLinkedList<T>::push_back(T data){
    this->insert(this->size(), data);
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T>
void SingleLinkedList<T>::insert(int index, T data){
    if (index < 0 || index > this->size()){
        throw std::out_of_range("index out_of_range");
    }
    auto* new_node = new Node<T>;
    new_node->data = data;
    Node<T>* current = this->get_head();
    for (int i = 0; i < index; i++){
        current = current->next;
    }
    if (current == this->get_tail() || this->size() == 0) this->tail = new_node;
    new_node->next = current->next;
    current->next = new_node;
    this->length++;
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T>
bool SingleLinkedList<T>::search_iterative(T data) {
    Node<T>* temp = this->get_head();
    while (temp->next){
        temp = temp->next;
        if (data == temp->data)
            return true;
    }
    return false;
}//시간복잡도 O(N), 공간복잡도 O(1)

template <typename T>
bool SingleLinkedList<T>::search_recursive(Node<T>* node, T data) {
    if (node == nullptr) return false;
    if (node->data == data) return true;
    return search_recursive(node->next, data);
}//시간복잡도 O(N), 공간복잡도 O(N)

template <typename T>
void SingleLinkedList<T>::erase(int index) {
    if (this->size() == 0) throw std::out_of_range("erase() called on empty list");
    if (index >= this->size() || index < 0) throw std::out_of_range("erase() called on out of range index");
    Node<T>* temp = this->get_head();
    for (int i = 0; i < index; i++) temp = temp->next;
    if (temp->next == this->get_tail()) this->tail = temp;
    Node<T>* node_to_delete = temp->next;
    temp->next = node_to_delete->next;
    delete node_to_delete;
    this->length--;
}// time complexity O(N), space complexity O(1)

template <typename T>
void SingleLinkedList<T>::pop_front() {
    if (this->size() == 0) throw std::out_of_range("pop_front() called on empty list");
    this->erase(0);
}//time complexity O(1), space complexity O(1)

template <typename T>
void SingleLinkedList<T>::pop_back() {
    if (this->size() == 0) throw std::out_of_range("pop_back() called on empty list");
    this->erase(this->size() - 1);
}// time complexity O(N), space complexity O(1)

template <typename T>
T SingleLinkedList<T>::front() {
    if (this->size() == 0) throw std::out_of_range("front() called on empty list");
    return this->get_head()->next->data;
}//time complexity O(1), space complexity O(1)

template <typename T>
T SingleLinkedList<T>::back() {
    if (this->size() == 0) throw std::out_of_range("back() called on empty list");
    return this->get_tail()->data;
}//time complexity O(1), space complexity O(1)

#endif //SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
