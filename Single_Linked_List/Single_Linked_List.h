//
// Created by jaehy on 2024-06-14.
//

#ifndef SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H

#include <type_traits>

template <typename T>
struct DefaultValue {
    static constexpr T value = std::conditional_t<std::is_arithmetic<T>::value, std::integral_constant<T, 0>, std::integral_constant<T*, nullptr>>::value;
};

template <typename T>
struct Node{
    T data;
    Node* next;
};

template <typename T>
struct SingleLinkedList{
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    SingleLinkedList(){
        head = new Node<T>;
        head->data = DefaultValue<T>::value;
        tail = nullptr;
        head->next = tail;
        size = 0;
    }
    int get_size(){
        return this->size;
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
    void insert(int index, T data);
    bool search_iterative(T data);
    bool search_recursive(Node<T>* node, T data);
    int get_size_iterative();
    int get_size_recursive(Node<T>* node);
    int get_size_tail_recursive(Node<T>* node, int get_size = 0);
};

#endif //SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
