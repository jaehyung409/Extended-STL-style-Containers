/*
 * @ Created by jaehyung409 on 25. 1. 23.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License. 
 */

module;
#include <memory>
#include <iterator>
#include <initializer_list>
#include <ranges>

export module j.forward_list;

import j.range_basics;

namespace j {
    export template <class T, class Allocator = std::allocator<T>>
    class forward_list {
    public:
        using value_type        = T;
        using allocator_type    = std::allocator<T>;
        using pointer           = value_type*;
        using const_pointer     = const value_type*;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using size_type         = std::size_t;
        using difference_type   = std::ptrdiff_t;
        class iterator;
        class const_iterator;

    private:
        struct _forward_list_node;
        using Node = _forward_list_node;

        using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node* _before_head;
        node_allocator _node_alloc;
        // size_type size; didn't use for faster performance

        // helper function (sort)
        template <class Compare>
        void merge_sort(forward_list& x, Compare comp);

    public:
        // constructor and destructor
        forward_list() : forward_list(Allocator()) {}
        explicit forward_list(const Allocator& alloc);
        explicit forward_list(size_type n, const Allocator& alloc = Allocator()); // n size forward_list with default value of T.
        forward_list(size_type n, const T& value, const Allocator& alloc = Allocator()); // n size forward_list with value.
        template<class InputIter>
        requires std::input_iterator<InputIter>
        forward_list(InputIter first, InputIter last, const Allocator& alloc = Allocator());
        template <container_compatible_range<T> R>
        forward_list(std::ranges::from_range_t, R&& range, const Allocator& = Allocator()) = delete;

        forward_list(const forward_list& x);
        forward_list(forward_list&& x) noexcept;
        forward_list(const forward_list& x, const std::type_identity_t<Allocator>&);
        forward_list(forward_list&& x, const std::type_identity_t<Allocator>&);
        forward_list(std::initializer_list<T>, const Allocator& = Allocator());

        ~forward_list();

        // assignment
        forward_list& operator=(const forward_list& x);
        forward_list& operator=(forward_list&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
        forward_list& operator=(std::initializer_list<T> il);
        template <class InputIt>
        requires std::input_iterator<InputIt>
        void assign(InputIt first, InputIt last);
        template <container_compatible_range<T> R>
        void assign_range(R&& rg) = delete; // not implemented yet
        void assign(size_type n, const T& t);
        void assign(std::initializer_list<T> il);
        allocator_type get_allocator() const noexcept;

        // iteratos
        iterator before_begin() noexcept;
        const_iterator before_begin() const noexcept;
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cbefore_begin() const noexcept;
        const_iterator cend() const noexcept;


        // capacity
        bool empty() const noexcept;
        size_type max_size() const noexcept;

        // element access
        reference front();
        const_reference front() const;

        // modifiers -> need to modify method impl order
        template <class... Args>
        reference emplace_front(Args&&... args);
        void push_front(const T& value);
        void push_front(T&& value);
        template <container_compatible_range<T> R>
        void prepend_range(R&& rg) = delete;
        void pop_front();

        template <class... Args>
        iterator emplace_after(const_iterator position, Args&&... args);
        iterator insert_after(const_iterator position, const T& value);
        iterator insert_after(const_iterator position, T&& value);

        iterator insert_after(const_iterator position, size_type count, const T& value);
        template <class InputIter>
        requires std::input_iterator<InputIter>
        iterator insert_after(const_iterator position, InputIter first, InputIter last);
        iterator insert_after(const_iterator position, std::initializer_list<T> il);
        template <container_compatible_range<T> R>
        iterator insert_range_after(const_iterator position, R&& rg) = delete; // not implemented yet

        iterator erase_after(const_iterator position);
        iterator erase_after(const_iterator position, const_iterator last);
        void swap(forward_list& x) noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value));

        void resize(size_type nsz);
        void resize(size_type nsz, const T& value);
        void clear() noexcept;

