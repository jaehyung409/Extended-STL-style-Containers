/*
 * @ Created by jaehyung409 on 25. 1. 27.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <iostream> // for debugging

export module deque;

namespace j {
    export template<class T, class Allocator = std::allocator<T>>
    class Deque {
    public:
        class _Deque_iterator;
        class _const_Deque_iterator;

        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = _Deque_iterator;
        using const_iterator = _const_Deque_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        using map = value_type**;
        using box = value_type*;
        using map_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<box>;
        using box_allocator = Allocator;
        const size_type default_map_size = 4;
        const size_type _box_size = 512;
        map _map;                   // pointer to the map
        size_type _size;            // size of the deque
        size_type _map_capacity;    // capacity of the map
        size_type _begin_map_index;  // index of the first map
        size_type _begin_box_index;  // index of the first box (in the first map)
        size_type _end_map_index;    // index of the next of the last map (like end())
        size_type _end_box_index;    // index of the next of the last box (like end())
        map_allocator _map_alloc;
        box_allocator _box_alloc;

        // helper functions (change private variables)
        [[nodiscard]] map _create_map(size_type n) {
            map new_map = std::allocator_traits<map_allocator>::allocate(_map_alloc, n); // T* is trivial type
            std::fill(new_map, new_map + n, nullptr);                  // initialize all pointers to nullptr (empty)
            return new_map;
        }
        void _destroy_map() {  // need to deallocate all boxes previously
            std::allocator_traits<map_allocator>::deallocate(_map_alloc, _map, _map_capacity);
            _map = nullptr;
            _map_capacity = 0;
        }
        void _create_box(size_type box_index) {
            if (_map[box_index] != nullptr) {
                return;
            }
            _map[box_index] = std::allocator_traits<box_allocator>::allocate(_box_alloc, _box_size);
        }
        void _destroy_box(size_type box_index) {
            if (_map[box_index] == nullptr) {
                return;
            }
            std::allocator_traits<box_allocator>::deallocate(_box_alloc, _map[box_index], _box_size);
            _map[box_index] = nullptr;
        }
        void _reallocate_map(size_type n) {
            if (n == _map_capacity) {
                return;
            }
            map new_map = _create_map(n);
            if (_map_capacity < n) {
                for (size_type i = 0; i < _map_capacity; ++i) {
                    new_map[i] = _map[i];
                    _map[i] = nullptr;
                }
            } else {
                size_type used = _map_size();
                for (size_type i = 0; i < used; ++i) {
                    new_map[i] = _map[(_begin_map_index + i) % _map_capacity];
                    _map[(_begin_map_index + i) % _map_capacity] = nullptr;
                }
                _begin_map_index = 0;
                _end_map_index = used - 1;
            }
            _destroy_map();
            _map = new_map;
            _map_capacity = n;
        }
        size_type _map_size() const noexcept {
            if (empty()) {
                return 0;
            }
            return (_end_map_index - _begin_map_index + _map_capacity) % _map_capacity + 1;
            // This means that memory is allocated in advance even if the space pointed to by end_index is an empty box.
        }
        // getter and setter
        map _get_map() const noexcept { return _map; }
        size_type _get_map_capacity() const noexcept { return _map_capacity; }
        void _set_map_capacity(size_type n) noexcept { _map_capacity = n; }
        size_type _get_begin_map_index() const noexcept { return _begin_map_index; }
        void _set_begin_map_index(size_type n) noexcept {
            _begin_map_index = (n + _map_capacity) % _map_capacity; // circular
        }
        size_type _get_begin_box_index() const noexcept { return _begin_box_index; }
        void _set_begin_box_index(size_type n) noexcept { _begin_box_index = n % _box_size; }
        size_type _get_end_map_index() const noexcept { return _end_map_index; }
        void _set_end_map_index(size_type n) noexcept {
            _end_map_index = (n + _map_capacity) % _map_capacity; // circular
        }
        size_type _get_end_box_index() const noexcept { return _end_box_index; }
        void _set_end_box_index(size_type n) noexcept { _end_box_index = n % _box_size; }
        void _set_size(size_type n) noexcept { _size = n; }

        // helper function for common logic
        void _for_front();
        void _for_back();
    public:
        Deque() : Deque(Allocator()) {}
        explicit Deque(const Allocator &alloc);
        explicit Deque(size_type n, const Allocator &alloc = Allocator());
        Deque(size_type n, const T &value, const Allocator &alloc = Allocator());
        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        Deque(InputIter first, InputIter last, const Allocator &alloc = Allocator());
        //template<ranges::input_range R>
        //Deque(from_range_t, R&& range, const Allocator& alloc = Allocator());
        Deque(const Deque &other);
        Deque(Deque &&other);
        Deque(const Deque &other, const std::type_identity_t<Allocator> &alloc);
        Deque(Deque &&other, const std::type_identity_t<Allocator> &alloc);
        Deque(std::initializer_list<T> il, const Allocator &alloc = Allocator());
        ~Deque();

        Deque &operator=(const Deque &other);
        Deque &operator=(Deque &&other) noexcept(
        std::allocator_traits<Allocator>::is_always_equal::value);
        Deque &operator=(std::initializer_list<T> il);
        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void assign(InputIter first, InputIter last);
        //template<ranges::input_range R>
        //void assign(R&& range);
        void assign(size_type n, const T &value);
        void assign(std::initializer_list<T> il);
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
        void resize(size_type sz, const T &value);
        void shrink_to_fit();

        // element access
        reference operator[](size_type n);
        const_reference operator[](size_type n) const;
        reference at(size_type n);
        const_reference at(size_type n) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        // modifiers
        template<class... Args>
        reference emplace_front(Args &&... args);
        template<class...Args>
        reference emplace_back(Args &&... args);
        template<class... Args>
        iterator emplace(const_iterator position, Args &&... args);

        void push_front(const T &value);
        void push_front(T &&value);
        // template<ranges::input_range R>
        // void prepend_range(R&& range);
        void push_back(const T &value);
        void push_back(T &&value);
        // template<ranges::input_range R>
        // void append_range(R&& range);

        iterator insert(const_iterator position, const T &value);
        iterator insert(const_iterator position, T &&value);
        iterator insert(const_iterator position, size_type count, const T &value);
        template<class InputIter>
        requires (!std::is_integral_v<InputIter>)
        iterator insert(const_iterator position, InputIter first, InputIter last);
        // template<ranges::input_range R>
        // iterator insert(const_iterator position, R&& range);
        iterator insert(const_iterator position, std::initializer_list<T> il);

        void pop_front();
        void pop_back();

        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);

        void swap(Deque &other) noexcept(
        std::allocator_traits<Allocator>::is_always_equal::value);
        void clear() noexcept;
    };

    template<class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
    Deque(InputIter, InputIter, Allocator = Allocator())
    -> Deque<typename std::iterator_traits<InputIter>::value_type, Allocator>;

    // template <ranges::input_range R, class Allocator = std::allocator<std::ranges::range_value_t<R>>>
    // Deque(from_range_t, R&&, Allocator = Allocator())
    //     -> Deque<std::ranges::range_value_t<R>, Allocator>;

    template<class T, class Allocator>
    class Deque<T, Allocator>::_Deque_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        pointer _ptr;

    public:
        _Deque_iterator(pointer ptr = nullptr) : _ptr(ptr) {}
        _Deque_iterator(const _const_Deque_iterator &other)
            : _ptr(other._ptr) {}

        reference operator*() { return *_ptr; }
        pointer operator->() { return &(*_ptr); }

        _Deque_iterator &operator++() {
            ++_ptr;
            return *this;
        }

        _Deque_iterator operator++(int) {
            _Deque_iterator temp = *this;
            ++(*this);
            return temp;
        }

        _Deque_iterator &operator--() {
            --_ptr;
            return *this;
        }

        _Deque_iterator operator--(int) {
            _Deque_iterator temp = *this;
            --(*this);
            return temp;
        }

        _Deque_iterator &operator+=(difference_type n) {
            _ptr += n;
            return *this;
        }

        _Deque_iterator operator+(difference_type n) const {
            _Deque_iterator temp = *this;
            return temp += n;
        }

        _Deque_iterator &operator-=(difference_type n) {
            _ptr -= n;
            return *this;
        }

        _Deque_iterator operator-(difference_type n) const {
            _Deque_iterator temp = *this;
            return temp -= n;
        }

        difference_type operator-(const _Deque_iterator &other) const {
            return _ptr - other._ptr;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator==(const _Deque_iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const _Deque_iterator &other) const { return _ptr != other._ptr; }
        bool operator<(const _Deque_iterator &other) const { return _ptr < other._ptr; }
        bool operator>(const _Deque_iterator &other) const { return _ptr > other._ptr; }
        bool operator<=(const _Deque_iterator &other) const { return _ptr <= other._ptr; }
        bool operator>=(const _Deque_iterator &other) const { return _ptr >= other._ptr; }
        operator _const_Deque_iterator() const { return _const_Deque_iterator(_ptr); }
    };

    template<class T, class Allocator>
    class Deque<T, Allocator>::_const_Deque_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using const_pointer = const T*;
        using const_reference = const T&;

    private:
        pointer _ptr;

    public:
        _const_Deque_iterator(pointer ptr = nullptr) : _ptr(ptr) {}

        const_reference operator*() const { return *_ptr; }
        const_pointer operator->() const { return &(*_ptr); }

        _const_Deque_iterator &operator++() {
            ++_ptr;
            return *this;
        }

        _const_Deque_iterator operator++(int) {
            _const_Deque_iterator temp = *this;
            ++(*this);
            return temp;
        }

        _const_Deque_iterator &operator--() {
            --_ptr;
            return *this;
        }

        _const_Deque_iterator operator--(int) {
            _const_Deque_iterator temp = *this;
            --(*this);
            return temp;
        }

        _const_Deque_iterator &operator+=(difference_type n) {
            _ptr += n;
            return *this;
        }

        _const_Deque_iterator operator+(difference_type n) const {
            _const_Deque_iterator temp = *this;
            return temp += n;
        }

        _const_Deque_iterator &operator-=(difference_type n) {
            _ptr -= n;
            return *this;
        }

        _const_Deque_iterator operator-(difference_type n) const {
            _const_Deque_iterator temp = *this;
            return temp -= n;
        }

        difference_type operator-(const _const_Deque_iterator &other) const {
            return _ptr - other._ptr;
        }

        const_reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator==(const _const_Deque_iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const _const_Deque_iterator &other) const { return _ptr != other._ptr; }
        bool operator<(const _const_Deque_iterator &other) const { return _ptr < other._ptr; }
        bool operator>(const _const_Deque_iterator &other) const { return _ptr > other._ptr; }
        bool operator<=(const _const_Deque_iterator &other) const { return _ptr <= other._ptr; }
        bool operator>=(const _const_Deque_iterator &other) const { return _ptr >= other._ptr; }
    };
}

namespace j {
    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(const Allocator &alloc) : _size(0), _map_alloc(alloc), _box_alloc(alloc) {
        _map = _create_map(default_map_size);
        _map_capacity = default_map_size;
        _begin_map_index = 0;
        _begin_box_index = 0;
        _end_map_index = 0;
        _end_box_index = 0;
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(Deque::size_type n, const Allocator &alloc) : Deque(n, T(), alloc) {}

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(Deque::size_type n, const T &value, const Allocator &alloc) : Deque(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_back(value);
        }
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    Deque<T, Allocator>::Deque(InputIter first, InputIter last, const Allocator &alloc) : Deque(alloc) {
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(const Deque &other)
        : Deque(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())) {
        for (const T &t : other) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(Deque &&other) : Deque(std::move(other), other.get_allocator()) {}

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(const Deque &other, const std::type_identity_t <Allocator> &alloc) : Deque(alloc) {
        for (const T &t : other) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(Deque &&other, const std::type_identity_t <Allocator> &alloc)
        : Deque(alloc) {
        _destroy_map();
        _map = other._get_map();
        _map_capacity = other._get_map_capacity();
        _size = other.size();
        _begin_map_index = other._get_begin_map_index();
        _begin_box_index = other._get_begin_box_index();
        _end_map_index = other._get_end_map_index();
        _end_box_index = other._get_end_box_index();

        other._map = _create_map(default_map_size);
        other._set_map_capacity(default_map_size);
        other._set_size(0);
        other._set_begin_map_index(0);
        other._set_begin_box_index(0);
        other._set_end_map_index(0);
        other._set_end_box_index(0);
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::Deque(std::initializer_list<T> il, const Allocator &alloc) : Deque(alloc) {
        for (const T &t : il) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    Deque<T, Allocator>::~Deque() {
        clear();
        _destroy_map();
    }

    template<class T, class Allocator>
    Deque<T, Allocator>& Deque<T, Allocator>::operator=(const Deque &other) {
        if (this != &other) {
            clear();
            _reallocate_map(other._get_map_capacity());
            for (const T& t : other) {
                emplace_back(t);
            }
        }
        return *this;
    }

    template<class T, class Allocator>
    Deque<T, Allocator>& Deque<T, Allocator>::operator=(Deque &&other)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &other) {
            clear();
            _destroy_map();
            _map = other._get_map();
            _map_capacity = other._get_map_capacity();
            _size = other.size();
            _begin_map_index = other._get_begin_map_index();
            _begin_box_index = other._get_begin_box_index();
            _end_map_index = other._get_end_map_index();
            _end_box_index = other._get_end_box_index();

            other._map = _create_map(default_map_size);
            other._set_map_capacity(default_map_size);
            other._set_size(0);
            other._set_begin_map_index(0);
            other._set_begin_box_index(0);
            other._set_end_map_index(0);
            other._set_end_box_index(0);
        }
        return *this;
    }

    template<class T, class Allocator>
    Deque<T, Allocator>& Deque<T, Allocator>::operator=(std::initializer_list<T> il) {
        clear();
        for (const T &t : il) {
            emplace_back(t);
        }
        return *this;
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    void Deque<T, Allocator>::assign(InputIter first, InputIter last) {
        clear();
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::assign(Deque::size_type n, const T &value) {
        clear();
        for (size_type i = 0; i < n; ++i) {
            emplace_back(value);
        }
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::assign(std::initializer_list<T> il) {
        clear();
        for (const T &t : il) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::allocator_type Deque<T, Allocator>::get_allocator() const noexcept {
        return _box_alloc;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::begin() noexcept {
        return iterator(_map[_begin_map_index] + _begin_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::begin() const noexcept {
        return const_iterator(_map[_begin_map_index] + _begin_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::end() noexcept {
        return iterator(_map[_end_map_index] + _end_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::end() const noexcept {
        return const_iterator(_map[_end_map_index] + _end_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reverse_iterator Deque<T, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reverse_iterator Deque<T, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_map[_begin_map_index] + _begin_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cend() const noexcept {
        return const_iterator(_map[_end_map_index] + _end_box_index);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reverse_iterator Deque<T, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reverse_iterator Deque<T, Allocator>::crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    template<class T, class Allocator>
    bool Deque<T, Allocator>::empty() const noexcept {
        return size() == 0;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::size_type Deque<T, Allocator>::size() const noexcept {
        return _size;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::size_type Deque<T, Allocator>::max_size() const noexcept {
        return std::allocator_traits<Allocator>::max_size(_box_alloc) * _box_size;
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::resize(Deque::size_type sz) {
        resize(sz, T());
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::resize(Deque::size_type sz, const T &value) {
        if (sz < size()) {
            const size_type target_size = size() - sz;
            for (size_type i = 0; i < target_size; ++i) {
                pop_back();
            }
        } else if (sz > size()) {
            const size_type target_size = sz - size();
            for (size_type i = 0; i < target_size; ++i) {
                emplace_back(value);
            }
        }
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::shrink_to_fit() {
        const size_type old_map_capacity = _get_map_capacity();
        if (_get_begin_map_index() <= _get_end_map_index()) {
            for (size_type i = 0; i < old_map_capacity; ++i) {
                if (i < _get_begin_map_index() || i > _get_end_map_index()) {
                    _destroy_box(i);
                }
            }
        }
        else {
            for (size_type i = 0; i < old_map_capacity; ++i) {
                if (i < _get_begin_map_index() && i > _get_end_map_index()) {
                    _destroy_box(i);
                }
            }
        }
        size_t fit_size = default_map_size;
        const size_t now_map_size = 2 * _map_size();
        while (fit_size < now_map_size) {
            fit_size *= 2;
        }
        _reallocate_map(fit_size);
    }   // free unused memory, and reallocate map (2 * size <= 4, 8, 16, ...)

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::operator[](Deque::size_type n) {
        return _map[(_begin_map_index + (_begin_box_index + n) / _box_size)  % _map_capacity]
                   [(_begin_box_index + n) % _box_size];
    } // circular

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reference Deque<T, Allocator>::operator[](Deque::size_type n) const {
        return _get_map()[(_begin_map_index + (_begin_box_index + n) / _box_size)  % _map_capacity]
                         [(_begin_box_index + n) % _box_size];
    } // circular

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::at(Deque::size_type n) {
        if (n >= size()) {
            throw std::out_of_range("Deque::at() : index is out of range");
        }
        return this->operator[](n);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reference Deque<T, Allocator>::at(Deque::size_type n) const {
        if (n >= size()) {
            throw std::out_of_range("Deque::at() : index is out of range");
        }
        return this->operator[](n);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::front() {
        if (empty()) {
            throw std::out_of_range("Deque::front() : deque is empty");
        }
        return _map[_begin_map_index][_begin_box_index];
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reference Deque<T, Allocator>::front() const {
        if (empty()) {
            throw std::out_of_range("Deque::front() : deque is empty");
        }
        return _get_map()[_begin_map_index][_begin_box_index];
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::back() {
        if (empty()) {
            throw std::out_of_range("Deque::back() : deque is empty");
        }
        return this->operator[](size() - 1);
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::const_reference Deque<T, Allocator>::back() const {
        if (empty()) {
            throw std::out_of_range("Deque::back() : deque is empty");
        }
        return this->operator[](size() - 1);
    }

    template <class T, class Allocator>
    void Deque<T, Allocator>::_for_front() {
        if (_get_begin_box_index() == 0) {
            if (_map_size() == _get_map_capacity()) {
                _reallocate_map(2 * _get_map_capacity());
            }
            if (_get_begin_map_index() == 0) {
                _set_begin_map_index(_get_map_capacity() - 1);
            } else {
                _set_begin_map_index(_get_begin_map_index() - 1);
            }
            if (_get_map()[_get_begin_map_index()] == nullptr) {
                _create_box(_get_begin_map_index());
            }
            _set_begin_box_index(_box_size);
        }
        _set_begin_box_index(_get_begin_box_index() - 1);
        _set_size(size() + 1);
    }

    template <class T, class Allocator>
    void Deque<T, Allocator>::_for_back() {
        if (_get_end_box_index() == _box_size - 1) {
            if (_map_size() == _get_map_capacity()) {
                _reallocate_map(2 * _get_map_capacity());
            }
            if (_get_end_map_index() == _get_map_capacity() - 1) {
                _set_end_map_index(0);
            } else {
                _set_end_map_index(_get_end_map_index() + 1);
            }
            if (_get_map()[_get_end_map_index()] == nullptr) {
                _create_box(_get_end_map_index());
            }
            _set_end_box_index(-1);
        }
        _set_end_box_index(_get_end_box_index() + 1);
        _set_size(size() + 1);
    }

    template <class T, class Allocator>
    template <class... Args>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::emplace_front(Args &&... args) {
        _for_front();
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_begin_map_index()] + _get_begin_box_index(), std::forward<Args>(args)...);
        return _map[_get_begin_map_index()][_get_begin_box_index()];
    }

    template<class T, class Allocator>
    template<class... Args>
    typename Deque<T, Allocator>::reference Deque<T, Allocator>::emplace_back(Args &&... args) {
        if (_get_map()[_get_end_map_index()] == nullptr) {
                _create_box(_get_end_map_index());
        }
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_end_map_index()] + _get_end_box_index(), std::forward<Args>(args)...);
        _for_back();
        return _map[_get_end_map_index()][_get_end_box_index()];
    }

    template<class T, class Allocator>
    template<class... Args>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::emplace(Deque::const_iterator position, Args &&... args) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin > size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::emplace() : index is out of range");
        }
        if (distance_from_begin == 0) {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        } else if (distance_from_begin == size()) {
            emplace_back(std::forward<Args>(args)...);
            return end() - 1;
        } else if (distance_from_begin < size() / 2) {
            reference value = std::move(this->operator[](0));
            for (difference_type  i = 0; i < distance_from_begin; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            this->operator[](distance_from_begin) = value_type(std::forward<Args>(args)...);
            emplace_front(value);
        } else {
            reference value = std::move(this->operator[](size() - 1));
            for (difference_type  i = size() - 1; i > distance_from_begin; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            this->operator[](distance_from_begin) = value_type(std::forward<Args>(args)...);
            emplace_back(value);
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::push_front(const T &value) {
        _for_front();
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_begin_map_index()] + _get_begin_box_index(), value);
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::push_front(T &&value) {
        _for_front();
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_begin_map_index()] + _get_begin_box_index(), std::move(value));
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::push_back(const T &value) {
        if (_get_map()[_get_end_map_index()] == nullptr) {
            _create_box(_get_end_map_index());
        }
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_end_map_index()] + _get_end_box_index(), value);
        _for_back();
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::push_back(T &&value) {
        if (_get_map()[_get_end_map_index()] == nullptr) {
            _create_box(_get_end_map_index());
        }
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_end_map_index()] + _get_end_box_index(), std::move(value));
        _for_back();
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::insert
            (Deque::const_iterator position, const T &value) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin > size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::insert() : index is out of range");
        }
        if (distance_from_begin == 0) {
            emplace_front(value);
            return begin();
        } else if (distance_from_begin == size()) {
            emplace_back(value);
            return end() - 1;
        } else if (distance_from_begin < size() / 2) {
            reference v = std::move(this->operator[](0));
            for (difference_type i = 0; i < distance_from_begin; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            this->operator[](distance_from_begin) = value;
            emplace_front(v);
        } else {
            reference v = std::move(this->operator[](size() - 1));
            for (difference_type i = size() - 1; i > distance_from_begin; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            this->operator[](distance_from_begin) = value;
            emplace_back(v);
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::insert
            (Deque::const_iterator position, T &&value) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin > size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::insert() : index is out of range");
        }
        if (distance_from_begin == 0) {
            emplace_front(std::move(value));
            return begin();
        } else if (distance_from_begin == size()) {
            emplace_back(std::move(value));
            return end() - 1;
        } else if (distance_from_begin < size() / 2) {
            reference v = this->operator[](0);
            for (difference_type i = 0; i < distance_from_begin; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            this->operator[](distance_from_begin) = std::move(value);
            emplace_front(v);
        } else {
            reference v = this->operator[](size() - 1);
            for (difference_type i = size() - 1; i > distance_from_begin; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            this->operator[](distance_from_begin) = std::move(value);
            emplace_back(std::move(v));
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::insert
            (Deque::const_iterator position, Deque::size_type count, const T &value) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin > size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::insert() : index is out of range");
        }
        if (distance_from_begin == 0) {
            for (size_type i = 0; i < count; ++i) {
                emplace_front(value);
            }
            return begin();
        } else if (distance_from_begin == size()) {
            for (size_type i = 0; i < count; ++i) {
                emplace_back(value);
            }
            return end() - count;
        } else if (distance_from_begin < size() / 2) {
            if (distance_from_begin + 1 <= count) {
                T arr[distance_from_begin + 1];
                difference_type i = 0;
                for (; i <= distance_from_begin; ++i) {
                    arr[i] = std::move(this->operator[](i));
                    this->operator[](i) = value;
                }
                for (; i < count; ++i) {
                    emplace_front(value);
                }
                for (i = distance_from_begin; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            } else {
                T arr[count];
                difference_type i = 0;
                for (; i < distance_from_begin - count; ++i) {
                    if (i < count) {
                        arr[i] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i + count));
                }
                for (; i <= distance_from_begin; ++i) {
                    this->operator[](i) = value;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            }
        } else {
            if (size() - distance_from_begin <= count) {
                const size_type current_size = size();
                T arr[current_size - distance_from_begin];
                difference_type i = distance_from_begin;
                for (; i < current_size; ++i) {
                    arr[i - distance_from_begin] = std::move(this->operator[](i));
                    this->operator[](i) = value;
                }
                for (; i < distance_from_begin + count; ++i) {
                    emplace_back(value);
                }
                for (i = current_size - distance_from_begin - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            } else {
                T arr[count];
                const size_type current_size = size();
                difference_type i = current_size - 1;
                for (; i >= distance_from_begin + count; --i) {
                    if (i >= current_size - count) {
                        arr[current_size - i - 1] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i - count));
                }
                for (; i >= distance_from_begin; --i) {
                    this->operator[](i) = value;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::insert
            (Deque::const_iterator position, InputIter first, InputIter last) {
        difference_type count = std::distance(first, last);
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin > size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::insert() : index is out of range");
        }
        if (distance_from_begin == size()) {
            for (; first != last; ++first) {
                emplace_back(*first);
            }
            return end() - count;
        } else if (distance_from_begin < size() / 2) {
            if (distance_from_begin + 1 <= count) {
                T arr[distance_from_begin + 1];
                difference_type i = 0;
                for (; i <= distance_from_begin; ++i, ++first) {
                    arr[i] = std::move(this->operator[](i));
                    this->operator[](i) = *first;
                }
                for (; i < count; ++i, ++first) {
                    emplace_front(*first);
                }
                for (i = distance_from_begin; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            } else {
                T arr[count];
                difference_type i = 0;
                for (; i < distance_from_begin - count; ++i) {
                    if (i < count) {
                        arr[i] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i + count));
                }
                for (; i <= distance_from_begin; ++i, ++first) {
                    this->operator[](i) = *first;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            }
        } else {
            if (size() - distance_from_begin <= count) {
                const size_type current_size = size();
                T arr[current_size - distance_from_begin];
                difference_type i = distance_from_begin;
                for (; i < current_size; ++i, ++first) {
                    arr[i - distance_from_begin] = std::move(this->operator[](i));
                    this->operator[](i) = *first;
                }
                for (; i < distance_from_begin + count; ++i, ++first) {
                    emplace_back(*first);
                }
                for (i = current_size - distance_from_begin - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            } else {
                T arr[count];
                const size_type current_size = size();
                difference_type i = current_size - 1;
                for (; i >= distance_from_begin + count; --i) {
                    if (i >= current_size - count) {
                        arr[current_size - i - 1] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i - count));
                }
                for (i = distance_from_begin; i < distance_from_begin + count; ++i, ++first) {
                    this->operator[](i) = *first;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::insert
            (Deque::const_iterator position, std::initializer_list<T> il) {
        return insert(position, il.begin(), il.end());
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::pop_front() {
        if (empty()) {
            throw std::out_of_range("Deque::pop_front() : deque is empty");
        }
        if (_get_begin_box_index() == _box_size - 1) {
            _set_begin_map_index((_get_begin_map_index() + 1) % _map_capacity);
            _set_begin_box_index(0);
            _set_size(size() - 1);
            if (_get_map_capacity() != default_map_size && _map_size() <= _get_map_capacity() / 4) {
                shrink_to_fit();
            }
        } else {
            _set_begin_box_index(_get_begin_box_index() + 1);
            _set_size(size() - 1);
        }
        std::allocator_traits<box_allocator>::destroy
            (_box_alloc, _map[_get_begin_map_index()] + _get_begin_box_index());
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::pop_back() {
        if (empty()) {
            throw std::out_of_range("Deque::pop_back() : deque is empty");
        }
        if (_get_end_box_index() == 0) {
            _set_end_map_index((_get_end_map_index() - 1) % _map_capacity);
            _set_end_box_index(_box_size - 1);
            _set_size(size() - 1);
            if (_get_map_capacity() != default_map_size && _map_size() <= _get_map_capacity() / 4) {
                shrink_to_fit();
            }
        } else {
            _set_size(size() - 1);
            _set_end_box_index(_get_end_box_index() - 1);
        }
        std::allocator_traits<box_allocator>::destroy
            (_box_alloc, _map[_get_end_map_index()] + _get_end_box_index());
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::erase(Deque::const_iterator position) {
        if (empty()) {
            throw std::out_of_range("Deque::erase() : deque is empty");
        }
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin >= size() || distance_from_begin < 0) {
            throw std::out_of_range("Deque::erase() : index is out of range");
        }
        if (distance_from_begin < size() / 2) {
            for (difference_type i = distance_from_begin; i > 0; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            pop_front();
        } else {
            for (difference_type i = distance_from_begin; i < size() - 1; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            pop_back();
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename Deque<T, Allocator>::iterator Deque<T, Allocator>::erase
            (Deque::const_iterator first, Deque::const_iterator last) {
        if (empty()) {
            throw std::out_of_range("Deque::erase() : deque is empty");
        } else if (first < begin() || last > end()) {
            throw std::out_of_range("Deque::erase() : index is out of range");
        }
        const difference_type count = std::distance(first, last);
        const difference_type distance_from_begin = std::distance(cbegin(), first);
        if (distance_from_begin < size() / 2) {
            difference_type i = distance_from_begin + count - 1;
            for (; i >= count; --i) {
                this->operator[](i) = std::move(this->operator[](i - count));
            }
            for (i = 0; i < count; ++i) {
                pop_front();
            }
        } else {
            difference_type i = distance_from_begin;
            for (; i < size() - count; ++i) {
                this->operator[](i) = std::move(this->operator[](i + count));
            }
            for (i = 0; i < count; ++i) {
                pop_back();
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    void Deque<T, Allocator>::swap(Deque &other)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        using std::swap;
        swap(_map, other._map);
        swap(_size, other._size);
        swap(_map_capacity, other._map_capacity);
        swap(_begin_map_index, other._begin_map_index);
        swap(_begin_box_index, other._begin_box_index);
        swap(_end_map_index, other._end_map_index);
        swap(_end_box_index, other._end_box_index);
        swap(_map_alloc, other._map_alloc);
        swap(_box_alloc, other._box_alloc);
    }
    template<class T, class Allocator>
    void Deque<T, Allocator>::clear() noexcept {
        const size_type target_size = size();
        for (size_type i = 0; i < target_size; ++i) {
            pop_back();
        }
        for (size_type i = 0; i < _map_capacity; ++i) {
            _destroy_box(i);
        }
    }
}