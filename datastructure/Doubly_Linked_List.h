//
// Created by jaehy on 25. 1. 7.
//

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "Default_Value.h"
#include <limits>
#include <iostream>

namespace DoublyLinkedListSpace {
    template<typename T>
    struct Node {
        T data;
        Node *next;
        Node *prev;
    };
}

template <typename T>
struct DoublyLinkedList {
private:
    DoublyLinkedListSpace::Node<T> *head; // head->next is the first node
    DoublyLinkedListSpace::Node<T> *tail; // tail is the last node
    int length;

public:
    DoublyLinkedList() {
        head = new DoublyLinkedListSpace::Node<T>;
        head->data = DefaultValue<T>::value;
        tail = nullptr;
        head->prev = head;
        head->next = tail;
        length = 0;
    }

    template<typename Container>
    DoublyLinkedList(Container &cont) : DoublyLinkedList() {
        for (auto it = cont.begin(); it != cont.end(); it++) {
            push_back(*it);
        }
    }

    ~DoublyLinkedList() {
        this->clear();
        delete head;
    }

    struct Iterator {
    private:
        DoublyLinkedListSpace::Node<T> *ptr;
    public:
        Iterator(DoublyLinkedListSpace::Node<T> *ptr) : ptr(ptr) {}

        T &operator*() {
            return ptr->data;
        }
        Iterator &operator++() {
            if (ptr) ptr = ptr->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        Iterator &operator--(){
            if (ptr) ptr = ptr->prev;
            return *this;
        }
        bool operator==(const Iterator &other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }
    };
    Iterator begin() {
        return Iterator(head->next);
    }
    Iterator end() {
        return Iterator(nullptr);
    }
    struct Reverse_Iterator : public Iterator{
        using Iterator::Iterator;
        Reverse_Iterator(Iterator i) : Iterator(i) {}

        Reverse_Iterator operator++(){
            Iterator::operator--();
            return *this;
        }
        Reverse_Iterator operator++(int){
            Reverse_Iterator temp = *this;
            Iterator::operator--();
            return temp;
        }
    };
    Reverse_Iterator rbegin(){
        if (this->tail) return Reverse_Iterator(this->tail);
        return Reverse_Iterator(this->head);
    }
    Reverse_Iterator rend(){
        return Reverse_Iterator(this->head);
    }
    size_t size() {
        return this->length;
    }
    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(DoublyLinkedListSpace::Node<T>);
    }
    DoublyLinkedListSpace::Node<T> *get_head() {
        return this->head;
    }
    DoublyLinkedListSpace::Node<T> *get_tail() {
        return this->tail;
    }
    void print() {
        for (auto it = begin(); it != end(); it++) {
            std::cout << *it << ' ';
        }
        std::cout << '\n';
    }
    void print_reverse() {
        for (auto it = rbegin(); it != rend(); it++) {
            std::cout << *it << ' ';
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
    bool empty() {
        return this->size() == 0;
    }
    void clear();
};

template <typename T>
void DoublyLinkedList<T>::push_front(T data){
    this->insert(0, data);
}//time complexity = O(1), space complexity = O(1)

template <typename T>
void DoublyLinkedList<T>::push_back(T data){
    auto* new_node = new DoublyLinkedListSpace::Node<T>;
    new_node->data = data;
    new_node->next = nullptr;
    new_node->prev =this->tail;
    this->tail->next = new_node;
    this->tail = new_node;
    this->length++;
}//time complexity = O(1), space complexity = O(1)

template <typename T>
void DoublyLinkedList<T>::pop_front() {
    this->erase(0);
}//time complexity = O(1), space complexity = O(1)

template <typename T>
void DoublyLinkedList<T>::pop_back() {
    if (this->empty()) throw std::out_of_range("pop_back() called on empty list");
    auto* new_tail = this->tail->prev;
    new_tail->prev = this->tail->prev->prev;
    delete this->tail;
    new_tail->next = nullptr;
    this->tail = new_tail;
    this->length--;
}//time complexity = O(1), space complexity = O(1)

template <typename T>
T DoublyLinkedList<T>::front() {
    if (this->empty()) throw std::out_of_range("front() called on empty list");
    return this->get_head()->next->data;
}//time complexity O(1), space complexity O(1)

template <typename T>
T DoublyLinkedList<T>::back() {
    if (this->empty()) throw std::out_of_range("back() called on empty list");
    return this->get_tail()->data;
}//time complexity O(1), space complexity O(1)

template <typename T>
void DoublyLinkedList<T>::insert(int index, T data) {
    if (index < 0 || index > this->size()){
        throw std::out_of_range("index out_of_range");
    }
    auto* new_node = new DoublyLinkedListSpace::Node<T>;
    new_node->data = data;
    DoublyLinkedListSpace::Node<T>* current = this->get_head();
    for (int i = 0; i < index; i++){
        current = current->next;
    }
    if (current == this->get_tail() || this->size() == 0) this->tail = new_node;
    new_node->next = current->next;
    if (current->next) current->next->prev = new_node;
    current->next = new_node;
    new_node->prev = current;
    this->length++;
}

template <typename T>
void DoublyLinkedList<T>::erase(int index) {
    if (this->empty()) throw std::out_of_range("erase() called on empty list");
    if (index >= this->size() || index < 0) throw std::out_of_range("erase() called on out of range index");
    DoublyLinkedListSpace::Node<T>* temp = this->get_head();
    for (int i = 0; i < index; i++) temp = temp->next;
    if (temp->next == this->get_tail()) this->tail = temp;
    DoublyLinkedListSpace::Node<T>* node_to_delete = temp->next;
    temp->next = node_to_delete->next;
    if (node_to_delete->next) node_to_delete->next->prev = temp;
    delete node_to_delete;
    this->length--;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while (!this->empty()){
        this->pop_front();
    }
}

#endif // DOUBLY_LINKED_LIST_H
