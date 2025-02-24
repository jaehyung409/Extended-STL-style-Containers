/*
 * @ Created by jaehyung409 on 25. 2. 12.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIKey License.
 */

module;
#include <functional>
#include <initializer_list>

export module j.set;

import j.red_black_tree;
import j.avl_tree;
import j.tree_helper;

namespace j {
    export template <class Key, class Compare, class Container>
    class multiset;
}

namespace j {
    export template <class Key,
                     class Compare = std::less<Key>,
                     class Container = red_black_tree<std::pair<const Key, _dummy>, Compare>>
    class set {
    public:
        using key_type                  = typename Container::key_type;
        using key_compare               = typename Container::key_compare;
        using value_type                = typename Container::value_type;
        using value_compare             = typename Container::value_compare;
        using allocator_type            = typename Container::allocator_type;
        using pointer                   = typename Container::pointer;
        using const_pointer             = typename Container::const_pointer;
        using reference                 = typename Container::reference;
        using const_reference           = typename Container::const_reference;
        using size_type                 = typename Container::size_type;
        using difference_type           = typename Container::difference_type;
        using iterator                  = typename Container::iterator;
        using const_iterator            = typename Container::const_iterator;
        using reverse_iterator          = typename Container::reverse_iterator;
        using const_reverse_iterator    = typename Container::const_reverse_iterator;
        using node_type                 = typename Container::node_type;
        using insert_return_type        = typename Container::insert_return_type;

    protected:
        Container _container;
        Compare _comp;

