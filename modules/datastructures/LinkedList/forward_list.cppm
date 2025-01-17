/*
 * @ Created by jaehyung409 on 25. 1. 16.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <limits>
#include <memory>
#include <utility>
#include <iterator>

export module forward_list;

import default_value;

namespace j {
    export template <class T, class Allocator = std::allocator<T>>
    class Forward_list {
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = size_t;
        using iterator = typename std::iterator_traits<value_type*>::iterator;
        using const_iterator = const iterator;

    private:
        class Node {
            T value;
            Node* next;

            Node() : value(Default_value<T>::value), next(nullptr) {}
            explicit Node(const T& value) : value(value), next(nullptr) {}
            explicit Node(T&& value) : value(std::move(value)), next(nullptr) {}
        };

        Node* before_head; // node for before_begin();
        Node* head;
        Allocator alloc;
        size_type size;

    public:
        // constructor and destructor
        Forward_list() : head(nullptr), alloc(Allocator()), size(0) {
            before_head = alloc.allocate(1);
            alloc.construct(before_head, Node());
            before_head->next = head;
        }
        explicit Forward_list(const Allocator& alloc);
        explicit Forward_list(size_type n, const Allocator& alloc = Allocator()); // n size forward_list with default value of T.
        Forward_list(size_type n, const T& value, const Allocator& alloc = Allocator()); // n size forward_list with value.
        //template<class InputIter>
        //forward_list(InputIter first, InputIter last, const Allocator& = Allocator());
        Forward_list(const Forward_list& x);
        Forward_list(Forward_list&& x) noexcept;
        //forward_list(const forward_list& x, const std::type_identity_t<Allocator>&);
        //forward_list(forward_list&& x, const std::type_identity_t<Allocator>&);
        //forward_list(std::initializer_list<T>, const Allocator& = Allocator());
        ~Forward_list();

        // member functions
        Forward_list& operator=(const Forward_list& x);
        Forward_list& operator=(Forward_list&& x) noexcept;
        //forward_list& operator=(initializer_list<T>);
        void assign(size_type count, const T& value);
        //template <class InputIt>
        //void assign(InputIt first, InputIt last);
        //void assign(std::initializer_list<T> ilist);
        allocator_type get_allocator() const noexcept;

        // Iterator -> Use std::iterator before I make iterator
        iterator before_begin() noexcept;
        const_iterator before_begin() const noexcept;
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cbefore_begin() const noexcept;
        const_iterator cend() const noexcept;


        // Capacity
        bool empty() const noexcept;
        size_type max_size() const noexcept;

        // Accessor
        T& front();
        const T& front() const;

        // modifiers
        iterator insert_after(const_iterator pos, const T& value);
        iterator insert_after(const_iterator pos, T& value);
        iterator insert_after(const_iterator pos, size_type count, const T& value);
        template <class... Args>
        iterator emplace_after(const_iterator pos, Args&&... args);
        iterator erase_after(const_iterator pos);
        //iterator erase_after(const_iterator first, const_iterator last);
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

        void splice_after(const_iterator pos, Forward_list& other);
        void splice_after(const_iterator pos, Forward_list&& other);

        void sort();
        template <class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
    };

    export template<class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x,
              Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y)));

    // erasure
    export template<class T, class Allocator, class U = T>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c,
                                                         const U& value);
    export template<class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c,
                                                            Predicate pred);

}

namespace j {
    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Allocator& alloc): head(nullptr), alloc(alloc), size(0) {
        before_head = alloc.allocate(1);
        alloc.construct(before_head, Node());
        before_head->next = head;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const Allocator& alloc) : head(nullptr), alloc(alloc), size(n) {
        before_head = alloc.allocate(1);
        alloc.construct(before_head, Node());
        before_head->next = head;
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
    Forward_list<T, Allocator>::Forward_list(const size_type n, const T& value, const Allocator& alloc) : head(nullptr), alloc(alloc), size(n){
        before_head = alloc.allocate(1);
        alloc.construct(before_head, Node());
        before_head->next = head;
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
    Forward_list<T, Allocator>::Forward_list(const Forward_list& x) : head(nullptr), alloc(x.alloc), size(x.size) {
        before_head = alloc.allocate(1);
        alloc.construct(before_head, Node());
        before_head->next = head;
        if (!x.head) return;

        head = alloc.allocate(1);
        alloc.construct(head, Node(x.head->value));

        Node* ptr = head;
        Node* x_node = x.head->next;

        while (x_node){
            ptr->next = alloc.allocate(1);
            alloc.construct(ptr->next, Node(x_node->value));
            ptr = ptr->next;
            x_node = x_node->next;
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(Forward_list&& x)  noexcept
        : before_head(std::move(x.before_head)), head(std::move(x.head)), alloc(std::move(x.alloc)), size(x.size) {
        x.head = nullptr;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::~Forward_list() {
        clear();
        delete before_head;
        delete head;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(const Forward_list& x) {
        if (this != &x) {
            clear();
            auto it = x.begin();
            while (it != x.end()) {
                insert_after(before_begin(), *it);
                ++it;
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(Forward_list&& x) noexcept {
        if (this != &x) {
            clear();
            head = std::move(x.head);
            before_head = std::move(x.before_head);
            x.head = nullptr;
        }
        return *this;
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::assign(Forward_list::size_type count, const T &value) {
        clear();
        for (int i = 0; i < count; i++)
            push_front(value);
    }

    template<class T, class Allocator>
    Allocator Forward_list<T, Allocator>::get_allocator() const noexcept {
        return alloc;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::before_begin() noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::before_begin() const noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::begin() noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::begin() const noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::end() noexcept {
        return iterator(nullptr);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::end() const noexcept {
        return iterator(nullptr);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbegin() const noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbefore_begin() const noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cend() const noexcept {
        return iterator(nullptr);
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
        if (empty()) throw std::out_of_range("front() called on empty forward_list");
        return head->value;
    }

    template <class T, class Allocator>
    const T& Forward_list<T, Allocator>::front() const {
        if (empty()) throw std::out_of_range("front() called on empty forward_list");
        return head->value;
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, const T &value) {
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(value));
        new_node->next=pos->next;
        pos->next = new_node;
        size++;
        return iterator(new_node);
    }


    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, T &value) {
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(value));
        new_node->next=pos->next;
        pos->next = new_node;
        size++;
        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, Forward_list::size_type count,
                                             const T &value) {
        iterator it = nullptr;
        for (int i = 0; i < count; i++)
            it = insert_after(pos, value);
        return it;
    }

    template<class T, class Allocator>
    template<class... Args>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::emplace_after(Forward_list::const_iterator pos, Args &&... args) {
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(T(std::forward<Args>(args)...)));
        new_node->next=pos->next;
        pos->next = new_node;
        size++;
        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::erase_after(Forward_list::const_iterator pos) {
        Node* node_to_delete = pos->next;
        pos->next = node_to_delete->next;
        delete node_to_delete;
        size--;
        return pos->next;
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
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(value));
        new_node->next = head;
        head = new_node;
        size++;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(T&& value) {
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, Node(value));
        new_node->next = head;
        head = new_node;
        size++;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::pop_front() {
        Node* del_node = head;
        head = head->next;
        delete del_node;
        size--;
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
