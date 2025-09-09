/*
 * @ Created by jaehyung409 on 25. 1. 28.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License. 
 */

module;
#include <type_traits>
#include <utility>
#include <iterator>

export module j:stack;

import :deque;

namespace j {
    export template <class T, class Container = deque<T>>
    class stack {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;

    protected:
        Container _c;

    public:
        stack() : stack(Container()) {}
        explicit stack(const Container &cont);
        explicit stack(Container &&cont);
        stack(const stack&) = default;
        stack(stack&&) = default;
        template <class InputIter>
        requires std::input_iterator<InputIter>
        stack(InputIter first, InputIter last);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        explicit stack(const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        stack(const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        stack(Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        stack(const stack &other, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        stack(stack &&other, const Alloc &alloc);

        stack& operator=(const stack&) = default;
        stack& operator=(stack&&) = default;

        template <class InputIter, class Alloc>
        requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
        stack(InputIter first, InputIter last, const Alloc& alloc);
        ~stack() = default;


        [[nodiscard]] bool empty() const { return _c.empty(); }
        [[nodiscard]] size_type size() const { return _c.size(); }
        reference top() { return _c.back(); }
        const_reference top() const { return _c.back(); }
        void push(const value_type &value) { _c.push_back(value); }
        void push(value_type &&value) { _c.push_back(std::move(value)); }
        template <class... Args>
        decltype(auto) emplace(Args &&... args) {
            return _c.emplace_back(std::forward<Args>(args)...);
        }
        void pop() { _c.pop_back(); }
        void swap(stack &s) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, s._c);
        }
        bool operator==(const stack& s) const {
            return _c == s._c;
        }
        auto operator<=>(const stack &s) const {
            return _c <=> s._c;
        }
    };

    template <class Container>
    stack(Container) -> stack<typename Container::value_type, Container>;

    template <class InputIter>
    stack(InputIter, InputIter) -> stack<typename std::iterator_traits<InputIter>::value_type>;

    template <class Container, class Allocator>
    stack(Container, Allocator) -> stack<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    stack(InputIter, InputIter, Allocator)
        -> stack<typename std::iterator_traits<InputIter>::value_type,
                 deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    export template <class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.operator==(rhs);
    }

    export template <class T, class Container>
    auto operator<=>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.operator<=>(rhs);
    }

    export template <class T, class Container>
    void swap(stack<T, Container>& x, stack<T, Container>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }
}

template <class T, class Container, class Alloc>
struct std::uses_allocator<j::stack<T, Container>, Alloc> : uses_allocator<Container, Alloc> {};

namespace j {
    template <class T, class Container>
    stack<T, Container>::stack(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    stack<T, Container>::stack(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template <class InputIter>
requires std::input_iterator<InputIter>
    stack<T, Container>::stack(InputIter first, InputIter last) : _c(first, last) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(const Alloc &alloc) : _c(alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(const stack &other, const Alloc &alloc) : _c(other._c, alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(stack &&other, const Alloc &alloc) : _c(std::move(other._c), alloc) {
    }

    template <class T, class Container>
    template <class InputIter, class Alloc>
requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    stack<T, Container>::stack(InputIter first, InputIter last, const Alloc &alloc) : _c(first, last, alloc) {
    }
}