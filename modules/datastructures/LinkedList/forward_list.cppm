/*
 * @ Created by jaehyung409 on 25. 1. 16.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <limits>
#include <memory>
#include <utility>

export module forward_list;

import default_value;

namespace j {
    export template <class T, class Allocator = std::allocator<T>>
    class Forward_list {
        using size_type = size_t;
    private:
        class Node {
            T value;
            Node* next;

            Node() : value(Default_value<T>::value), next(nullptr) {}
            explicit Node(const T& value) : value(value), next(nullptr) {}
        };

        Node* head;
        Allocator alloc;
        size_type size;

    public:
        Forward_list() : head(nullptr), alloc(Allocator()), size(0) {}
        explicit Forward_list(const Allocator&);
        explicit Forward_list(size_type n, const Allocator& = Allocator()); // n size forward_list with default value of T.
        Forward_list(size_type n, const T& value, const Allocator& = Allocator()); // n size forward_list with value.
        //template<class InputIter>
        //forward_list(InputIter first, InputIter last, const Allocator& = Allocator());
        Forward_list(const Forward_list& x);
        Forward_list(Forward_list&& x);
        //forward_list(const forward_list& x, const std::type_identity_t<Allocator>&);
        //forward_list(forward_list&& x, const std::type_identity_t<Allocator>&);
        //forward_list(std::initializer_list<T>, const Allocator& = Allocator());
        ~Forward_list();
        Forward_list& operator=(const Forward_list& x);
        Forward_list& operator=(Forward_list&& x) noexcept;
        //forward_list& operator=(initializer_list<T>);

        // Capacity
        bool empty() const noexcept;
        size_type max_size() const noexcept;

        // Accessor
        T& front();
        const T& front() const;

        // modifiers
        template <class... Args>
        T& emplace_front(Args&&... args);
        void push_front(const T& value);
        void push_front(T&& value);
        void pop_front();

        void resize(size_type nsz);
        void resize(size_type nsz, const T& value);
        void clear() noexcept;

        size_type remove(const T& value);
        //template <class Predicate>
        //size_type remove_if(Predicate pred);
        size_type unique();

        void merge(Forward_list& x);
        void merge(Forward_list&& x);
        template <class Compare>
        void merge(Forward_list& x, Compare comp);
        template <class Compare>
        void merge(Forward_list&& x, Compare comp);

        void sort();
        template <class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
    };
    template<class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x,
              Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y)));

    // erasure
    template<class T, class Allocator, class U = T>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c,
                                                         const U& value);
    template<class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c,
                                                            Predicate pred);

}

namespace j {
    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Allocator&): head(nullptr), alloc(alloc), size(0) {}

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const Allocator&) : head(nullptr), alloc(alloc), size(n) {
        if (n > 0) {
            head = alloc.allocate(1);
            alloc.construct(head, Node());

            Node* prev = head;
            for (int i = 1; i < n; i++) {
                Node* ptr = alloc.allocate(1);
                alloc.construct(ptr, Node());
                prev->next = ptr;
                prev = ptr;
            }
            prev->next = nullptr;
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const T& value, const Allocator&) : head(nullptr), alloc(alloc), size(n){
        if (n > 0) {
            head = alloc.allocate(1);
            alloc.construct(head, Node(value));

            Node* prev = head;
            for (int i = 1; i < n; i++) {
                Node* ptr = alloc.allocate(1);
                alloc.construct(ptr, Node(value));
                prev->next = ptr;
                prev = ptr;
            }
            prev->next = nullptr;
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Forward_list& x) {
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(Forward_list&& x) {
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::~Forward_list() {
        clear();
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(const Forward_list& x) {
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(Forward_list&& x) noexcept {
    }

    template <class T, class Allocator>
    bool Forward_list<T, Allocator>::empty() const noexcept {
        return size == 0;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(Node);
    }

    template <class T, class Allocator>
    T& Forward_list<T, Allocator>::front() {
        if (empty()) throw std::out_of_range("front() called on empty fowrard_list");
        return head->value;
    }

    template <class T, class Allocator>
    const T& Forward_list<T, Allocator>::front() const {
        if (empty()) throw std::out_of_range("front() called on empty fowrard_list");
        return head->value;
    }

    template <class T, class Allocator>
    template <class ... Args>
    T& Forward_list<T, Allocator>::emplace_front(Args&&... args) {
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(T(std::forward<Args>(args)...)));
        new_node->next = head;
        head = new_node;
        size++;
        return head;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(const T& value) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(T&& value) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::pop_front() {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz, const T& value) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::clear() noexcept {
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::remove(const T& value) {
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::unique() {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::merge(Forward_list& x) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::merge(Forward_list&& x) {
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::merge(Forward_list& x, Compare comp) {
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::merge(Forward_list&& x, Compare comp) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::sort() {
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::sort(Compare comp) {
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::reverse() noexcept {
    }

    template <class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x, Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
    }

    template <class T, class Allocator, class U>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c, const U& value) {
    }

    template <class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c, Predicate pred) {
    }
}
