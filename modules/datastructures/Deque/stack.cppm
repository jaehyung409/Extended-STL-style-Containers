/*
 * @ Created by jaehyung409 on 25. 1. 28.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <initializer_list>
#include <iterator>
#include <ranges>

export module stack;

import basics;
import deque;

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
        //template <class InputIter>
        //stack(InputIter first, InputIter last);
        //template <container-compatible-range<T> R>
        //stack(from_range_t, R&& range);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        explicit stack(const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        stack(const Container &cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        stack(Container &&cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        stack(const stack &other, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        stack(stack &&other, const Alloc &alloc);

        //template <class Alloc, class InputIter>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //stack(InputIter first, InputIter last, const Alloc& alloc);
        //template <class Alloc, container-compatible-range<T> R>
        //stack(from_range_t, R&& range, const Alloc& alloc);

        bool empty() const { return _c.empty(); }
        size_type size() const { return _c.size(); }
        reference top() { return _c.back(); }
        const_reference top() const { return _c.back(); }
        void push(const value_type &value) { _c.push_back(value); }
        void push(value_type &&value) { _c.push_back(std::move(value)); }
        //template<container-compatible-range<T> R>
        //void push_range(R&& rg);
        template <class... Args>
        decltype(auto) emplace(Args &&... args) {
            return _c.emplace_back(std::forward<Args>(args)...);
        }
        void pop() { _c.pop_back(); }
        void swap(stack &s) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, s._c);
        }
    };

    template <class Container>
    stack(Container) -> stack<typename Container::value_type, Container>;

    //template <class InputIter>
    //stack(InputIter, InputIter) -> stack<typename std::iterator_traits<InputIter>::value_type>;

    //template <container-compatible-range<T> R>
    //stack(from_range_t, R&&) -> stack<typename std::ranges::range_value_t<R>>;

    template <class Container, class Allocator>
    stack(Container, Allocator) -> stack<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    stack(InputIter, InputIter, Allocator)
        -> stack<typename std::iterator_traits<InputIter>::value_type,
                 deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    //template <ranges::input_range R, class Allocator>
    //stack(from_range_t, R&&, Allocator)
    //    -> stack<std::ranges::range_value_t<R>, deque<std::ranges::range_value_t<R>, Allocator>>;

    template <class T, class Container, class Alloc>
    struct Uses_allocator<stack<T, Container>, Alloc> : Uses_allocator<Container, Alloc>::type {};
}

namespace j {
    template <class T, class Container>
    stack<T, Container>::stack(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    stack<T, Container>::stack(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    stack<T, Container>::stack(const Alloc &alloc) : _c(Container(), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    stack<T, Container>::stack(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    stack<T, Container>::stack(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    stack<T, Container>::stack(const stack &other, const Alloc &alloc) : _c(other._c, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    stack<T, Container>::stack(stack &&other, const Alloc &alloc) : _c(std::move(other._c, alloc)) {}
}