        // forward_list operations
        void splice_after(const_iterator position, forward_list& x);
        void splice_after(const_iterator position, forward_list&& x);
        void splice_after(const_iterator position, forward_list& x, const_iterator i);
        void splice_after(const_iterator position, forward_list&& x, const_iterator i);
        void splice_after(const_iterator position, forward_list& x, const_iterator first, const_iterator last);
        void splice_after(const_iterator position, forward_list&& x, const_iterator first, const_iterator last);

        size_type remove(const T& value);
        template <class Predicate>
        requires std::predicate<Predicate, T>
        size_type remove_if(Predicate pred);

        size_type unique();
        template <class BinaryPredicate>
        requires std::predicate<BinaryPredicate, T, T>
        size_type unique(BinaryPredicate binary_pred = std::equal_to<T>());

        void merge(forward_list& x);
        void merge(forward_list&& x);
        template <class Compare>
        void merge(forward_list& x, Compare comp);
        template <class Compare>
        void merge(forward_list&& x, Compare comp);

        void sort();
        template <class Compare>
        requires std::strict_weak_order<Compare, T, T>
        void sort(Compare comp);

        void reverse() noexcept;
    };

    template<class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
    forward_list( InputIter first, InputIter last, const Allocator& = Allocator())
        -> forward_list<typename std::iterator_traits<InputIter>::value_type, Allocator>;

    template <std::ranges::input_range R, class Allocator = std::allocator<std::ranges::range_value_t<R>>>
    forward_list(std::ranges::from_range_t, R&& range, const Allocator& = Allocator())
        -> forward_list<std::ranges::range_value_t<R>, Allocator>;

    export template<class T, class Allocator>
    bool operator== (const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    export template<class T, class Allocator>
    auto operator<=>(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs) {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                      rhs.begin(), rhs.end(),
                                                      std::compare_three_way{});
    }

    export template<class T, class Allocator>
    void swap(forward_list<T, Allocator>& x, forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    export template<class T, class Allocator, class U>
    typename forward_list<T, Allocator>::size_type erase(forward_list<T, Allocator>& c, const U& value) {
        return c.remove(value);
    }

    export template<class T, class Allocator, class Predicate>
    typename forward_list<T, Allocator>::size_type erase_if(forward_list<T, Allocator>& c, Predicate pred) {
        return c.remove_if(pred);
    }

    template <class T, class Allocator>
    struct forward_list<T, Allocator>::_forward_list_node {
        friend forward_list;
        friend iterator;
        friend const_iterator;

    private:
        value_type _value;
        _forward_list_node* _next;
    };

    template <class T, class Allocator>
    class forward_list<T, Allocator>::iterator {
        friend forward_list;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        using node_pointer = Node*;
        node_pointer _ptr;

    public:
        explicit iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}
        iterator operator=(const const_iterator& other) {
            _ptr = other._ptr;
            return *this;
        }

        reference operator*() const { return _ptr->_value; }
        pointer operator->() const { return &(_ptr->_value); }

        iterator& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const { return _ptr == other._ptr; }
        operator const_iterator() const { return const_iterator(_ptr); }
    };

    template <class T, class Allocator>
    class forward_list<T, Allocator>::const_iterator {
        friend forward_list;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using const_pointer = const T*;
        using const_reference = const T&;

    private:
        using node_pointer = Node*;
        node_pointer _ptr;

    public:
        explicit const_iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}
        explicit const_iterator(const iterator& other) : _ptr(other._ptr) {}
        const_iterator operator=(const iterator& other) {
            _ptr = other._ptr;
            return *this;
        }

        const_reference operator*() const { return _ptr->_value; }
        const_pointer operator->() const { return &(_ptr->_value); }

        const_iterator& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const const_iterator& other) const { return _ptr == other._ptr; }
    };
}

