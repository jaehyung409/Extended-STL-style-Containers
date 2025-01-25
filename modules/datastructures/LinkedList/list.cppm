/*
 * @ Created by jaehyung409 on 25. 1. 24.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <limits>
#include <memory>
#include <iterator>
#include <initializer_list>

export module list;

import default_value;

namespace j {
    template<class U>
    class List_node {
        template <class T, class Allocator>
        friend class List;
        template <class Node>
        friend class Bidirectional_iterator;
        template<class Node>
        friend class const_Bidirectional_iterator;

    public:
        using value_type = U;

    private:
        U value;
        mutable List_node *next;
        mutable List_node *prev;

    public:
        List_node() : value(Default_value<U>().get().value_or(U())), next(nullptr), prev(nullptr) {}
        explicit List_node(const U &value) : value(value), next(nullptr), prev(nullptr) {}
        explicit List_node(U &&value) : value(std::move(value)), next(nullptr), prev(nullptr) {}
        template <class... Args, class = std::enable_if_t<std::is_constructible_v<U, Args&&...>>>
        explicit List_node(Args&&... args) : value(std::forward<Args>(args)...), next(nullptr), prev(nullptr) {}
        ~List_node() = default;

        U& operator*() { return value; }
        const U& operator*() const { return value; }
    };

    export template<class Node>
    class const_Bidirectional_iterator;

    export template<class Node>
    class Bidirectional_iterator;

    export template<class T, class Allocator = std::allocator <T>>
    class List {
    public:
        using value_type = T;
        using allocator_type = std::allocator<T>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using Node = List_node<T>;
        using size_type = size_t;
        using iterator = Bidirectional_iterator<Node>;
        using const_iterator = const_Bidirectional_iterator<const Node>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node *head;
        Node *tail;         // tail is a dummy node (sentinel)
        node_allocator node_alloc;
        size_type _size;

        // helper function (getter and setter)
        Node* get_head() noexcept {
            return head;
        }
        Node* get_tail() noexcept {
            return tail;
        }
        void set_head(Node *new_head) noexcept {
            head = new_head;
        }
        void set_tail(Node *new_tail) noexcept {
            tail = new_tail;
        }
        void set_size(size_type new_size) noexcept {
            _size = new_size;
        }

        // helper function (sort)
        template<class Compare>
        void merge_sort(List& x, Compare comp);

    public:
        // constructor and destructor
        List() : head(nullptr), tail(nullptr), node_alloc(Allocator()), _size(0) {
            tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
            tail->next = tail;
            tail->prev = tail;
            head = tail;
        }

        explicit List(const Allocator& alloc);
        explicit List(size_type n, const Allocator& alloc = Allocator());
        List(size_type n, const T& value, const Allocator& alloc = Allocator());
        //template <class InputIter>
        //List(InputIter first, InputIter last, const Allocator& alloc = Allocator());
        //template <container-compatible-range<T> R>
        //list(from_range_t, R&& range, const Allocator & = Allocator());
        List(const List& x);
        List(List&& x);
        List(const List& x, const std::type_identity_t<Allocator>& alloc);
        List(List&& x, const std::type_identity_t<Allocator>& alloc);
        List(std::initializer_list<T> il, const Allocator& alloc = Allocator());
        ~List();
        List& operator=(const List& x);
        List& operator=(List&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
        List& operator=(std::initializer_list<T> il);
        //template<class InputIter>
        //void assign(InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //void assign_range(R&& rg);
        void assign(size_type n, const T& t);
        //void assign(initializer_list<T>);
        allocator_type get_allocator() const noexcept;

        // iterators
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        // capacity
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;
        void resize(size_type sz);
        void resize(size_type sz, const T& c);

        // element access
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        // modifiers
        template<class... Args>
        reference emplace_front(Args&&... args);
        template<class... Args>
        reference emplace_back(Args&&... args);
        void push_front(const T& x);
        void push_front(T&& x);
        //template<container-compatible-range<T> R>
        //void prepend_range(R&& rg);
        void pop_front();
        void push_back(const T& x);
        void push_back(T&& x);
        //template<container-compatible-range<T> R>
        //void append_range(R&& rg);
        void pop_back();

        template<class... Args>
        iterator emplace(const_iterator position, Args&&... args);
        iterator insert(const_iterator position, const T& x);
        iterator insert(const_iterator position, T&& x);
        iterator insert(const_iterator position, size_type n, const T& x);
        //template<class InputIter>
        //iterator insert(const_iterator position, InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //iterator insert_range(const_iterator position, R&& rg);
        //iterator insert(const_iterator position, initializer_list<T> il);

        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);
        void swap(List& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
            std::swap(head, x.head);
            std::swap(tail, x.tail);
            std::swap(node_alloc, x.node_alloc);
            std::swap(_size, x._size);
        }
        void clear() noexcept;

        // list operations
        void splice(const_iterator position, List& x);
        void splice(const_iterator position, List&& x);
        void splice(const_iterator position, List& x, const_iterator i);
        void splice(const_iterator position, List&& x, const_iterator i);
        void splice(const_iterator position,
                    List& x,
                    const_iterator first,
                    const_iterator last);
        void splice(const_iterator position,
                    List&& x,
                    const_iterator first,
                    const_iterator last);

        size_type remove(const T& value);
        //template<class Predicate>
        //size_type remove_if(Predicate pred);

        size_type unique();
        //template<class BinaryPredicate>
        //size_type unique(BinaryPredicate binary_pred);

        void merge(List& x);
        void merge(List&& x);
        template<class Compare>
        void merge(List& x, Compare comp);
        template<class Compare>
        void merge(List&& x, Compare comp);

        void sort();
        template<class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
    };

    template<class Node>
    class Bidirectional_iterator {
        template<class T, class Allocator>
        friend class List;

        template<typename T>
        friend class const_Bidirectional_iterator;

    public:
        using value_type = typename Node::value_type;
        using node_pointer = Node*;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        node_pointer ptr;

    public:
        Bidirectional_iterator(node_pointer ptr = nullptr) : ptr(ptr) {}
        template<class OtherNode>
        Bidirectional_iterator(const const_Bidirectional_iterator<OtherNode>& other)
            : ptr(const_cast<node_pointer>(other.ptr)) {}

        reference operator*() { return const_cast<reference>(**ptr); }
        pointer operator->() { return &(**ptr); }

        Bidirectional_iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }

        Bidirectional_iterator operator++(int) {
            Bidirectional_iterator temp = *this;
            ++(*this);
            return temp;
        }

        Bidirectional_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        }

        Bidirectional_iterator operator--(int) {
            Bidirectional_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Bidirectional_iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const Bidirectional_iterator &other) const { return ptr != other.ptr; }
        operator const_Bidirectional_iterator<Node>() const { return const_Bidirectional_iterator(ptr); }
    };

    template<class Node>
    class const_Bidirectional_iterator  {
        template <class T, class Allocator>
        friend class List;
        template <typename T>
        friend class Bidirectional_iterator;

    public:
        using value_type = typename Node::value_type;
        using const_node_pointer = const Node*;
        using const_pointer = const value_type*;
        using const_reference = const value_type&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        const_node_pointer ptr;

    public:
        const_Bidirectional_iterator(const_node_pointer ptr = nullptr) : ptr(ptr) {}
        template <class OtherNode>
        const_Bidirectional_iterator(const Bidirectional_iterator<OtherNode>& other) : ptr(const_cast<const_node_pointer>(other.ptr)) {}

        const_reference operator*() const { return **ptr; }
        const_pointer operator->() const { return &(**ptr); }

        const_Bidirectional_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        const_Bidirectional_iterator operator++(int) {
            const_Bidirectional_iterator temp = *this;
            ++(*this);
            return temp;
        }

        const_Bidirectional_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        const_Bidirectional_iterator operator--(int) {
            const_Bidirectional_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const const_Bidirectional_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const const_Bidirectional_iterator& other) const { return ptr != other.ptr; }
    };

    export template<class T, class Allocator>
    void swap(List<T, Allocator>& x, List<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }
}

namespace j {
    template <class T, class Allocator>
    List<T, Allocator>::List(const Allocator& alloc) : List() {
        node_alloc = alloc;
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(size_type n, const Allocator& alloc) : List(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_front(Default_value<T>().get().value_or(T()));
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(size_type n, const T& value, const Allocator& alloc) : List(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_front(value);
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(const List& x) : List(std::allocator_traits<Allocator>::select_on_container_copy_construction(x.get_allocator())) {
        for (const T& t : x) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(List&& x) : head(x.get_head()), tail(x.get_tail()), node_alloc(std::move(x.get_allocator())), _size(std::move(x.size()))  {
        x.set_head(nullptr);
        x.set_tail(nullptr);
        x.set_size(0);
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(const List& x, const std::type_identity_t<Allocator>& alloc) : List(alloc) {
        auto xit = x.begin();
        for (const T& t : x) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(List&& x, const std::type_identity_t<Allocator>& alloc) : head(x.get_head()), tail(x.get_tail()), node_alloc(alloc), _size(std::move(x.size())) {
        x.set_head(nullptr);
        x.set_tail(nullptr);
        x.set_size(0);
    }

    template <class T, class Allocator>
    List<T, Allocator>::List(std::initializer_list<T> il, const Allocator& alloc) : List(alloc) {
        for (const T& t : il) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>::~List() {
        clear();
        std::allocator_traits<node_allocator>::destroy(node_alloc, tail);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, tail, 1);
    }

    template <class T, class Allocator>
    List<T, Allocator>& List<T, Allocator>::operator=(const List& x) {
        if (this != &x){
            clear();
            for (const T& t : x) {
                emplace_back(t);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    List<T, Allocator>& List<T, Allocator>::operator=(List&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &x){
            clear();
            head = x.get_head();
            tail = x.get_tail();
            node_alloc = std::move(x.get_allocator());
            _size = std::move(x.size());
            x.set_head(nullptr);
            x.set_tail(nullptr);
            x.set_size(0);
        }
    }

    template <class T, class Allocator>
    List<T, Allocator>& List<T, Allocator>::operator=(std::initializer_list<T> il) {
        clear();
        for (const T& t : il) {
            emplace_back(t);
        }
    }


    template <class T, class Allocator>
    void List<T, Allocator>::assign(size_type n, const T& t) {
        clear();
        for (size_type i = 0; i < n; ++i) {
            emplace_front(t);
        }
    }

    template <class T, class Allocator>
    std::allocator<T> List<T, Allocator>::get_allocator() const noexcept{
        return std::allocator<T>(node_alloc);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::begin() noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::begin() const noexcept {
        return const_iterator(head);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::end() noexcept {
        return iterator(tail);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::end() const noexcept {
        return const_iterator(tail);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::cbegin() const noexcept {
        return const_iterator(head);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::cend() const noexcept {
        return const_iterator(tail);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class T, class Allocator>
    bool List<T, Allocator>::empty() const noexcept {
        return _size == 0;
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::size() const noexcept {
        return _size;
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(Node) - 1; // -1 for tail
    }

    template <class T, class Allocator>
    void List<T, Allocator>::resize(size_type sz) {
        if (sz < _size) {
            for (size_type i = 0, n = _size - sz; i < n; ++i) {
                pop_back();
            }
        } else if (sz > _size) {
            for (size_type i = 0, n = sz - _size; i < n; ++i) {
                emplace_back(Default_value<T>().get().value_or(T()));
            }
        }
    }

    template <class T, class Allocator>
    void List<T, Allocator>::resize(size_type sz, const T& c) {
        if (sz < _size) {
            for (size_type i = 0; i < _size - sz; ++i) {
                pop_back();
            }
        } else if (sz > _size) {
            for (size_type i = 0; i < sz - _size; ++i) {
                emplace_back(c);
            }
        }
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::reference List<T, Allocator>::front() {
        if (empty()) {
            throw std::out_of_range("List::front() : list is empty");
        }
        return head->value;
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reference List<T, Allocator>::front() const {
        if (empty()) {
            throw std::out_of_range("List::front() : list is empty");
        }
        return head->value;
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::reference List<T, Allocator>::back() {
        if (empty()) {
            throw std::out_of_range("List::back() : list is empty");
        }
        return tail->prev->value;
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::const_reference List<T, Allocator>::back() const {
        if (empty()) {
            throw std::out_of_range("List::back() : list is empty");
        }
        return tail->prev->value;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename List<T, Allocator>::reference List<T, Allocator>::emplace_front(Args&&... args) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::forward<Args>(args)...));
        if (empty()) {
            new_node->next = tail;
            tail->prev = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
        }
        head = new_node;
        new_node->prev = new_node;
        _size++;
        return new_node->value;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename List<T, Allocator>::reference List<T, Allocator>::emplace_back(Args&&... args) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::forward<Args>(args)...));
        if (empty()) {
            head = new_node;
            new_node->prev = new_node;
        } else {
            new_node->prev = tail->prev;
            new_node->prev->next = new_node;
        }
        tail->prev = new_node;
        new_node->next = tail;
        _size++;
        return new_node->value;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::push_front(const T& x) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(x));
        if (empty()) {
            new_node->next = tail;
            tail->prev = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
        }
        head = new_node;
        new_node->prev = new_node;
        _size++;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::push_front(T&& x) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::move(x)));
        if (empty()) {
            new_node->next = tail;
            tail->prev = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
        }
        head = new_node;
        new_node->prev = new_node;
        _size++;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::pop_front() {
        if (empty()) {
            throw std::out_of_range("List::pop_front() : list is empty");
        }
        Node *del_node = head;
        head = head->next;
        head->prev = head;
        std::allocator_traits<node_allocator>::destroy(node_alloc, del_node);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, del_node, 1);
        _size--;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::push_back(const T& x) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(x));
        if (empty()) {
            head = new_node;
            new_node->prev = new_node;
        } else {
            new_node->prev = tail->prev;
            new_node->prev->next = new_node;
        }
        tail->prev = new_node;
        new_node->next = tail;
        _size++;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::push_back(T&& x) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::move(x)));
        if (empty()) {
            head = new_node;
            new_node->prev = new_node;
        } else {
            new_node->prev = tail->prev;
            new_node->prev->next = new_node;
        }
        tail->prev = new_node;
        new_node->next = tail;
        _size++;
    }

    template <class T, class Allocator>
    void List<T, Allocator>::pop_back() {
        if (empty()) {
            throw std::out_of_range("List::pop_back() : list is empty");
        }
        Node *del_node = tail->prev;
        tail->prev = del_node->prev;
        tail->prev->next = tail;
        std::allocator_traits<node_allocator>::destroy(node_alloc, del_node);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, del_node, 1);
        _size--;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename List<T, Allocator>::iterator List<T, Allocator>::emplace(typename List<T, Allocator>::const_iterator position, Args&&... args) {
        if (position == begin()) {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        }
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::forward<Args>(args)...));
        new_node->prev = position.ptr->prev;
        new_node->next = const_cast<Node*>(position.ptr);
        position.ptr->prev->next = new_node;
        position.ptr->prev = new_node;
        _size++;
        return iterator(new_node);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(typename List<T, Allocator>::const_iterator position, const T& x) {
        if (position == begin()) {
            push_front(x);
            return begin();
        }
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(x));
        new_node->prev = position.ptr->prev;
        new_node->next = const_cast<Node*>(position.ptr);
        position.ptr->prev->next = new_node;
        position.ptr->prev = new_node;
        _size++;
        return iterator(new_node);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(typename List<T, Allocator>::const_iterator position, T&& x) {
        if (position == begin()) {
            push_front(std::move(x));
            return begin();
        }
        Node *new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(std::move(x)));
        new_node->prev = position.ptr->prev;
        new_node->next = const_cast<Node*>(position.ptr);
        position.ptr->prev->next = new_node;
        position.ptr->prev = new_node;
        _size++;
        return iterator(new_node);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(typename List<T, Allocator>::const_iterator position, size_type n, const T& x) {
        for (size_type i = 0; i < n; ++i) {
            position = insert(position, x);
        }
        return iterator(position);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::erase(typename List<T, Allocator>::const_iterator position) {
        if (empty()) {
            throw std::out_of_range("List::erase() : list is empty");
        } else if (position == end()) {
            throw std::out_of_range("List::erase() : position is out of range");
        } else if (position == begin()) {
            pop_front();
            return begin();
        }
        Node *del_node = const_cast<Node*>(position.ptr);
        position.ptr->prev->next = position.ptr->next;
        position.ptr->next->prev = position.ptr->prev;
        auto next = position.ptr->next;
        std::allocator_traits<node_allocator>::destroy(node_alloc, del_node);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, del_node, 1);
        _size--;
        return iterator(next);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::erase(typename List<T, Allocator>::const_iterator first,
                                                                    typename List<T, Allocator>::const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(first);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::clear() noexcept {
        while (!empty()) {
            pop_front();
        }
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position, List& x) {
        if (get_allocator() != x.get_allocator()) {
            throw std::invalid_argument("List::splice() : allocator is not equal");
        } else if (!x.empty()) {
            if (position == begin()){
                x.get_tail()->prev->next = head;
                head->prev = x.get_tail()->prev;
                head = x.get_head();
                head->prev = head;
                x.set_head(nullptr);
                _size += x.size();
                x.set_size(0);
            } else {
                x.get_tail()->prev->next = const_cast<Node*>(position.ptr);
                position.ptr->prev->next = x.get_head();
                x.get_head()->prev = position.ptr->prev;
                position.ptr->prev = x.get_tail()->prev;
                x.set_head(nullptr);
                _size += x.size();
                x.set_size(0);
            }
        }
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position, List&& x) {
        splice(position, x);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position, List& x,
                                    typename List<T, Allocator>::const_iterator i) {
        if (get_allocator() != x.get_allocator()) {
            throw std::invalid_argument("List::splice() : allocator is not equal");
        } else if (x.empty() || i == x.end()) {
            throw std::out_of_range("List::splice() : i is out of range");
        }
        if (position == begin()) {
            if (i == x.begin()) {
                x.set_head(i.ptr->next);
                x.get_head()->prev = x.get_head();
            } else {
                i.ptr->prev->next = i.ptr->next;
                i.ptr->next->prev = i.ptr->prev;
            }
            i.ptr->next = head;
            head->prev = const_cast<Node*>(i.ptr);
            head = const_cast<Node*>(i.ptr);
            head->prev = head;
        } else {
            if (i == x.begin()) {
                x.set_head(i.ptr->next);
                x.get_head()->prev = x.get_head();
            } else {
                i.ptr->prev->next = i.ptr->next;
                i.ptr->next->prev = i.ptr->prev;
            }
            i.ptr->next = const_cast<Node*>(position.ptr);
            i.ptr->prev = position.ptr->prev;
            position.ptr->prev->next = const_cast<Node*>(i.ptr);
            position.ptr->prev = const_cast<Node*>(i.ptr);
        }
        _size++;
        x.set_size(x.size() - 1);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position, List&& x,
                                    typename List<T, Allocator>::const_iterator i) {
        splice(position, x, i);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position,
                                    List& x,
                                    typename List<T, Allocator>::const_iterator first,
                                    typename List<T, Allocator>::const_iterator last) {
        if (get_allocator() != x.get_allocator()) {
            throw std::invalid_argument("List::splice() : allocator is not equal");
        } else if (first == last) {
            throw std::out_of_range("List::splice() : first and last are equal");
        }
        size_type distance = std::distance(first, last);
        if (position == begin()) {
            last.ptr->prev->next = head;
            head->prev = last.ptr->prev;
            if (first == x.begin()) {
                x.set_head(const_cast<Node*>(last.ptr));
                x.get_head()->prev = x.get_head();
            } else {
                first.ptr->prev->next = const_cast<Node*>(last.ptr);
                last.ptr->prev = first.ptr->prev;
            }
            head = const_cast<Node*>(first.ptr);
            head->prev = head;
        } else {
            position.ptr->prev->next = const_cast<Node*>(first.ptr);
            last.ptr->prev->next = const_cast<Node*>(position.ptr);
            position.ptr->prev = last.ptr->prev;
            if (first == x.begin()) {
                x.set_head(const_cast<Node*>(last.ptr));
                x.get_head()->prev = x.get_head();
            } else {
                first.ptr->prev->next = const_cast<Node*>(last.ptr);
                last.ptr->prev = first.ptr->prev;
            }
            first.ptr->prev = position.ptr->prev;
        }
        _size += distance;
        x.set_size(x.size() - distance);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::splice(typename List<T, Allocator>::const_iterator position,
                                    List&& x,
                                    typename List<T, Allocator>::const_iterator first,
                                    typename List<T, Allocator>::const_iterator last) {
        splice(position, x, first, last);
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::remove(const T& value) {
        for (auto it = begin(); it != end();){
            if (*it == value) {
                it = erase(it);
            } else {
                ++it;
            }
        }
        return size();
    }

    template <class T, class Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::unique() {
        if (empty()) {
            return 0;
        }
        auto it = begin();
        auto next = std::next(it);
        while (next != end()) {
            if (*it == *next) {
                next = erase(next);
            } else {
                it = next;
                ++next;
            }
        }
        return size();
    }

    template <class T, class Allocator>
    void List<T, Allocator>::merge(List& x) {
        auto it = begin();
        auto xit = x.begin();
        while (it != end() && xit != x.end()) {
            if (*it < *xit) {
                ++it;
            } else {
                splice(it, x, xit++);
            }
        }
        if (xit != x.end()) {
            splice(end(), x, xit, x.end());
        }
    }

    template <class T, class Allocator>
    void List<T, Allocator>::merge(List&& x) {
        merge(x);
    }

    template <class T, class Allocator>
    template<class Compare>
    void List<T, Allocator>::merge(List& x, Compare comp) {
        auto it = begin();
        auto xit = x.begin();
        while (it != end() && xit != x.end()) {
            if (comp(*it, *xit)) {
                ++it;
            } else {
                splice(it, x, xit++);
            }
        }
        if (xit != x.end()) {
            splice(end(), x, xit, x.end());
        }
    }

    template <class T, class Allocator>
    template<class Compare>
    void List<T, Allocator>::merge(List&& x, Compare comp) {
        merge(x, comp);
    }

    template<class T, class Allocator>
    template<class Compare>
    void List<T, Allocator>::merge_sort(List &x, Compare comp) {
        if (x.size() <= 1) {
            return;
        }
        auto slow = x.begin();
        auto fast = x.begin();
        while (fast != x.end() && std::next(fast) != x.end()) {
            std::advance(slow, 1);
            std::advance(fast, 2);
        }
        List<T> temp;
        temp.splice(temp.begin(), x, slow, x.end());
        merge_sort(x, comp);
        merge_sort(temp, comp);
        x.merge(temp, comp);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::sort() {
        merge_sort(*this, std::less<T>());
    }

    template <class T, class Allocator>
    template<class Compare>
    void List<T, Allocator>::sort(Compare comp) {
        merge_sort(*this, comp);
    }

    template <class T, class Allocator>
    void List<T, Allocator>::reverse() noexcept {
        if (empty()) {
            return;
        }
        auto next = std::next(begin());
        while (next != end()) {
            auto temp = next;
            next = std::next(next);
            splice(begin(), *this, temp);
        }
    }
}
