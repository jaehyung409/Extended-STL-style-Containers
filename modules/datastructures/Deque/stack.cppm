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
    export template <class T, class Container = Deque<T>>
    class Stack {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;

    protected:
        Container _c;

    public:
        Stack() : Stack(Container()) {}
        explicit Stack(const Container &cont);
        explicit Stack(Container &&cont);
        //template <class InputIter>
        //Stack(InputIter first, InputIter last);
        //template <container-compatible-range<T> R>
        //Stack(from_range_t, R&& range);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        explicit Stack(const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Stack(const Container &cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Stack(Container &&cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Stack(const Stack &other, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Stack(Stack &&other, const Alloc &alloc);

        //template <class Alloc, class InputIter>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //Stack(InputIter first, InputIter last, const Alloc& alloc);
        //template <class Alloc, container-compatible-range<T> R>
        //Stack(from_range_t, R&& range, const Alloc& alloc);

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
        void swap(Stack &s) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, s._c);
        }
    };

    template <class Container>
    Stack(Container) -> Stack<typename Container::value_type, Container>;

    //template <class InputIter>
    //Stack(InputIter, InputIter) -> Stack<typename std::iterator_traits<InputIter>::value_type>;

    //template <container-compatible-range<T> R>
    //Stack(from_range_t, R&&) -> Stack<typename std::ranges::range_value_t<R>>;

    template <class Container, class Allocator>
    Stack(Container, Allocator) -> Stack<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    Stack(InputIter, InputIter, Allocator)
        -> Stack<typename std::iterator_traits<InputIter>::value_type,
                 Deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    //template <ranges::input_range R, class Allocator>
    //Stack(from_range_t, R&&, Allocator)
    //    -> Stack<std::ranges::range_value_t<R>, Deque<std::ranges::range_value_t<R>, Allocator>>;

    template <class T, class Container, class Alloc>
    struct Uses_allocator<Stack<T, Container>, Alloc> : Uses_allocator<Container, Alloc>::type {};
}

namespace j {
    template <class T, class Container>
    Stack<T, Container>::Stack(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    Stack<T, Container>::Stack(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Stack<T, Container>::Stack(const Alloc &alloc) : _c(Container(), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Stack<T, Container>::Stack(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Stack<T, Container>::Stack(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Stack<T, Container>::Stack(const Stack &other, const Alloc &alloc) : _c(other._c, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Stack<T, Container>::Stack(Stack &&other, const Alloc &alloc) : _c(std::move(other._c, alloc)) {}
}