    public:
        set() : set(Compare()) {}
        explicit set(const Compare &comp) : set(comp, Container()) {}
        set(const Compare& comp, const Container &cont);
        set(const Compare& comp, Container &&cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        set(InputIter first, InputIter last, const Compare &comp = Compare());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        set(InputIter first, InputIter last, const Compare &comp, const Container &cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        set(InputIter first, InputIter last, const Compare &comp, Container &&cont);
        //template <container-compatible-range<Key> R>
        //set(from_range_t, R&& range, const Compare &comp = Compare());
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        set(const Compare &comp, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        set(const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        set(const Compare &comp, Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        set(const set &other, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        set(set &&other, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        set(InputIter first, InputIter last, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        set(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        set(InputIter first, InputIter last, const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        set(InputIter first, InputIter last, const Compare &comp, Container &&cont, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //set(from_range_t, R&& range, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>

        iterator begin() noexcept { return _container.begin(); }
        const_iterator begin() const noexcept { return _container.begin(); }
        iterator end() noexcept { return _container.end(); }
        const_iterator end() const noexcept { return _container.end(); }
        reverse_iterator rbegin() noexcept { return _container.rbegin(); }
        const_reverse_iterator rbegin() const noexcept { return _container.rbegin(); }
        reverse_iterator rend() noexcept { return _container.rend(); }
        const_reverse_iterator rend() const noexcept { return _container.rend(); }
        const_iterator cbegin() const noexcept { return _container.cbegin(); }
        const_iterator cend() const noexcept { return _container.cend(); }
        const_iterator crbegin() const noexcept { return _container.crbegin(); }
        const_iterator crend() const noexcept { return _container.crend(); }

        bool empty() const noexcept { return _container.empty(); }
        size_type size() const noexcept { return _container.size(); }
        size_type max_size() const noexcept { return _container.max_size(); }

        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&... args) {
            return _container.emplace_unique(std::forward<Args>(args)...);
        }
        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args &&... args) {
            return _container.emplace_hint_unique(hint, std::forward<Args>(args)...);
        }
        std::pair<iterator, bool> insert(const value_type &x) { return _container.insert_unique(x).first; }
        std::pair<iterator, bool> insert(value_type &&x) { return _container.insert_unique(std::move(x)).first; }
        template <class K>
        std::pair<iterator, bool> insert(K &&x) { return _container.insert_unique(std::forward<K>(x)).first; }
        iterator insert(const_iterator hint, const value_type &x) { return _container.insert_unique(hint, x); }
        iterator insert(const_iterator hint, value_type &&x) {
            return _container.insert_unique(hint, std::move(x));
        }
        template <class K>
        iterator insert(const_iterator hint, K &&x) { return _container.insert_unique(hint, std::forward<K>(x)); }
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert(InputIter first, InputIter last) { return _container.insert_unique(first, last); }
        // template <container-compatible-range<Key> R>
        // void insert_range(R &&rg) { return _container.insert_unique(rg); }
        void insert(std::initializer_list<value_type> il) { return _container.insert_unique(il); }

        node_type extract(const_iterator position) { return _container.extract(position); }
        node_type extract(const key_type &key) { return _container.extract(key); }
        template <class K>
        node_type extract(K &&key) { return _container.extract(std::forward<K>(key)); }
        insert_return_type insert(node_type &&node) { return _container.insert_unique(std::move(node)); }
        iterator insert(const_iterator hint, node_type &&node) {
            return _container.insert_unique(hint, std::move(node));
        }
        iterator erase(iterator pos)
        requires std::is_same_v<iterator, const_iterator> { return _container.erase(pos); }
        iterator erase(const_iterator pos) { return _container.erase(pos); }
        size_type erase(const key_type &key) { return _container.erase(key); }
        template <class K>
        size_type erase(K &&key) { return _container.erase(std::forward<K>(key)); }
        iterator erase(const_iterator first, const_iterator last) { return _container.erase(first, last); }
        void swap(set &other) noexcept(
            std::is_nothrow_swappable_v<Container> &&
            std::is_nothrow_swappable_v<Compare>
        ) {
            using std::swap;
            swap(_container, other._container);
            swap(_comp, other._comp);
        }

        void clear() noexcept { _container.clear(); }

        template <class C2>
        void merge(set<key_type, C2, allocator_type> &source) { _container.merge(source._container); }
        template <class C2>
        void merge(set<key_type, C2, allocator_type> &&source) { _container.merge(std::move(source._container)); }
        template <class C2>
        void merge(multiset<key_type, C2, allocator_type> &source) {
            _container.merge(source._container);
        }
        template <class C2>
        void merge(multiset<key_type, C2, allocator_type> &&source) {
            _container.merge(std::move(source._container));
        }

        key_compare key_comp() const { return _container.key_comp(); }
        value_compare value_comp() const { return _container.value_comp(); }

        iterator find(const key_type &key) { return _container.find(key); }
        const_iterator find(const key_type &key) const { return _container.find(key); }
        template <class K>
        iterator find(K &&key) { return _container.find(std::forward<K>(key)); }
        template <class K>
        const_iterator find(K &&key) const { return _container.find(std::forward<K>(key)); }

        size_type count(const key_type &key) const { return _container.count(key); }
        template <class K>
        size_type count(K &&key) const { return _container.count(std::forward<K>(key)); }

        bool contains(const key_type &key) const { return _container.contains(key); }
        template <class K>
        bool contains(K &&key) const { return _container.contains(std::forward<K>(key)); }

        iterator lower_bound(const key_type &key) { return _container.lower_bound(key); }
        const_iterator lower_bound(const key_type &key) const { return _container.lower_bound(key); }
        template <class K>
        iterator lower_bound(K &&key) { return _container.lower_bound(std::forward<K>(key)); }
        template <class K>
        const_iterator lower_bound(K &&key) const { return _container.lower_bound(std::forward<K>(key)); }

        iterator upper_bound(const key_type &key) { return _container.upper_bound(key); }
        const_iterator upper_bound(const key_type &key) const { return _container.upper_bound(key); }
        template <class K>
        iterator upper_bound(K &&key) { return _container.upper_bound(std::forward<K>(key)); }
        template <class K>
        const_iterator upper_bound(K &&key) const { return _container.upper_bound(std::forward<K>(key)); }

        std::pair<iterator, iterator> equal_range(const key_type &key) { return _container.equal_range(key); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
            return _container.equal_range(key);
        }
        template <class K>
        std::pair<iterator, iterator> equal_range(K &&key) { return _container.equal_range(std::forward<K>(key)); }
        template <class K>
        std::pair<const_iterator, const_iterator> equal_range(K &&key) const {
            return _container.equal_range(std::forward<K>(key));
        }
    };

    template <class Compare, class Container>
    set(Compare, Container)
        -> set<typename Container::value_type, Compare, Container>;

    template <class InputIter,
              class Compare    = std::less<typename std::iterator_traits<InputIter>::value_type>,
              class Container  = red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type,
                                                          _dummy>>,
                                                Compare>
    set(InputIter, InputIter, Compare = Compare(), Container = Container())
        -> set<typename std::iterator_traits<InputIter>::value_type, Compare, Container>;

    //template <container-compatible-range<Key> R,
    //          class Compare = std::less<Key>,
    //          class Container = red_black_tree<const Key, _dummy>, Compare>
    //set(from_range_t, R&&, Compare = Compare(), Container = Container())
    //    -> set<Key, Container, Compare>;

    template <class Compare, class Container, class Alloc>
    set(Compare, Container, Alloc)
        -> set<typename Container::value_type, Compare, Container>;

    template <class InputIter, class Alloc>
    set(InputIter, InputIter, Alloc)
        -> set<typename std::iterator_traits<InputIter>::value_type,
               red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type, _dummy>,
                              std::less<typename std::iterator_traits<InputIter>::value_type>>,
               std::less<typename  std::iterator_traits<InputIter>::value_type>>;

    template <class InputIter, class Compare, class Alloc>
    set(InputIter, InputIter, Compare, Alloc)
        -> set<typename std::iterator_traits<InputIter>::value_type,
               red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type, _dummy>, Compare>,
               Compare>;

    template <class InputIter, class Compare, class Container, class Alloc>
    set(InputIter, InputIter, Compare, Container, Alloc)
        -> set<typename std::iterator_traits<InputIter>::value_type, Compare, Container>;

    template <class Key, class Compare = std::less<Key>, class Container = red_black_tree<Key>>
    set(std::initializer_list<Key>, Compare = Compare(), Container = Container())
        -> set<Key, Compare, Container>;

    template <class Key, class Compare, class Alloc>
    set(std::initializer_list<Key>, Compare, Alloc)
        -> set<Key, Compare>;


    //template <container-compatible-range<Key> R, class Compare, class Container, class Alloc>
    //set(from_range_t, R&&, Compare, Container, Alloc)
    //    -> set<Key, Compare, Container>;

    //template <container-compatible-range<Key> R, class Alloc>
    //set(from_range_t, R&&, Alloc)
    //    -> set<Key, red_black_tree<std::pair<const Key, _dummy>, std::less<Key>>>;
}

namespace j {
    template <class Key, class Compare, class Container>
    set<Key, Compare, Container>::set(const Compare& comp, const Container& cont)
        : _container(cont), _comp(comp) {}

    template <class Key, class Compare, class Container>
    set<Key, Compare, Container>::set(const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    set<Key, Compare, Container>::set(InputIter first, InputIter last, const Compare& comp)
        : set(first, last, comp, Container()) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    set<Key, Compare, Container>::set(InputIter first, InputIter last, const Compare& comp, const Container& cont)
        : _container(cont(first, last)), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    set<Key, Compare, Container>::set(InputIter first, InputIter last, const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {
        _container.clear();
        _container.insert_unique(first, last);
    }

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(const Compare& comp, const Alloc& alloc) : set(comp, Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont, alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::
        value_type; }
    set<Key, Compare, Container>::set(const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(const set& other, const Alloc& alloc)
        : _container(other, alloc) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(set&& other, const Alloc& alloc) : _container(std::move(other), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(InputIter first, InputIter last, const Alloc& alloc)
        : set(first, last, Compare(), Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::set(InputIter first, InputIter last, const Compare& comp, const Alloc& alloc)
        : set(first, last, comp, Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::
    set(InputIter first, InputIter last, const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont(first, last), alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    set<Key, Compare, Container>::
    set(InputIter first, InputIter last, const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {
        _container.clear();
        _container.insert_unique(first, last);
    }
}

namespace j {
    template <class Key,
                     class Compare = std::less<Key>,
                     class Container = red_black_tree<std::pair<Key, _dummy>>>
    class multiset {
    public:
        using key_type                  = typename Container::key_type;
        using key_compare               = typename Container::key_compare;
        using value_type                = typename Container::value_type;
        using value_compare             = typename Container::value_compare;
        using allocator_type            = typename Container::allocator_type;
        using pointer                   = typename Container::pointer;
        using const_pointer             = typename Container::const_pointer;
        using reference                 = typename Container::reference;
        using const_reference           = typename Container::const_reference;
        using size_type                 = typename Container::size_type;
        using difference_type           = typename Container::difference_type;
        using iterator                  = typename Container::iterator;
        using const_iterator            = typename Container::const_iterator;
        using reverse_iterator          = typename Container::reverse_iterator;
        using const_reverse_iterator    = typename Container::const_reverse_iterator;
        using node_type                 = typename Container::node_type;
        using insert_return_type        = typename Container::insert_return_type;

    protected:
        Container _container;
        Compare _comp;

    public:
        multiset() : multiset(Compare()) {}
        explicit multiset(const Compare &comp) : multiset(comp, Container()) {}
        multiset(const Compare& comp, const Container &cont);
        multiset(const Compare& comp, Container &&cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multiset(InputIter first, InputIter last, const Compare &comp = Compare());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multiset(InputIter first, InputIter last, const Compare &comp, const Container &cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multiset(InputIter first, InputIter last, const Compare &comp, Container &&cont);
        //template <container-compatible-range<Key> R>
        //multiset(from_range_t, R&& range, const Compare &comp = Compare());
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(const Compare &comp, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(const Compare &comp, Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(const multiset &other, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(multiset &&other, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(InputIter first, InputIter last, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(InputIter first, InputIter last, const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multiset(InputIter first, InputIter last, const Compare &comp, Container &&cont, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //multiset(from_range_t, R&& range, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>

        iterator begin() noexcept { return _container.begin(); }
        const_iterator begin() const noexcept { return _container.begin(); }
        iterator end() noexcept { return _container.end(); }
        const_iterator end() const noexcept { return _container.end(); }
        reverse_iterator rbegin() noexcept { return _container.rbegin(); }
        const_reverse_iterator rbegin() const noexcept { return _container.rbegin(); }
        reverse_iterator rend() noexcept { return _container.rend(); }
        const_reverse_iterator rend() const noexcept { return _container.rend(); }
        const_iterator cbegin() const noexcept { return _container.cbegin(); }
        const_iterator cend() const noexcept { return _container.cend(); }
        const_iterator crbegin() const noexcept { return _container.crbegin(); }
        const_iterator crend() const noexcept { return _container.crend(); }

        bool empty() const noexcept { return _container.empty(); }
        size_type size() const noexcept { return _container.size(); }
        size_type max_size() const noexcept { return _container.max_size(); }

        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&... args) {
            return _container.emplace(std::forward<Args>(args)...);
        }
        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args &&... args) {
            return _container.emplace_hint(hint, std::forward<Args>(args)...);
        }
        std::pair<iterator, bool> insert(const value_type &x) { return _container.insert(x).first; }
        std::pair<iterator, bool> insert(value_type &&x) { return _container.insert(std::move(x)).first; }
        template <class K>
        std::pair<iterator, bool> insert(K &&x) { return _container.insert(std::forward<K>(x)).first; }
        iterator insert(const_iterator hint, const value_type &x) { return _container.insert(hint, x); }
        iterator insert(const_iterator hint, value_type &&x) { return _container.insert(hint, std::move(x)); }
        template <class K>
        iterator insert(const_iterator hint, K &&x) { return _container.insert(hint, std::forward<K>(x)); }
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert(InputIter first, InputIter last) { return _container.insert(first, last); }
        // template <container-compatible-range<Key> R>
        // void insert_range(R &&rg) { return _container.insert(rg); }
        void insert(std::initializer_list<value_type> il) { return _container.insert(il); }

        node_type extract(const_iterator position) { return _container.extract(position); }
        node_type extract(const key_type &key) { return _container.extract(key); }
        template <class K>
        node_type extract(K &&key) { return _container.extract(std::forward<K>(key)); }
        insert_return_type insert(node_type &&node) { return _container.insert(std::move(node)); }
        iterator insert(const_iterator hint, node_type &&node) { return _container.insert(hint, std::move(node)); }

        iterator erase(iterator pos)
        requires std::is_same_v<iterator, const_iterator> { return _container.erase(pos); }
        iterator erase(const_iterator pos) { return _container.erase(pos); }
        size_type erase(const key_type &key) { return _container.erase(key); }
        template <class K>
        size_type erase(K &&key) { return _container.erase(std::forward<K>(key)); }
        iterator erase(const_iterator first, const_iterator last) { return _container.erase(first, last); }
        void swap(multiset &other) noexcept(
            std::is_nothrow_swappable_v<Container> &&
            std::is_nothrow_swappable_v<Compare>
        ) {
            using std::swap;
            swap(_container, other._container);
            swap(_comp, other._comp);
        }

        void clear() noexcept { _container.clear(); }

        template <class C2>
        void merge(multiset<key_type, C2, allocator_type> &source) { _container.merge(source._container); }
        template <class C2>
        void merge(multiset<key_type, C2, allocator_type> &&source) { _container.merge(std::move(source._container)); }
        template <class C2>
        void merge(set<key_type, C2, allocator_type> &source) { _container.unique_merge(source._container); }
        template <class C2>
        void merge(set<key_type, C2, allocator_type> &&source) { _container.unique_merge(std::move(source._container)); }

        key_compare key_comp() const { return _container.key_comp(); }
        value_compare value_comp() const { return _container.value_comp(); }

        iterator find(const key_type &key) { return _container.find(key); }
        const_iterator find(const key_type &key) const { return _container.find(key); }
        template <class K>
        iterator find(K &&key) { return _container.find(std::forward<K>(key)); }
        template <class K>
        const_iterator find(K &&key) const { return _container.find(std::forward<K>(key)); }

        size_type count(const key_type &key) const { return _container.count(key); }
        template <class K>
        size_type count(K &&key) const { return _container.count(std::forward<K>(key)); }

        bool contains(const key_type &key) const { return _container.contains(key); }
        template <class K>
        bool contains(K &&key) const { return _container.contains(std::forward<K>(key)); }

        iterator lower_bound(const key_type &key) { return _container.lower_bound(key); }
        const_iterator lower_bound(const key_type &key) const { return _container.lower_bound(key); }
        template <class K>
        iterator lower_bound(K &&key) { return _container.lower_bound(std::forward<K>(key)); }
        template <class K>
        const_iterator lower_bound(K &&key) const { return _container.lower_bound(std::forward<K>(key)); }

        iterator upper_bound(const key_type &key) { return _container.upper_bound(key); }
        const_iterator upper_bound(const key_type &key) const { return _container.upper_bound(key); }
        template <class K>
        iterator upper_bound(K &&key) { return _container.upper_bound(std::forward<K>(key)); }
        template <class K>
        const_iterator upper_bound(K &&key) const { return _container.upper_bound(std::forward<K>(key)); }

        std::pair<iterator, iterator> equal_range(const key_type &key) { return _container.equal_range(key); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const { return _container.equal_range(key); }
        template <class K>
        std::pair<iterator, iterator> equal_range(K &&key) { return _container.equal_range(std::forward<K>(key)); }
        template <class K>
        std::pair<const_iterator, const_iterator> equal_range(K &&key) const { return _container.equal_range(std::forward<K>(key)); }
    };

    template <class Compare, class Container>
    multiset(Compare, Container)
        -> multiset<typename Container::value_type, Compare, Container>;

    template <class InputIter,
              class Compare    = std::less<typename std::iterator_traits<InputIter>::value_type>,
              class Container  = red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type,
                                                          _dummy>>,
                                                Compare>
    multiset(InputIter, InputIter, Compare = Compare(), Container = Container())
        -> multiset<typename std::iterator_traits<InputIter>::value_type, Compare, Container>;

    //template <container-compatible-range<Key> R,
    //          class Compare = std::less<Key>,
    //          class Container = red_black_tree<const Key, _dummy>, Compare>
    //multiset(from_range_t, R&&, Compare = Compare(), Container = Container())
    //    -> multiset<Key, Container, Compare>;

    template <class Compare, class Container, class Alloc>
    multiset(Compare, Container, Alloc)
        -> multiset<typename Container::value_type, Compare, Container>;

    template <class InputIter, class Alloc>
    multiset(InputIter, InputIter, Alloc)
        -> multiset<typename std::iterator_traits<InputIter>::value_type,
                    red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type, _dummy>,
                          std::less<typename std::iterator_traits<InputIter>::value_type>>,
                    std::less<typename  std::iterator_traits<InputIter>::value_type>>;

    template <class InputIter, class Compare, class Alloc>
    multiset(InputIter, InputIter, Compare, Alloc)
        -> multiset<typename std::iterator_traits<InputIter>::value_type,
                    red_black_tree<std::pair<const typename std::iterator_traits<InputIter>::value_type, _dummy>,
                                             Compare>,
                    Compare>;

    template <class InputIter, class Compare, class Container, class Alloc>
    multiset(InputIter, InputIter, Compare, Container, Alloc)
        -> multiset<typename std::iterator_traits<InputIter>::value_type, Compare, Container>;

    template <class Key, class Compare = std::less<Key>, class Container = red_black_tree<Key>>
    multiset(std::initializer_list<Key>, Compare = Compare(), Container = Container())
        -> multiset<Key, Compare, Container>;

    template <class Key, class Compare, class Alloc>
    multiset(std::initializer_list<Key>, Compare, Alloc)
        -> multiset<Key, Compare>;


    //template <container-compatible-range<Key> R, class Compare, class Container, class Alloc>
    //multiset(from_range_t, R&&, Compare, Container, Alloc)
    //    -> multiset<Key, Compare, Container>;

    //template <container-compatible-range<Key> R, class Alloc>
    //multiset(from_range_t, R&&, Alloc)
    //    -> multiset<Key, red_black_tree<std::pair<const Key, _dummy>, std::less<Key>>>;
}

namespace j {
    template <class Key, class Compare, class Container>
    multiset<Key, Compare, Container>::multiset(const Compare& comp, const Container& cont)
        : _container(cont), _comp(comp) {}

    template <class Key, class Compare, class Container>
    multiset<Key, Compare, Container>::multiset(const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multiset<Key, Compare, Container>::multiset(InputIter first, InputIter last, const Compare& comp)
        : multiset(first, last, comp, Container()) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multiset<Key, Compare, Container>::
    multiset(InputIter first, InputIter last, const Compare& comp, const Container& cont)
        : _container(cont(first, last)), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multiset<Key, Compare, Container>::multiset(InputIter first, InputIter last, const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {
        _container.clear();
        _container.insert(first, last);
    }

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::multiset(const Compare& comp, const Alloc& alloc)
        : multiset(comp, Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::multiset(const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont, alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::
        value_type; }
    multiset<Key, Compare, Container>::multiset(const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::multiset(const multiset& other, const Alloc& alloc)
        : _container(other, alloc) {}

    template <class Key, class Compare, class Container>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::multiset(multiset&& other, const Alloc& alloc)
        : _container(std::move(other), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::multiset(InputIter first, InputIter last, const Alloc& alloc)
        : multiset(first, last, Compare(), Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::
    multiset(InputIter first, InputIter last, const Compare& comp, const Alloc& alloc)
        : multiset(first, last, comp, Container(), alloc) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::
    multiset(InputIter first, InputIter last, const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont(first, last), alloc), _comp(comp) {}

    template <class Key, class Compare, class Container>
    template <class InputIter, class Alloc> requires (!std::is_integral_v<InputIter>) &&
    std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multiset<Key, Compare, Container>::
    multiset(InputIter first, InputIter last, const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {
        _container.clear();
        _container.insert(first, last);
    }
}