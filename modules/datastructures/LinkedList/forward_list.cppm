/*
 * @ Created by jaehyung409 on 25. 1. 23.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 */

module;
#include <initializer_list>
#include <iterator>
#include <memory>

export module j:forward_list;

namespace j {
export template <class T, class Allocator = std::allocator<T>> class forward_list {
  public:
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    class iterator;
    class const_iterator;

  private:
    struct _forward_list_node;
    using Node = _forward_list_node;

    using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    Node *_before_head;
    node_allocator _node_alloc;
    // size_type size; didn't use for faster performance

    // helper function (sort)
    template <class Compare> void _sort_impl(forward_list &x, Compare comp);

  public:
    // constructor and destructor
    forward_list() : forward_list(Allocator()) {}
    explicit forward_list(const Allocator &alloc);
    explicit forward_list(size_type n,
                          const Allocator &alloc = Allocator()); // n size forward_list with default value of T.
    forward_list(size_type n, const T &value, const Allocator &alloc = Allocator()); // n size forward_list with value.
    template <class InputIter>
        requires std::input_iterator<InputIter>
    forward_list(InputIter first, InputIter last, const Allocator &alloc = Allocator());

    forward_list(const forward_list &x);
    forward_list(forward_list &&x) noexcept;
    forward_list(const forward_list &x, const std::type_identity_t<Allocator> &);
    forward_list(forward_list &&x, const std::type_identity_t<Allocator> &);
    forward_list(std::initializer_list<T>, const Allocator & = Allocator());

    ~forward_list();

    // assignment
    forward_list &operator=(const forward_list &x);
    forward_list &operator=(forward_list &&x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
    forward_list &operator=(std::initializer_list<T> il);
    template <class InputIt>
        requires std::input_iterator<InputIt>
    void assign(InputIt first, InputIt last);
    void assign(size_type n, const T &t);
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
    template <class... Args> reference emplace_front(Args &&...args);
    void push_front(const T &value);
    void push_front(T &&value);
    void pop_front();

    template <class... Args> iterator emplace_after(const_iterator position, Args &&...args);
    iterator insert_after(const_iterator position, const T &value);
    iterator insert_after(const_iterator position, T &&value);

    iterator insert_after(const_iterator position, size_type count, const T &value);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    iterator insert_after(const_iterator position, InputIter first, InputIter last);
    iterator insert_after(const_iterator position, std::initializer_list<T> il);

    iterator erase_after(const_iterator position);
    iterator erase_after(const_iterator position, const_iterator last);
    void swap(forward_list &x) noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value));

    void resize(size_type nsz);
    void resize(size_type nsz, const T &value);
    void clear() noexcept;

    // forward_list operations
    void splice_after(const_iterator position, forward_list &x);
    void splice_after(const_iterator position, forward_list &&x);
    void splice_after(const_iterator position, forward_list &x, const_iterator i);
    void splice_after(const_iterator position, forward_list &&x, const_iterator i);
    void splice_after(const_iterator position, forward_list &x, const_iterator first, const_iterator last);
    void splice_after(const_iterator position, forward_list &&x, const_iterator first, const_iterator last);

    size_type remove(const T &value);
    template <class Predicate>
        requires std::predicate<Predicate, T>
    size_type remove_if(Predicate pred);

    size_type unique();
    template <class BinaryPredicate>
        requires std::predicate<BinaryPredicate, T, T>
    size_type unique(BinaryPredicate binary_pred = std::equal_to<T>());

    void merge(forward_list &x);
    void merge(forward_list &&x);
    template <class Compare> void merge(forward_list &x, Compare comp);
    template <class Compare> void merge(forward_list &&x, Compare comp);

    void sort();
    template <class Compare>
        requires std::strict_weak_order<Compare, T, T>
    void sort(Compare comp);

    void reverse() noexcept;
};

template <class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
forward_list(InputIter first, InputIter last, const Allocator & = Allocator())
    -> forward_list<typename std::iterator_traits<InputIter>::value_type, Allocator>;