namespace j {
    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(const Allocator& alloc)
        : _node_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        _before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(&_before_head->_value);
        _before_head->_next = nullptr; // Initialize the next pointer to nullptr
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(const size_type n, const Allocator& alloc)
        : forward_list(n, T(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(const size_type n, const T& value, const Allocator& alloc)
        : forward_list(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        for (size_type i = 0; i < n; ++i) {
            emplace_front(value);
        }
    }

    template <class T, class Allocator>
    template <class InputIter> requires std::input_iterator<InputIter>
    forward_list<T, Allocator>::forward_list(InputIter first, InputIter last, const Allocator& alloc)
        : forward_list(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        auto it = before_begin();
        for (; first != last; ++first) {
            it = emplace_after(it, *first);
        }
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(const forward_list& x)
        : forward_list(
            x.begin(),
            x.end(),
            std::allocator_traits<Allocator>::select_on_container_copy_construction(x._node_alloc)
          ) {}

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(forward_list&& x)  noexcept
        : _before_head(x._before_head), _node_alloc(std::move(x._node_alloc)) {
        x._before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(&x._before_head->_value);
        x._before_head->_next = nullptr; // Initialize the next pointer to nullptr
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(const forward_list& x, const std::type_identity_t<Allocator>& alloc)
        : forward_list(
            x.begin(),
            x.end(),
            std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)
          ) {}

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(forward_list&& x, const std::type_identity_t<Allocator>& alloc)
        : _before_head(x._before_head),
          _node_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        x._before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(&x._before_head->_value);
        x._before_head->_next = nullptr; // Initialize the next pointer to nullptr
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>::forward_list(std::initializer_list<T> il, const Allocator& alloc)
        : forward_list(
            il.begin(),
            il.end(),
            std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)
          ) {}

    template <class T, class Allocator>
    forward_list<T, Allocator>::~forward_list() {
        clear();
        std::destroy_at(&_before_head->_value);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>& forward_list<T, Allocator>::operator=(const forward_list& x) {
        if (this != &x) {
            clear();
            auto xit = x.begin();
            for (auto it = before_begin(); xit != x.end(); ++xit) {
                it = emplace_after(it, *xit);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    forward_list<T, Allocator>& forward_list<T, Allocator>::operator=(forward_list&& x) noexcept {
        if (this != &x) {
            clear();
            std::destroy_at(_before_head);
            std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);

            _head = x.get_head();
            _before_head = x.get_before_head();
            _node_alloc = std::move(x.get_allocator());

            auto new_before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
            std::construct_at(new_before_head);

            x.set_head(nullptr);
            x.set_before_head(new_before_head);
            x._before_head->_next = x._head;
        }
        return *this;
    }

    template<class T, class Allocator>
    void forward_list<T, Allocator>::assign(forward_list::size_type count, const T &value) {
        clear();
        for (int i = 0; i < count; i++) {
            emplace_front(value);
        }
    }

    template<class T, class Allocator>
    std::allocator<T> forward_list<T, Allocator>::get_allocator() const noexcept {
        return std::allocator<T>(_node_alloc);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::before_begin() noexcept {
        return iterator(_before_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::before_begin() const noexcept {
        return const_iterator(_before_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::begin() noexcept {
        return iterator(_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::begin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::end() noexcept {
        return iterator(nullptr);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::end() const noexcept {
        return const_iterator(nullptr);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::cbefore_begin() const noexcept {
        return iterator(_before_head);
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::cend() const noexcept {
        return iterator();
    }

    template <class T, class Allocator>
    bool forward_list<T, Allocator>::empty() const noexcept {
        return _head == nullptr;
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(Node) - 1; // -1 for _before_head
    }

    template <class T, class Allocator>
    T& forward_list<T, Allocator>::front() {
        return _head->_value;
    }

    template <class T, class Allocator>
    const T& forward_list<T, Allocator>::front() const {
        return _head->_value;
    }

    template<class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::insert_after(forward_list::const_iterator position, const T &value) {
        if (position == before_begin()) {
            emplace_front(value);
            return begin();
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, value);
        new_node->_next = position->_next;
        position->_next = new_node;

        return iterator(new_node);
    }


    template<class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::insert_after(forward_list::const_iterator position, T &value) {
        if (position == before_begin()) {
            emplace_front(value);
            return begin();
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, value);

        new_node->_next = position->_next;
        position->_next = new_node;
        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename forward_list<T, Allocator>::iterator
    forward_list<T, Allocator>::insert_after(forward_list::const_iterator position, forward_list::size_type count,
                                             const T &value) {
        for (int i = 0; i < count; i++) {
            position = insert_after(position, value);
        }
        return forward_list::iterator(position._ptr);
    }

    template<class T, class Allocator>
    template<class... Args>
    typename forward_list<T, Allocator>::iterator
    forward_list<T, Allocator>::emplace_after(forward_list::const_iterator position, Args &&... args) {
        if (position == before_begin()) {
            emplace_front(args...);
            return begin();
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);

        new_node->_next = position->_next;
        position->_next = new_node;

        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::erase_after(forward_list::const_iterator position) {
        if (position == before_begin()) {
            pop_front();
            return begin();
        }

        Node* node_to_delete = position->_next;
        position->_next = node_to_delete->_next;
        std::destroy_at(node_to_delete);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, node_to_delete, 1);
        return position->_next;
    }

    template <class T, class Allocator>
    template <class ... Args>
    T& forward_list<T, Allocator>::emplace_front(Args&&... args) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        if (empty()) {
            new_node->_next = nullptr;
        } else {
            new_node->_next = _head;
        }
        _head = new_node;
        _before_head->_next = _head;
        return _head->_value;
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::push_front(const T& value) {
        emplace_front(value);
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::push_front(T&& value) {
        emplace_front(std::move(value));
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::pop_front() {
        Node* del_node = _head;
        _head = _head->_next;
        _before_head->_next = _head;
        std::destroy_at(del_node);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::resize(size_type nsz) {
        resize(nsz, T());
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::resize(size_type nsz, const T& value) {
        size_type size = 0;
        auto prev = before_begin();
        auto it = begin();
        for(; it != end() && size < nsz; ++prev, ++it, ++size) {}
        if (size == nsz){
            if (it == end()) {
                return;
            }
            for(; std::next(prev) != end(); erase_after(prev)) {}
        }
        else {
            for(; size < nsz; size++){
                emplace_after(prev, value);
            }
        }
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::clear() noexcept {
        for (auto it = begin(); it != end(); ) {
            auto temp = it;
            ++it;
            std::destroy_at(temp._ptr);
            std::allocator_traits<node_allocator>::deallocate(_node_alloc, temp._ptr, 1);
        }
        _head = nullptr;
        _before_head->_next = _head;
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::remove(const T& value) {
        if (empty()) {
            return 0;
        }
        size_type size = 0;
        auto it = before_begin();
        auto next = begin();
        while (next != end()) {
            if (*next == value) {
                erase_after(it);
                next = std::next(it);
            } else {
                ++it;
                ++next;
                ++size;
            }
        }
        return size;
    }

    template <class T, class Allocator>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::unique() {
        if (empty()) {
            return 0;
        }
        size_type size = 1;
        auto it = begin();
        auto next = std::next(begin());
        while (next != end()) {
            if (*next == *it) {
                erase_after(it);
                next = std::next(it);
            } else {
                ++it;
                ++next;
                ++size;
            }
        }
        return std::distance(begin(), end());
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::merge(forward_list& x) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || std::less<>{}(*next_xit, *next_it)) {
                it = emplace_after(it, *next_xit);
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::merge(forward_list&& x) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || std::less<>{}(*next_xit, *next_it)) {
                it = emplace_after(it, std::move(*next_xit));
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    template <class Compare>
    void forward_list<T, Allocator>::merge(forward_list& x, Compare comp) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || comp(*next_xit, *next_it)) {
                it = emplace_after(it, *next_xit);
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    template <class Compare>
    void forward_list<T, Allocator>::merge(forward_list&& x, Compare comp) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || comp(*next_xit, *next_it)) {
                it = emplace_after(it, std::move(*next_xit));
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template<class T, class Allocator>
    void forward_list<T, Allocator>::splice_after(forward_list::const_iterator position, forward_list &other) {
        if (!other.empty()) {
            auto last = other.before_begin();
            while (std::next(last) != other.end()) {
                ++last;
            }
            last->_next = position->_next;
            if (position == before_begin()) {
                _head = other.get_head();
                _before_head->_next = _head;
            } else {
                position->_next = other.get_head();
            }
            other.set_head(nullptr);
        }
    }

    template<class T, class Allocator>
    void forward_list<T, Allocator>::splice_after(forward_list::const_iterator position, forward_list &&other) {
        splice_after(position, other);
    }

    template<class T, class Allocator>
    void forward_list<T, Allocator>::splice_after(forward_list::const_iterator position, forward_list &other,
                                                  forward_list::const_iterator first,
                                                  forward_list::const_iterator last) {
        if (first == last) {
            return;
        }
        auto before_last = first;
        while (std::next(before_last) != last) {
            ++before_last;
        }
        auto move_first = first->_next;
        if (first == other.before_begin()) {
            other.set_head(before_last->_next);
            other.get_before_head()->_next = other.get_head();
        } else {
            first->_next = before_last->_next;
        }
        before_last->_next = position->_next;
        if (position == before_begin()) {
            _head = move_first;
            _before_head->_next = _head;
        } else {
            position->_next = move_first;
        }
    }

    template<class T, class Allocator>
    void forward_list<T, Allocator>::splice_after(forward_list::const_iterator position, forward_list &&other,
                                                  forward_list::const_iterator first,
                                                  forward_list::const_iterator last) {
        splice_after(position, other, first, last);
    }


    template<class T, class Allocator>
    template<class Compare>
    void forward_list<T, Allocator>::merge_sort(forward_list &x, Compare comp) {
        if (x.empty() || std::next(x.begin()) == x.end()) {
            return;
        }
        auto slow = x.begin();
        auto fast = x.begin();
        while (std::next(fast) != x.end() && std::next(fast, 2) != x.end()){
            std::advance(slow, 1);
            std::advance(fast, 2);
        }
        forward_list<T> second;
        second.splice_after(second.before_begin(), x, slow, x.end());
        merge_sort(x, comp);
        merge_sort(second, comp);
        x.merge(second, comp);
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::sort() {
        merge_sort(*this, std::less<T>());
    }

    template <class T, class Allocator>
    template <class Compare>
    void forward_list<T, Allocator>::sort(Compare comp) {
        merge_sort(*this, comp);
    }

    template <class T, class Allocator>
    void forward_list<T, Allocator>::reverse() noexcept {
        if (empty()) {
            return;
        }
        Node* prev = nullptr;
        Node* current = _head;
        Node* next = nullptr;
        while (current != nullptr) {
            next = current->_next;
            current->_next = prev;
            prev = current;
            current = next;
        }
        _head = prev;
        _before_head->_next = _head;
    }

    template <class T, class Allocator, class U>
    typename forward_list<T, Allocator>::size_type erase(forward_list<T, Allocator>& c, const U& value) {
        auto it = c.before_begin();
        while (std::next(it) != c.end()) {
            if (*std::next(it) == value) {
                it = erase_after(it);
            } else {
                ++it;
            }
        }
        return std::distance(c.begin(), c.end());
    }
/*
    template <class T, class Allocator, class Predicate>
    typename forward_list<T, Allocator>::size_type erase_if(forward_list<T, Allocator>& c, Predicate pred) {
    } */
}
