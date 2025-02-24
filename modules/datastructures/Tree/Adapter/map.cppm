/*
 * @ Created by jaehyung409 on 25. 2. 12.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */
 
module;
#include <functional>
#include <initializer_list>

export module j.map;

import j.red_black_tree;
import j.avl_tree;

namespace j {
    export template <class Key, class Value, class Compare, class Container>
    class multimap;
}

namespace j {
    export template <class Key,
                     class Value,
                     class Compare = std::less<Key>,
                     class Container = red_black_tree<std::pair<const Key, Value>, Compare>>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    class map {
    public:
        using key_type                  = typename Container::key_type;
        using key_compare               = typename Container::key_compare;
        using value_type                = typename Container::value_type;
        using value_compare             = typename Container::value_compare;
        using mapped_type               = typename Container::mapped_type;
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
        map() : map(Compare()) {}
        explicit map(const Compare &comp) : map(comp, Container()) {}
        map(const Compare& comp, const Container &cont);
        map(const Compare& comp, Container &&cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        map(InputIter first, InputIter last, const Compare &comp = Compare());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        map(InputIter first, InputIter last, const Compare &comp, const Container &cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        map(InputIter first, InputIter last, const Compare &comp, Container &&cont);
        //template <container-compatible-range<Key> R>
        //map(from_range_t, R&& range, const Compare &comp = Compare());
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        map(const Compare &comp, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        map(const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        map(const Compare &comp, Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        map(const map &other, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        map(map &&other, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        map(InputIter first, InputIter last, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        map(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        map(InputIter first, InputIter last, const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        map(InputIter first, InputIter last, const Compare &comp, Container &&cont, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //map(from_range_t, R&& range, const Alloc &alloc);
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

        // element access
        mapped_type& operator[](const key_type& x) {
            auto it = this->insert({x, mapped_type()}).first;
            return it->_ptr->mapped();
        }
        mapped_type& operator[](key_type&& x) { return (*this)[std::move(x)]; }
        template <class K> requires std::convertible_to<K, key_type>
        mapped_type& operator[](K&& x) { return (*this)[std::forward<K>(x)]; }
        mapped_type& at(const key_type& x) {
            auto it = this->find(x);
            if (it == this->end()) {
                throw std::out_of_range("map::at(): key not found");
            }
            return it->_ptr->mapped();
        }
        const mapped_type& at(const key_type& x) const {
            auto it = this->find(x);
            if (it == this->end()) {
                throw std::out_of_range("map::at(): key not found");
            }
            return it->_ptr->mapped();
        }
        template <class K> requires std::convertible_to<K, key_type>
        mapped_type& at(const K& x) { return this->at(static_cast<key_type>(x)); }
        template <class K> requires std::convertible_to<K, key_type>
        const mapped_type& at(const K& x) const { return this->at(static_cast<key_type>(x)); }

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
        template <class K> requires std::convertible_to<K, value_type>
        std::pair<iterator, bool> insert(K &&x) { return _container.insert_unique(std::forward<K>(x)).first; }
        iterator insert(const_iterator hint, const value_type &x) { return _container.insert_unique(hint, x); }
        iterator insert(const_iterator hint, value_type &&x) {
            return _container.insert_unique(hint, std::move(x));
        }
        template <class K> requires std::convertible_to<K, value_type>
        iterator insert(const_iterator hint, K &&x) { return _container.insert_unique(hint, std::forward<K>(x)); }
        template <class InputIter> requires (!std::is_integral_v<InputIter>)
        void insert(InputIter first, InputIter last) { return _container.insert_unique(first, last); }
        // template <container-compatible-range<Key> R>
        // void insert_range(R &&rg) { return _container.insert_unique(rg); }
        void insert(std::initializer_list<value_type> il) { return _container.insert_unique(il); }

        node_type extract(const_iterator position) { return _container.extract(position); }
        node_type extract(const key_type &key) { return _container.extract(key); }
        template <class K> requires std::convertible_to<K, key_type>
        node_type extract(K &&key) { return _container.extract(std::forward<K>(key)); }
        insert_return_type insert(node_type &&node) { return _container.insert_unique(std::move(node)); }
        iterator insert(const_iterator hint, node_type &&node) {
            return _container.insert_unique(hint, std::move(node));
        }
        // modifiers
        template<class... Args>
        std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args) {
            auto it = this->find(k);
            if (it == this->end()) {
                return this->emplace(std::piecewise_construct,
                                     std::forward_as_tuple(k),
                                     std::forward_as_tuple(std::forward<Args>(args)...));

            }
            return {it, false};
        }
        template<class... Args>
        std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
            return this->try_emplace(std::move(k), std::forward<Args>(args)...);
        }
        template<class K, class... Args> requires std::convertible_to<K, key_type>
        std::pair<iterator, bool> try_emplace(K&& k, Args&&... args) {
            return this->try_emplace(std::forward<K>(k), std::forward<Args>(args)...);
        }
        template<class... Args>
        iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args) {
            auto it = this->find(k);
            if (it == this->end()) {
                return this->emplace_hint(hint, std::piecewise_construct,
                                                std::forward_as_tuple(k),
                                                std::forward_as_tuple(std::forward<Args>(args)...));
            }
            return {it, false};
        }
        template<class... Args>
        iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args) {
            return this->try_emplace(hint, std::move(k), std::forward<Args>(args)...);
        }
        template<class K, class... Args> requires std::convertible_to<K, key_type>
        iterator try_emplace(const_iterator hint, K&& k, Args&&... args) {
            return this->try_emplace(hint, std::forward<K>(k), std::forward<Args>(args)...);
        }
        template<class M> requires std::assignable_from<mapped_type&, M>
        std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) {
            auto [it, inserted] = this->insert({k, mapped_type()});
            it._ptr->mapped() = std::forward<M>(obj);
            return {it, inserted};
        }
        template<class M> requires std::assignable_from<mapped_type&, M>
        std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) {
            return insert_or_assign(std::move(k), std::forward<M>(obj));
        }
        template<class K, class M> requires std::convertible_to<K, key_type> && std::assignable_from<mapped_type&, M>
        std::pair<iterator, bool> insert_or_assign(K&& k, M&& obj) {
            return insert_or_assign(std::forward<K>(k), std::forward<M>(obj));
        }
        template<class M> requires std::assignable_from<mapped_type&, M>
        iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj) {
            auto it = this->insert(hint, {k, mapped_type()});
            it._ptr->mapped() = std::forward<M>(obj);
            return it;
        }
        template<class M> requires std::assignable_from<mapped_type&, M>
        iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj) {
            return insert_or_assign(hint, std::move(k), std::forward<M>(obj));
        }
        template<class K, class M> requires std::convertible_to<K, key_type> && std::assignable_from<mapped_type&, M>
        iterator insert_or_assign(const_iterator hint, K&& k, M&& obj) {
            return insert_or_assign(hint, std::forward<K>(k), std::forward<M>(obj));
        }

        iterator erase(iterator pos) requires std::is_same_v<iterator, const_iterator> { return _container.erase(pos); }
        iterator erase(const_iterator pos) { return _container.erase(pos); }
        size_type erase(const key_type &key) { return _container.erase(key); }
        template <class K> requires std::convertible_to<K, key_type>
        size_type erase(K &&key) { return _container.erase(std::forward<K>(key)); }
        iterator erase(const_iterator first, const_iterator last) { return _container.erase(first, last); }
        void swap(map &other)
            noexcept(std::is_nothrow_swappable_v<Container> && std::is_nothrow_swappable_v<Compare>) {
            using std::swap;
            swap(_container, other._container);
            swap(_comp, other._comp);
        }

        void clear() noexcept { _container.clear(); }

        template <class C2>
        void merge(map<key_type, value_type, C2, allocator_type> &source) {
            _container.unique_merge(source._container);
        }
        template <class C2>
        void merge(map<key_type, value_type, C2, allocator_type> &&source) {
            _container.unique_merge(std::move(source._container));
        }
        template <class C2>
        void merge(multimap<key_type, value_type, C2, allocator_type> &source) {
            _container.unique_merge(source._container);
        }
        template <class C2>
        void merge(multimap<key_type, value_type, C2, allocator_type> &&source) {
            _container.unique_merge(std::move(source._container));
        }

        key_compare key_comp() const { return _container.key_comp(); }
        value_compare value_comp() const { return _container.value_comp(); }

        iterator find(const key_type &key) { return _container.find(key); }
        const_iterator find(const key_type &key) const { return _container.find(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator find(K &&key) { return _container.find(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator find(K &&key) const { return _container.find(std::forward<K>(key)); }

        size_type count(const key_type &key) const { return _container.count(key); }
        template <class K> requires std::convertible_to<K, key_type>
        size_type count(K &&key) const { return _container.count(std::forward<K>(key)); }

        bool contains(const key_type &key) const { return _container.contains(key); }
        template <class K> requires std::convertible_to<K, key_type>
        bool contains(K &&key) const { return _container.contains(std::forward<K>(key)); }

        iterator lower_bound(const key_type &key) { return _container.lower_bound(key); }
        const_iterator lower_bound(const key_type &key) const { return _container.lower_bound(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator lower_bound(K &&key) { return _container.lower_bound(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator lower_bound(K &&key) const { return _container.lower_bound(std::forward<K>(key)); }

        iterator upper_bound(const key_type &key) { return _container.upper_bound(key); }
        const_iterator upper_bound(const key_type &key) const { return _container.upper_bound(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator upper_bound(K &&key) { return _container.upper_bound(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator upper_bound(K &&key) const { return _container.upper_bound(std::forward<K>(key)); }

        std::pair<iterator, iterator> equal_range(const key_type &key) { return _container.equal_range(key); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
            return _container.equal_range(key);
        }
        template <class K> requires std::convertible_to<K, key_type>
        std::pair<iterator, iterator> equal_range(K &&key) { return _container.equal_range(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        std::pair<const_iterator, const_iterator> equal_range(K &&key) const {
            return _container.equal_range(std::forward<K>(key));
        }
    };

    template <class Compare, class Container>
    map(Compare, Container)
        -> map<std::remove_const_t<typename Container::key_type>, typename Container::mapped_type, Compare, Container>;

    template <class InputIter,
              class Key        = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value      = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare    = std::less<Key>,
              class Container  = red_black_tree<std::pair<const Key, Value>, Compare>>
    map(InputIter, InputIter, Compare = Compare(), Container = Container())
        -> map<Key, Value, Compare, Container>;

    //template <container-compatible-range<Key> R,
    //          class Key       = std::remove_const_t<typename std::tuple_element<0, Pair>::type>,
    //          class Value     = typename std::tuple_element<1, Pair>::type,
    //          class Compare   = std::less<Key>,
    //          class Container = red_black_tree<std::pair<const Key, Value>, Compare>>
    //map(from_range_t, R&&, Compare = Compare(), Container = Container())
    //    -> map<Key, Value, Compare, Container>;

    template <class Compare, class Container, class Alloc>
    map(Compare, Container, Alloc)
        -> map<std::remove_const_t<typename Container::key_type>, typename Container::mapped_type, Compare, Container>;

    template <class InputIter,
              class Key       = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value     = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare   = std::less<Key>,
              class Alloc,
              class Container = red_black_tree<std::pair<const Key, Value>, Compare, Alloc>>
    map(InputIter, InputIter, Alloc)
        -> map<Key, Value, Compare, Container>;

    template <class InputIter,
              class Key       = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value     = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare,
              class Alloc,
              class Container = red_black_tree<std::pair<const Key, Value>, Compare, Alloc>>
    map(InputIter, InputIter, Compare, Alloc)
        -> map<Key, Value, Compare, Container>;

    template <class InputIter, class Compare, class Container, class Alloc>
    map(InputIter, InputIter, Compare, Container, Alloc)
        -> map<typename std::iterator_traits<InputIter>::value_type::first_type,
               typename std::iterator_traits<InputIter>::value_type::second_type,
               Container, Compare>;

    template <class Key, class Compare = std::less<Key>, class Container = red_black_tree<Key>>
    map(std::initializer_list<Key>, Compare = Compare(), Container = Container())
        -> map<typename Key::first_type, typename Key::second_type, Container, Compare>;

    template <class Key, class Compare, class Alloc>
    map(std::initializer_list<Key>, Compare, Alloc)
        -> map<typename Key::first_type, typename Key::second_type, red_black_tree<Key>, Compare>;


    //template <container-compatible-range<Key> R, class Compare, class Container, class Alloc>
    //map(from_range_t, R&&, Compare, Container, Alloc)
    //    -> map<typename Key::first_type, typename Key::second_type, Container, Compare>;

    //template <container-compatible-range<Key> R, class Alloc>
    //map(from_range_t, R&&, Alloc)
    //    -> map<typename Key::first_type, typename Key::second_type, red_black_tree<Key>, std::less<Key>>;

}

namespace j {
    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    map<Key, Value, Compare, Container>::map(const Compare& comp, const Container& cont)
        : _container(cont), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    map<Key, Value, Compare, Container>::map(const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    map<Key, Value, Compare, Container>::map(InputIter first, InputIter last, const Compare& comp)
        : map(first, last, comp, Container()) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    map<Key, Value, Compare, Container>::
    map(InputIter first, InputIter last, const Compare& comp, const Container& cont)
        : _container(cont(first, last)), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    map<Key, Value, Compare, Container>::map(InputIter first, InputIter last, const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {
        _container.clear();
        _container.insert_unique(first, last);
    }

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(const Compare& comp, const Alloc& alloc) : map(comp, Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont, alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(const map& other, const Alloc& alloc)
        : _container(other, alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(map&& other, const Alloc& alloc)
        : _container(std::move(other), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(InputIter first, InputIter last, const Alloc& alloc)
        : map(first, last, Compare(), Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::map(InputIter first, InputIter last, const Compare& comp, const Alloc& alloc)
        : map(first, last, comp, Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::
    map(InputIter first, InputIter last, const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont(first, last), alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    map<Key, Value, Compare, Container>::
    map(InputIter first, InputIter last, const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {
        _container.clear();
        _container.insert_unique(first, last);
    }
}

namespace j {
    export template <class Key,
                     class Value,
                     class Compare = std::less<Key>,
                     class Container = red_black_tree<std::pair<const Key, Value>, Compare>>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    class multimap {
    public:
        using key_type                  = typename Container::key_type;
        using key_compare               = typename Container::key_compare;
        using value_type                = typename Container::value_type;
        using value_compare             = typename Container::value_compare;
        using mapped_type               = typename Container::mapped_type;
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

    protected:
        Container _container;
        Compare _comp;

    public:
        multimap() : multimap(Compare()) {}
        explicit multimap(const Compare &comp) : multimap(comp, Container()) {}
        multimap(const Compare& comp, const Container &cont);
        multimap(const Compare& comp, Container &&cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multimap(InputIter first, InputIter last, const Compare &comp = Compare());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multimap(InputIter first, InputIter last, const Compare &comp, const Container &cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        multimap(InputIter first, InputIter last, const Compare &comp, Container &&cont);
        //template <container-compatible-range<Key> R>
        //multimap(from_range_t, R&& range, const Compare &comp = Compare());
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(const Compare &comp, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(const Compare &comp, Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(const multimap &other, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(multimap &&other, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(InputIter first, InputIter last, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(InputIter first, InputIter last, const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        multimap(InputIter first, InputIter last, const Compare &comp, Container &&cont, const Alloc &alloc);
        //template <container-compatible-range<Key> R, class Alloc>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //multimap(from_range_t, R&& range, const Alloc &alloc);
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
        template <class K> requires std::convertible_to<K, value_type>
        std::pair<iterator, bool> insert(K &&x) { return _container.insert(std::forward<K>(x)).first; }
        iterator insert(const_iterator hint, const value_type &x) { return _container.insert(hint, x); }
        iterator insert(const_iterator hint, value_type &&x) {
            return _container.insert(hint, std::move(x));
        }
        template <class K> requires std::convertible_to<K, value_type>
        iterator insert(const_iterator hint, K &&x) { return _container.insert(hint, std::forward<K>(x)); }
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        void insert(InputIter first, InputIter last) { return _container.insert(first, last); }
        // template <container-compatible-range<Key> R>
        // void insert_range(R &&rg) { return _container.insert(rg); }
        void insert(std::initializer_list<value_type> il) { return _container.insert(il); }

        node_type extract(const_iterator position) { return _container.extract(position); }
        node_type extract(const key_type &key) { return _container.extract(key); }
        template <class K> requires std::convertible_to<K, key_type>
        node_type extract(K &&key) { return _container.extract(std::forward<K>(key)); }
        iterator insert(node_type &&node) { return _container.insert(std::move(node)); }
        iterator insert(const_iterator hint, node_type &&node) {
            return _container.insert(hint, std::move(node));
        }
        iterator erase(iterator pos)
        requires std::is_same_v<iterator, const_iterator> { return _container.erase(pos); }
        iterator erase(const_iterator pos) { return _container.erase(pos); }
        size_type erase(const key_type &key) { return _container.erase(key); }
        template <class K> requires std::convertible_to<K, key_type>
        size_type erase(K &&key) { return _container.erase(std::forward<K>(key)); }
        iterator erase(const_iterator first, const_iterator last) { return _container.erase(first, last); }
        void swap(multimap &other) noexcept(
            std::is_nothrow_swappable_v<Container> &&
            std::is_nothrow_swappable_v<Compare>
        ) {
            using std::swap;
            swap(_container, other._container);
            swap(_comp, other._comp);
        }

        void clear() noexcept { _container.clear(); }

        template <class C2>
        void merge(multimap<key_type, value_type, C2, allocator_type> &source) { _container.merge(source._container); }
        template <class C2>
        void merge(multimap<key_type, value_type, C2, allocator_type> &&source) {
            _container.merge(std::move(source._container));
        }
        template <class C2>
        void merge(map<key_type, value_type, C2, allocator_type> &source) {
            _container.merge(source._container);
        }
        template <class C2>
        void merge(map<key_type, value_type, C2, allocator_type> &&source) {
            _container.merge(std::move(source._container));
        }

        key_compare key_comp() const { return _container.key_comp(); }
        value_compare value_comp() const { return _container.value_comp(); }

        iterator find(const key_type &key) { return _container.find(key); }
        const_iterator find(const key_type &key) const { return _container.find(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator find(K &&key) { return _container.find(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator find(K &&key) const { return _container.find(std::forward<K>(key)); }

        size_type count(const key_type &key) const { return _container.count(key); }
        template <class K> requires std::convertible_to<K, key_type>
        size_type count(K &&key) const { return _container.count(std::forward<K>(key)); }

        bool contains(const key_type &key) const { return _container.contains(key); }
        template <class K> requires std::convertible_to<K, key_type>
        bool contains(K &&key) const { return _container.contains(std::forward<K>(key)); }

        iterator lower_bound(const key_type &key) { return _container.lower_bound(key); }
        const_iterator lower_bound(const key_type &key) const { return _container.lower_bound(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator lower_bound(K &&key) { return _container.lower_bound(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator lower_bound(K &&key) const { return _container.lower_bound(std::forward<K>(key)); }

        iterator upper_bound(const key_type &key) { return _container.upper_bound(key); }
        const_iterator upper_bound(const key_type &key) const { return _container.upper_bound(key); }
        template <class K> requires std::convertible_to<K, key_type>
        iterator upper_bound(K &&key) { return _container.upper_bound(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        const_iterator upper_bound(K &&key) const { return _container.upper_bound(std::forward<K>(key)); }

        std::pair<iterator, iterator> equal_range(const key_type &key) { return _container.equal_range(key); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
            return _container.equal_range(key);
        }
        template <class K> requires std::convertible_to<K, key_type>
        std::pair<iterator, iterator> equal_range(K &&key) { return _container.equal_range(std::forward<K>(key)); }
        template <class K> requires std::convertible_to<K, key_type>
        std::pair<const_iterator, const_iterator> equal_range(K &&key) const {
            return _container.equal_range(std::forward<K>(key));
        }
    };

    template <class Compare, class Container>
    multimap(Compare, Container)
        -> multimap<std::remove_const_t<typename Container::key_type>,
                    typename Container::mapped_type,
                    Compare,
                    Container>;

    template <class InputIter,
              class Key        = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value      = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare    = std::less<Key>,
              class Container  = red_black_tree<std::pair<const Key, Value>, Compare>>
    multimap(InputIter, InputIter, Compare = Compare(), Container = Container())
        -> multimap<Key, Value, Compare, Container>;

    //template <container-compatible-range<Key> R,
    //          class Key       = std::remove_const_t<typename std::tuple_element<0, Pair>::type>,
    //          class Value     = typename std::tuple_element<1, Pair>::type,
    //          class Compare   = std::less<Key>,
    //          class Container = red_black_tree<std::pair<const Key, Value>, Compare>>
    //multimap(from_range_t, R&&, Compare = Compare(), Container = Container())
    //    -> multimap<Key, Value, Compare, Container>;

    template <class Compare, class Container, class Alloc>
    multimap(Compare, Container, Alloc)
        -> multimap<std::remove_const_t<typename Container::key_type>,
                    typename Container::mapped_type,
                    Compare,
                    Container>;

    template <class InputIter,
              class Key       = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value     = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare   = std::less<Key>,
              class Alloc,
              class Container = red_black_tree<std::pair<const Key, Value>, Compare, Alloc>>
    multimap(InputIter, InputIter, Alloc)
        -> multimap<Key, Value, Compare, Container>;

    template <class InputIter,
              class Key       = std::remove_const_t<typename std::iterator_traits<InputIter>::value_type::first_type>,
              class Value     = typename std::iterator_traits<InputIter>::value_type::second_type,
              class Compare,
              class Alloc,
              class Container = red_black_tree<std::pair<const Key, Value>, Compare, Alloc>>
    multimap(InputIter, InputIter, Compare, Alloc)
        -> multimap<Key, Value, Compare, Container>;

    template <class InputIter, class Compare, class Container, class Alloc>
    multimap(InputIter, InputIter, Compare, Container, Alloc)
        -> multimap<typename std::iterator_traits<InputIter>::value_type::first_type,
               typename std::iterator_traits<InputIter>::value_type::second_type,
               Container, Compare>;

    template <class Key, class Compare = std::less<Key>, class Container = red_black_tree<Key>>
    multimap(std::initializer_list<Key>, Compare = Compare(), Container = Container())
        -> multimap<typename Key::first_type, typename Key::second_type, Container, Compare>;

    template <class Key, class Compare, class Alloc>
    multimap(std::initializer_list<Key>, Compare, Alloc)
        -> multimap<typename Key::first_type, typename Key::second_type, red_black_tree<Key>, Compare>;


    //template <container-compatible-range<Key> R, class Compare, class Container, class Alloc>
    //multimap(from_range_t, R&&, Compare, Container, Alloc)
    //    -> multimap<typename Key::first_type, typename Key::second_type, Container, Compare>;

    //template <container-compatible-range<Key> R, class Alloc>
    //multimap(from_range_t, R&&, Alloc)
    //    -> multimap<typename Key::first_type, typename Key::second_type, red_black_tree<Key>, std::less<Key>>;

}

namespace j {
    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    multimap<Key, Value, Compare, Container>::multimap(const Compare& comp, const Container& cont)
        : _container(cont), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    multimap<Key, Value, Compare, Container>::multimap(const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multimap<Key, Value, Compare, Container>::multimap(InputIter first, InputIter last, const Compare& comp)
        : multimap(first, last, comp, Container()) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multimap<Key, Value, Compare, Container>::
    multimap(InputIter first, InputIter last, const Compare& comp, const Container& cont)
        : _container(cont(first, last)), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter> requires (!std::is_integral_v<InputIter>)
    multimap<Key, Value, Compare, Container>::
    multimap(InputIter first, InputIter last, const Compare& comp, Container&& cont)
        : _container(std::move(cont)), _comp(comp) {
        _container.clear();
        _container.insert(first, last);
    }

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(const Compare& comp, const Alloc& alloc)
        : multimap(comp, Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc> requires std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont, alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(const multimap& other, const Alloc& alloc)
        : _container(other, alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(multimap&& other, const Alloc& alloc)
        : _container(std::move(other), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::multimap(InputIter first, InputIter last, const Alloc& alloc)
        : multimap(first, last, Compare(), Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::
    multimap(InputIter first, InputIter last, const Compare& comp, const Alloc& alloc)
        : multimap(first, last, comp, Container(), alloc) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::
    multimap(InputIter first, InputIter last, const Compare& comp, const Container& cont, const Alloc& alloc)
        : _container(cont(first, last), alloc), _comp(comp) {}

    template <class Key, class Value, class Compare, class Container>
    requires std::is_same_v<typename Container::value_type, std::pair<const Key, Value>>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
    requires { typename std::allocator_traits<Alloc>::value_type; }
    multimap<Key, Value, Compare, Container>::
    multimap(InputIter first, InputIter last, const Compare& comp, Container&& cont, const Alloc& alloc)
        : _container(std::move(cont), alloc), _comp(comp) {
        _container.clear();
        _container.insert(first, last);
    }
}