export template <class T, class Allocator>
constexpr bool operator==(const forward_list<T, Allocator> &lhs, const forward_list<T, Allocator> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class T, class Allocator>
constexpr auto operator<=>(const forward_list<T, Allocator> &lhs, const forward_list<T, Allocator> &rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class T, class Allocator>
constexpr void swap(forward_list<T, Allocator> &x, forward_list<T, Allocator> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class T, class Allocator, class U>
constexpr typename forward_list<T, Allocator>::size_type erase(forward_list<T, Allocator> &c, const U &value) {
    return c.remove(value);
}

export template <class T, class Allocator, class Predicate>
constexpr typename forward_list<T, Allocator>::size_type erase_if(forward_list<T, Allocator> &c, Predicate pred) {
    return c.remove_if(pred);
}

template <class T, class Allocator> struct forward_list<T, Allocator>::_forward_list_node {
    friend forward_list;
    friend iterator;
    friend const_iterator;

  private:
    value_type _value;
    _forward_list_node *_next;
};

template <class T, class Allocator> class forward_list<T, Allocator>::iterator {
    friend forward_list;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename forward_list::value_type;
    using difference_type = typename forward_list::difference_type;
    using pointer = typename forward_list::pointer;
    using reference = typename forward_list::reference;

  private:
    using node_pointer = Node *;
    node_pointer _ptr;

  public:
    explicit iterator(node_pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    iterator operator=(const const_iterator &other) noexcept {
        _ptr = other._ptr;
        return *this;
    }

    reference operator*() const noexcept {
        return _ptr->_value;
    }
    pointer operator->() const noexcept {
        return &(_ptr->_value);
    }

    iterator &operator++() noexcept {
        _ptr = _ptr->_next;
        return *this;
    }

    iterator operator++(int) noexcept {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const iterator &other) const noexcept {
        return _ptr == other._ptr;
    }
    operator const_iterator() const noexcept {
        return const_iterator(_ptr);
    }
};

template <class T, class Allocator> class forward_list<T, Allocator>::const_iterator {
    friend forward_list;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const typename forward_list::value_type;
    using difference_type = typename forward_list::difference_type;
    using pointer = typename forward_list::const_pointer;
    using reference = typename forward_list::const_reference;
    using const_pointer = typename forward_list::const_pointer;
    using const_reference = typename forward_list::const_reference;

  private:
    using node_pointer = Node *;
    node_pointer _ptr;

  public:
    explicit const_iterator(node_pointer ptr = nullptr) noexcept : _ptr(ptr) {}
    explicit const_iterator(const iterator &other) noexcept : _ptr(other._ptr) {}
    const_iterator operator=(const iterator &other) noexcept {
        _ptr = other._ptr;
        return *this;
    }

    const_reference operator*() const noexcept {
        return _ptr->_value;
    }
    const_pointer operator->() const noexcept {
        return &(_ptr->_value);
    }

    const_iterator &operator++() noexcept {
        _ptr = _ptr->_next;
        return *this;
    }

    const_iterator operator++(int) noexcept {
        const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const const_iterator &other) const noexcept {
        return _ptr == other._ptr;
    }
};
} // namespace j

namespace j {
template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(const Allocator &alloc)
    : _node_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    _before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
    std::construct_at(&_before_head->_value);
    _before_head->_next = nullptr; // Initialize the next pointer to nullptr
}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(const size_type n, const Allocator &alloc)
    : forward_list(n, T(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(const size_type n, const T &value, const Allocator &alloc)
    : forward_list(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    for (size_type i = 0; i < n; ++i) {
        emplace_front(value);
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
forward_list<T, Allocator>::forward_list(InputIter first, InputIter last, const Allocator &alloc)
    : forward_list(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    auto it = before_begin();
    for (; first != last; ++first) {
        it = emplace_after(it, *first);
    }
}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(const forward_list &x)
    : forward_list(x.begin(), x.end(),
                   std::allocator_traits<Allocator>::select_on_container_copy_construction(x._node_alloc)) {}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(forward_list &&x) noexcept
    : _before_head(x._before_head), _node_alloc(x._node_alloc) {
    x._before_head = std::allocator_traits<node_allocator>::allocate(x._node_alloc, 1);
    std::construct_at(&x._before_head->_value);
    x._before_head->_next = nullptr; // Initialize the next pointer to nullptr
}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(const forward_list &x, const std::type_identity_t<Allocator> &alloc)
    : forward_list(x.begin(), x.end(), std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(forward_list &&x, const std::type_identity_t<Allocator> &alloc)
    : _before_head(x._before_head),
      _node_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
    x._before_head = std::allocator_traits<node_allocator>::allocate(x._node_alloc, 1);
    std::construct_at(&x._before_head->_value);
    x._before_head->_next = nullptr; // Initialize the next pointer to nullptr
}

template <class T, class Allocator>
forward_list<T, Allocator>::forward_list(std::initializer_list<T> il, const Allocator &alloc)
    : forward_list(il.begin(), il.end(),
                   std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {}

template <class T, class Allocator> forward_list<T, Allocator>::~forward_list() {
    clear();
    std::destroy_at(&_before_head->_value);
    std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);
}

template <class T, class Allocator>
forward_list<T, Allocator> &forward_list<T, Allocator>::operator=(const forward_list &x) {
    if (this != std::addressof(x)) {
        clear();
        auto it = before_begin();
        for (auto xit = x.begin(); xit != x.end(); ++xit) {
            it = emplace_after(it, *xit);
        }
    }
    return *this;
}

template <class T, class Allocator>
forward_list<T, Allocator> &forward_list<T, Allocator>::operator=(forward_list &&x) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this != std::addressof(x)) {
        clear();
        std::destroy_at(&_before_head->_value);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);
        _before_head = x._before_head;
        _node_alloc = x._node_alloc;

        x._before_head = std::allocator_traits<node_allocator>::allocate(x._node_alloc, 1);
        std::construct_at(&x._before_head->_value);
        x._before_head->_next = nullptr; // Initialize the next pointer to nullptr
    }
    return *this;
}

template <class T, class Allocator>
forward_list<T, Allocator> &forward_list<T, Allocator>::operator=(std::initializer_list<T> il) {
    clear();
    auto it = before_begin();
    for (const T &t : il) {
        it = emplace_after(it, t);
    }
    return *this;
}

template <class T, class Allocator>
template <class InputIt>
    requires std::input_iterator<InputIt>
void forward_list<T, Allocator>::assign(InputIt first, InputIt last) {
    clear();
    auto it = before_begin();
    for (; first != last; ++first) {
        it = emplace_after(it, *first);
    }
}

template <class T, class Allocator> void forward_list<T, Allocator>::assign(size_type count, const T &value) {
    clear();
    for (int i = 0; i < count; i++) {
        emplace_front(value);
    }
}

template <class T, class Allocator> void forward_list<T, Allocator>::assign(std::initializer_list<T> il) {
    assign(il.begin(), il.end());
}

template <class T, class Allocator> std::allocator<T> forward_list<T, Allocator>::get_allocator() const noexcept {
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
    return iterator(_before_head->_next);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::begin() const noexcept {
    return const_iterator(_before_head->_next);
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
    return const_iterator(_before_head->_next);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::cbefore_begin() const noexcept {
    return iterator(_before_head);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::const_iterator forward_list<T, Allocator>::cend() const noexcept {
    return const_iterator(nullptr);
}

template <class T, class Allocator> bool forward_list<T, Allocator>::empty() const noexcept {
    return _before_head->_next == nullptr;
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::max_size() const noexcept {
    return std::allocator_traits<node_allocator>::max_size(_node_alloc);
}

template <class T, class Allocator> T &forward_list<T, Allocator>::front() {
    return _before_head->_next->_value;
}

template <class T, class Allocator> const T &forward_list<T, Allocator>::front() const {
    return _before_head->_next->_value;
}

template <class T, class Allocator>
template <class... Args>
T &forward_list<T, Allocator>::emplace_front(Args &&...args) {
    Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
    std::construct_at(&new_node->_value, std::forward<Args>(args)...);
    new_node->_next = _before_head->_next;
    _before_head->_next = new_node;
    return new_node->_value;
}

template <class T, class Allocator> void forward_list<T, Allocator>::push_front(const T &value) {
    emplace_front(value);
}

template <class T, class Allocator> void forward_list<T, Allocator>::push_front(T &&value) {
    emplace_front(std::move(value));
}

template <class T, class Allocator> void forward_list<T, Allocator>::pop_front() {
    Node *del_node = _before_head->_next;
    if (!del_node)
        return;

    _before_head->_next = del_node->_next;
    std::destroy_at(&del_node->_value);
    std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
}

template <class T, class Allocator>
template <class... Args>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::emplace_after(const_iterator position,
                                                                                        Args &&...args) {
    Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
    std::construct_at(&new_node->_value, std::forward<Args>(args)...);

    new_node->_next = position._ptr->_next;
    position._ptr->_next = new_node;

    return iterator(new_node);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::insert_after(const_iterator position,
                                                                                       const T &value) {
    Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
    std::construct_at(&new_node->_value, value);

    new_node->_next = position._ptr->_next;
    position._ptr->_next = new_node;

    return iterator(new_node);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::insert_after(const_iterator position,
                                                                                       T &&value) {
    Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
    std::construct_at(&new_node->_value, value);

    new_node->_next = position._ptr->_next;
    position._ptr->_next = new_node;

    return iterator(new_node);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator position, size_type count, const T &value) {
    for (int i = 0; i < count; i++) {
        position = insert_after(position, value);
    }
    return iterator(position._ptr);
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator position, InputIter first, InputIter last) {
    for (; first != last; ++first) {
        position = emplace_after(position, *first);
    }
    return iterator(position._ptr);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::insert_after(const_iterator position,
                                                                                       std::initializer_list<T> il) {
    return insert_after(position, il.begin(), il.end());
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::erase_after(const_iterator position) {
    Node *del_node = position._ptr->_next;
    if (!del_node)
        return end();

    position._ptr->_next = del_node->_next;
    std::destroy_at(&del_node->_value);
    std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
    return iterator(position._ptr->_next);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::erase_after(const_iterator position,
                                                                                      const_iterator last) {
    const_iterator del_pos = std::next(position);
    while (del_pos != last) {
        del_pos = erase_after(position);
    }
    return iterator(last._ptr);
}

template <class T, class Allocator>
void forward_list<T, Allocator>::swap(forward_list &x) noexcept(
    noexcept(std::allocator_traits<node_allocator>::is_always_equal::value)) {
    using std::swap;
    swap(_before_head, x._before_head);
    swap(_node_alloc, x._node_alloc);
}

template <class T, class Allocator> void forward_list<T, Allocator>::resize(size_type nsz) {
    resize(nsz, T());
}

template <class T, class Allocator> void forward_list<T, Allocator>::resize(size_type nsz, const T &value) {
    size_type size = 0;
    auto prev = before_begin();
    auto it = begin();
    while (size < nsz && it != end()) {
        ++prev;
        ++it;
        ++size;
    }
    if (size == nsz) {
        if (it == end()) {
            return;
        }
        while (std::next(prev) != end()) {
            erase_after(prev);
        }
    } else {
        while (size < nsz) {
            prev = emplace_after(prev, value);
            ++size;
        }
    }
}

template <class T, class Allocator> void forward_list<T, Allocator>::clear() noexcept {
    for (auto it = begin(); it != end();) {
        auto temp = it;
        ++it;
        std::destroy_at(temp._ptr);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, temp._ptr, 1);
    }
    _before_head->_next = nullptr;
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &x) {
    if (!x.empty()) {
        iterator last = x.before_begin();
        while (std::next(last) != x.end()) {
            ++last;
        }
        last._ptr->_next = position._ptr->_next;
        position._ptr->_next = x._before_head->_next;
        x._before_head->_next = nullptr;
    }
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &&x) {
    splice_after(position, x);
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &x, const_iterator i) {
    Node *moved = i._ptr->_next;
    if (!moved)
        return;
    i._ptr->_next = i._ptr->_next->_next;
    moved->_next = position._ptr->_next;
    position._ptr->_next = moved;
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &&x, const_iterator i) {
    splice_after(position, x, i);
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &x, const_iterator first,
                                              const_iterator last) {
    if (first == last)
        return;
    const_iterator last_before = first;
    while (std::next(last_before) != last) {
        ++last_before;
    }
    last_before._ptr->_next = position._ptr->_next;
    position._ptr->_next = first._ptr->_next;
    first._ptr->_next = last._ptr;
}

template <class T, class Allocator>
void forward_list<T, Allocator>::splice_after(const_iterator position, forward_list &&other, const_iterator first,
                                              const_iterator last) {
    splice_after(position, other, first, last);
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::remove(const T &value) {
    size_type count = 0;
    auto it = before_begin();

    while (std::next(it) != end()) {
        if (*std::next(it) == value) {
            erase_after(it);
            ++count;
        } else {
            ++it;
        }
    }

    return count;
}

template <class T, class Allocator>
template <class Predicate>
    requires std::predicate<Predicate, T>
typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::remove_if(Predicate pred) {
    size_type count = 0;

    auto it = before_begin();

    while (std::next(it) != end()) {
        if (pred(*std::next(it))) {
            erase_after(it);
            ++count;
        } else {
            ++it;
        }
    }

    return count;
}

template <class T, class Allocator>
typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::unique() {
    return unique(std::equal_to<T>());
}

template <class T, class Allocator>
template <class BinaryPredicate>
    requires std::predicate<BinaryPredicate, T, T>
typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::unique(BinaryPredicate binary_pred) {
    size_type count = 0;

    auto it = before_begin();

    while (std::next(it) != end() && std::next(std::next(it)) != end()) {
        if (binary_pred(*std::next(it), *std::next(std::next(it)))) {
            erase_after(it);
            ++count;
        } else {
            ++it;
        }
    }
    return count;
}

template <class T, class Allocator> void forward_list<T, Allocator>::merge(forward_list &x) {
    merge(x, std::less<T>());
}

template <class T, class Allocator> void forward_list<T, Allocator>::merge(forward_list &&x) {
    merge(x, std::less<T>());
}

template <class T, class Allocator>
template <class Compare>
void forward_list<T, Allocator>::merge(forward_list &x, Compare comp) {
    if (this == std::addressof(x) || x.empty())
        return;

    iterator it = before_begin();
    iterator xit = x.before_begin();
    while (std::next(it) != end() && std::next(xit) != x.end()) {
        if (!comp(*std::next(xit), *std::next(it))) {
            ++it;
        } else {
            splice_after(it, x, xit);
            ++it;
        }
    }
    if (std::next(xit) != x.end()) {
        splice_after(it, x, xit, x.end());
    }
}

template <class T, class Allocator>
template <class Compare>
void forward_list<T, Allocator>::merge(forward_list &&x, Compare comp) {
    merge(static_cast<forward_list &>(x), comp);
}

template <class T, class Allocator> void forward_list<T, Allocator>::sort() {
    _sort_impl(*this, std::less<T>());
}

template <class T, class Allocator>
template <class Compare>
    requires std::strict_weak_order<Compare, T, T>
void forward_list<T, Allocator>::sort(Compare comp) {
    _sort_impl(*this, comp);
}

template <class T, class Allocator> void forward_list<T, Allocator>::reverse() noexcept {
    if (empty())
        return;
    Node *prev = nullptr;
    Node *current = _before_head->_next;
    while (current != nullptr) {
        Node *next = current->_next;
        current->_next = prev;
        prev = current;
        current = next;
    }
    _before_head->_next = prev;
}

template <class T, class Allocator>
template <class Compare>
void forward_list<T, Allocator>::_sort_impl(forward_list &x, Compare comp) {
    if (x.empty() || std::next(x.begin()) == x.end()) {
        return;
    }
    auto slow = x.begin();
    auto fast = x.begin();
    while (std::next(fast) != x.end() && std::next(fast, 2) != x.end()) {
        std::advance(slow, 1);
        std::advance(fast, 2);
    }

    forward_list<T, Allocator> temp;
    temp.splice_after(temp.before_begin(), x, slow, x.end());
    _sort_impl(x, comp);
    _sort_impl(temp, comp);
    x.merge(temp, comp);
}
} // namespace j
