/*
 * @ Created by jaehyung409 on 25. 7. 29.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <random>
#include <vector>
import j.list;

TEST_CASE("List Basic") {
    j::list<int> lst = {1, 2, 3, 4, 5};
    j::list char_lst = {'a', 'b', 'c'};

    SECTION("Constructors") {
        // Default constructor
        j::list<int> default_lst;
        REQUIRE(default_lst.empty());
        REQUIRE(default_lst.size() == 0);

        // Size constructor
        j::list<int> size_lst(5);
        REQUIRE(size_lst.size() == 5);
        for (auto& val : size_lst) {
            REQUIRE(val == 0);
        }

        // Size + value constructor
        j::list<int> size_val_lst(3, 42);
        REQUIRE(size_val_lst.size() == 3);
        for (auto& val : size_val_lst) {
            REQUIRE(val == 42);
        }

        // Iterator constructor
        std::vector<int> vec = {10, 20, 30};
        j::list<int> iter_lst(vec.begin(), vec.end());
        REQUIRE(iter_lst.size() == 3);
        auto it = iter_lst.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);

        // Copy constructor
        j::list<int> copy_lst(lst);
        REQUIRE(copy_lst.size() == lst.size());
        auto it1 = lst.begin();
        auto it2 = copy_lst.begin();
        while (it1 != lst.end() && it2 != copy_lst.end()) {
            REQUIRE(*it1++ == *it2++);
        }

        // Move constructor
        j::list<int> move_source = {100, 200, 300};
        size_t original_size = move_source.size();
        j::list<int> move_lst(std::move(move_source));
        REQUIRE(move_lst.size() == original_size);
        REQUIRE(move_source.size() == 0);
        auto move_it = move_lst.begin();
        REQUIRE(*move_it++ == 100);
        REQUIRE(*move_it++ == 200);
        REQUIRE(*move_it++ == 300);

        // Initializer list constructor
        j::list<int> init_lst = {7, 8, 9};
        REQUIRE(init_lst.size() == 3);
        auto init_it = init_lst.begin();
        REQUIRE(*init_it++ == 7);
        REQUIRE(*init_it++ == 8);
        REQUIRE(*init_it++ == 9);
    }

    SECTION("Assignment operators") {
        // Copy assignment
        j::list<int> assign_lst;
        assign_lst = lst;
        REQUIRE(assign_lst.size() == lst.size());
        auto it1 = lst.begin();
        auto it2 = assign_lst.begin();
        while (it1 != lst.end() && it2 != assign_lst.end()) {
            REQUIRE(*it1++ == *it2++);
        }

        // Move assignment
        j::list<int> move_source = {10, 20, 30};
        j::list<int> move_assign_lst;
        move_assign_lst = std::move(move_source);
        REQUIRE(move_assign_lst.size() == 3);
        REQUIRE(move_source.size() == 0);

        // Initializer list assignment
        j::list<int> init_assign_lst;
        init_assign_lst = {11, 22, 33};
        REQUIRE(init_assign_lst.size() == 3);
        auto it = init_assign_lst.begin();
        REQUIRE(*it++ == 11);
        REQUIRE(*it++ == 22);
        REQUIRE(*it++ == 33);
    }

    SECTION("Assign methods") {
        j::list<int> assign_lst;

        // assign with count and value
        assign_lst.assign(4, 99);
        REQUIRE(assign_lst.size() == 4);
        for (auto& val : assign_lst) {
            REQUIRE(val == 99);
        }

        // assign with iterators
        std::vector<int> vec = {5, 6, 7};
        assign_lst.assign(vec.begin(), vec.end());
        REQUIRE(assign_lst.size() == 3);
        auto it = assign_lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);
        REQUIRE(*it++ == 7);

        // assign with initializer list
        assign_lst.assign({15, 25, 35});
        REQUIRE(assign_lst.size() == 3);
        it = assign_lst.begin();
        REQUIRE(*it++ == 15);
        REQUIRE(*it++ == 25);
        REQUIRE(*it++ == 35);
    }

    SECTION("Iterators") {
        auto it = lst.begin();
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
        --it;
        REQUIRE(*it == 1);

        auto rit = lst.rbegin();
        REQUIRE(*rit == 5);
        ++rit;
        REQUIRE(*rit == 4);
        --rit;
        REQUIRE(*rit == 5);

        auto const_it = lst.cbegin();
        REQUIRE(*const_it == 1);
        ++const_it;
        REQUIRE(*const_it == 2);

        // Test iterator distance
        REQUIRE(std::distance(lst.begin(), lst.end()) == 5);

        // Test const iterators
        const j::list<int>& const_lst = lst;
        auto const_begin = const_lst.begin();
        auto const_end = const_lst.end();
        REQUIRE(std::distance(const_begin, const_end) == 5);
    }

    SECTION("Capacity") {
        j::list<int> empty_lst;
        REQUIRE(lst.size() == 5);
        REQUIRE(!lst.empty());
        REQUIRE(empty_lst.size() == 0);
        REQUIRE(empty_lst.empty());

        // max_size should be reasonable
        REQUIRE(lst.max_size() > 0);
    }

    SECTION("Element access") {
        REQUIRE(lst.front() == 1);
        REQUIRE(lst.back() == 5);
        REQUIRE(char_lst.front() == 'a');
        REQUIRE(char_lst.back() == 'c');

        // Test const versions
        const auto& const_lst = lst;
        REQUIRE(const_lst.front() == 1);
        REQUIRE(const_lst.back() == 5);
    }

    SECTION("Resize") {
        j::list<int> resize_lst = {1, 2, 3};

        // Resize smaller
        resize_lst.resize(2);
        REQUIRE(resize_lst.size() == 2);
        auto it = resize_lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);

        // Resize larger
        resize_lst.resize(4);
        REQUIRE(resize_lst.size() == 4);
        it = resize_lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 0);
        REQUIRE(*it++ == 0);

        // Resize with value
        resize_lst.resize(6, 42);
        REQUIRE(resize_lst.size() == 6);
        it = resize_lst.begin();
        std::advance(it, 4);
        REQUIRE(*it++ == 42);
        REQUIRE(*it++ == 42);
    }
}

TEST_CASE("List Modifiers") {
    j::list<int> lst;

    SECTION("Push/Pop front") {
        lst.push_front(10);
        lst.push_front(20);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 20);
        REQUIRE(lst.back() == 10);

        lst.pop_front();
        REQUIRE(lst.size() == 1);
        REQUIRE(lst.front() == 10);
        REQUIRE(lst.back() == 10);
    }

    SECTION("Push/Pop back") {
        lst.push_back(30);
        lst.push_back(40);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 30);
        REQUIRE(lst.back() == 40);

        lst.pop_back();
        REQUIRE(lst.size() == 1);
        REQUIRE(lst.front() == 30);
        REQUIRE(lst.back() == 30);
    }

    SECTION("Emplace front/back") {
        lst.emplace_front(100);
        lst.emplace_back(200);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 100);
        REQUIRE(lst.back() == 200);
    }

    SECTION("Insert") {
        lst = {1, 2, 4, 5};
        auto it = lst.begin();
        std::advance(it, 2);

        // Insert single element
        auto insert_it = lst.insert(it, 3);
        REQUIRE(*insert_it == 3);
        REQUIRE(lst.size() == 5);

        // Check order
        auto check_it = lst.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);

        // Insert multiple elements
        it = lst.begin();
        std::advance(it, 1);
        lst.insert(it, 3, 10);
        REQUIRE(lst.size() == 8);

        // Insert with iterators
        std::vector<int> vec = {20, 21};
        it = lst.end();
        lst.insert(it, vec.begin(), vec.end());
        REQUIRE(lst.size() == 10);

        // Insert with initializer list
        it = lst.begin();
        lst.insert(it, {-1, -2});
        REQUIRE(lst.size() == 12);
    }

    SECTION("Emplace") {
        lst = {1, 3};
        auto it = lst.begin();
        ++it;
        auto emplace_it = lst.emplace(it, 2);
        REQUIRE(*emplace_it == 2);
        REQUIRE(lst.size() == 3);

        auto check_it = lst.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
    }

    SECTION("Erase") {
        lst = {1, 2, 3, 4, 5};

        // Erase single element
        auto it = lst.begin();
        std::advance(it, 2);
        auto erase_it = lst.erase(it);
        REQUIRE(*erase_it == 4);
        REQUIRE(lst.size() == 4);

        // Erase range
        it = lst.begin();
        auto end_it = lst.begin();
        std::advance(end_it, 2);
        auto range_erase_it = lst.erase(it, end_it);
        REQUIRE(lst.size() == 2);
        REQUIRE(*range_erase_it == 4);

        // Erase all elements one by one
        j::list<int> erase_all = {10, 20, 30};
        while (!erase_all.empty()) {
            erase_all.erase(erase_all.begin());
        }
        REQUIRE(erase_all.empty());
    }

    SECTION("Clear") {
        lst = {1, 2, 3};
        lst.clear();
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
    }

    SECTION("Swap") {
        j::list<int> lst1 = {1, 2, 3};
        j::list<int> lst2 = {4, 5, 6};

        lst1.swap(lst2);
        REQUIRE(lst1.size() == 3);
        REQUIRE(lst2.size() == 3);
        REQUIRE(lst1.front() == 4);
        REQUIRE(lst2.front() == 1);

        // Test non-member swap
        j::swap(lst1, lst2);
        REQUIRE(lst1.front() == 1);
        REQUIRE(lst2.front() == 4);
    }
}

TEST_CASE("List Operations") {
    SECTION("Splice") {
        j::list<int> lst1 = {1, 2, 3};
        j::list<int> lst2 = {4, 5, 6};

        // Splice entire list
        auto it = lst1.begin();
        ++it;
        lst1.splice(it, lst2);
        REQUIRE(lst1.size() == 6);
        REQUIRE(lst2.empty());

        auto check_it = lst1.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);
        REQUIRE(*check_it++ == 6);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);

        // Splice single element
        lst2 = {10, 20, 30};
        it = lst1.begin();
        auto splice_it = lst2.begin();
        ++splice_it;
        lst1.splice(it, lst2, splice_it);
        REQUIRE(lst1.size() == 7);
        REQUIRE(lst2.size() == 2);
        REQUIRE(lst1.front() == 20);

        // Splice range
        lst2 = {100, 200, 300};
        it = lst1.end();
        lst1.splice(it, lst2, lst2.begin(), lst2.end());
        REQUIRE(lst1.size() == 10);
        REQUIRE(lst2.empty());
    }

    SECTION("Remove") {
        j::list<int> lst = {1, 2, 2, 3, 2, 4};
        size_t removed = lst.remove(2);
        REQUIRE(removed == 3);
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);

        // Remove non-existent element
        size_t removed2 = lst.remove(99);
        REQUIRE(removed2 == 0);
        REQUIRE(lst.size() == 3);
    }

    SECTION("Remove if") {
        j::list<int> lst = {1, 2, 3, 4, 5, 6};
        size_t removed = lst.remove_if([](int n) { return n % 2 == 0; });
        REQUIRE(removed == 3);
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 5);

        // Test with custom predicate
        j::list<int> lst2 = {10, 15, 20, 25, 30};
        size_t removed2 = lst2.remove_if([](int n) { return n > 20; });
        REQUIRE(removed2 == 2);
        REQUIRE(lst2.size() == 3);
    }

    SECTION("Unique") {
        j::list<int> lst = {1, 1, 2, 2, 2, 3, 3};
        size_t removed = lst.unique();
        REQUIRE(removed == 4);
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);

        // Test with custom predicate
        j::list<int> lst2 = {1, 5, 2, 6, 3, 5};
        size_t removed2 = lst2.unique([](int a, int b) { return (a % 2) == (b % 2); });
        REQUIRE(removed2 == 3);
        REQUIRE(lst2.size() == 3);
    }

    SECTION("Merge") {
        j::list<int> lst1 = {1, 3, 5};
        j::list<int> lst2 = {2, 4, 6};

        lst1.merge(lst2);
        REQUIRE(lst1.size() == 6);
        REQUIRE(lst2.empty());

        auto it = lst1.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);

        // Test merge with custom comparator
        j::list<int> lst3 = {5, 3, 1};
        j::list<int> lst4 = {6, 4, 2};
        lst3.merge(lst4, std::greater<int>());
        REQUIRE(lst3.size() == 6);
        REQUIRE(lst4.empty());
    }

    SECTION("Sort") {
        j::list<int> lst = {5, 2, 8, 1, 9, 3};
        lst.sort();
        REQUIRE(lst.size() == 6);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 8);
        REQUIRE(*it++ == 9);

        // Test sort with custom comparator
        lst = {1, 2, 3, 4, 5};
        lst.sort(std::greater<int>());

        it = lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);

        // Test empty list sort
        j::list<int> empty_lst;
        empty_lst.sort();
        REQUIRE(empty_lst.empty());

        // Test single element sort
        j::list<int> single_lst = {42};
        single_lst.sort();
        REQUIRE(single_lst.size() == 1);
        REQUIRE(single_lst.front() == 42);
    }

    SECTION("Reverse") {
        j::list<int> lst = {1, 2, 3, 4, 5};
        lst.reverse();
        REQUIRE(lst.size() == 5);

        auto it = lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);

        // Test empty list reverse
        j::list<int> empty_lst;
        empty_lst.reverse();
        REQUIRE(empty_lst.empty());

        // Test single element reverse
        j::list<int> single_lst = {42};
        single_lst.reverse();
        REQUIRE(single_lst.size() == 1);
        REQUIRE(single_lst.front() == 42);
    }
}

TEST_CASE("List Edge Cases") {
    SECTION("Empty list operations") {
        j::list<int> empty_lst;

        // Test iterators on empty list
        REQUIRE(empty_lst.begin() == empty_lst.end());
        REQUIRE(empty_lst.cbegin() == empty_lst.cend());
        REQUIRE(empty_lst.rbegin() == empty_lst.rend());

        // Clear empty list
        empty_lst.clear();
        REQUIRE(empty_lst.empty());

        // Resize empty list
        empty_lst.resize(3, 42);
        REQUIRE(empty_lst.size() == 3);
        for (auto& val : empty_lst) {
            REQUIRE(val == 42);
        }
    }

    SECTION("Single element operations") {
        j::list<int> single_lst = {42};

        REQUIRE(single_lst.front() == 42);
        REQUIRE(single_lst.back() == 42);
        REQUIRE(single_lst.size() == 1);

        single_lst.reverse();
        REQUIRE(single_lst.front() == 42);

        single_lst.sort();
        REQUIRE(single_lst.front() == 42);

        single_lst.unique();
        REQUIRE(single_lst.size() == 1);
    }
}

TEST_CASE("List Operations on Large Dataset") {
    const size_t data_size = 1000;

    SECTION("Push and Pop") {
        j::list<int> lst;
        for (size_t i = 0; i < data_size; ++i) {
            lst.push_back(static_cast<int>(i));
        }
        REQUIRE(lst.size() == data_size);

        for (size_t i = 0; i < data_size; ++i) {
            REQUIRE(lst.front() == static_cast<int>(i));
            lst.pop_front();
        }
        REQUIRE(lst.empty());
    }

    SECTION("Erase and Insert") {
        j::list<int> lst;
        for (size_t i = 0; i < data_size; ++i) {
            lst.push_back(static_cast<int>(i));
        }
        REQUIRE(lst.size() == data_size);

        // Erase middle element
        auto it = lst.begin();
        std::advance(it, data_size / 2);
        int value_before = *std::prev(it);
        int value_after = *std::next(it);
        it = lst.erase(it);
        REQUIRE(lst.size() == data_size - 1);
        REQUIRE(*it == value_after);
        REQUIRE(*std::prev(it) == value_before);

        // Insert middle element
        it = lst.insert(it, 9999);
        REQUIRE(lst.size() == data_size);
        REQUIRE(*it == 9999);
        REQUIRE(*std::prev(it) == value_before);
        REQUIRE(*std::next(it) == value_after);
    }

    SECTION("Remove and Remove_if") {
        j::list<int> lst;
        for (size_t i = 0; i < data_size; ++i) {
            lst.push_back(static_cast<int>(i % 10)); // 0-9 repeated 100 times
        }

        // Remove value '5'
        size_t removed_count = lst.remove(5);
        REQUIRE(removed_count == 100);
        REQUIRE(lst.size() == data_size - 100);
        for (int val : lst) {
            REQUIRE(val != 5);
        }

        // Remove even numbers
        removed_count = lst.remove_if([](int n) { return n % 2 == 0; });
        REQUIRE(removed_count == 500);
        REQUIRE(lst.size() == 400);
        for (int val : lst) {
            REQUIRE(val % 2 != 0);
        }
    }

    SECTION("Sort and Merge") {
        j::list<int> lst1;
        j::list<int> lst2;
        std::vector<int> vec1;
        std::vector<int> vec2;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9999);

        for (size_t i = 0; i < data_size / 2; ++i) {
            int val1 = dis(gen);
            int val2 = dis(gen);
            lst1.push_back(val1);
            lst2.push_back(val2);
            vec1.push_back(val1);
            vec2.push_back(val2);
        }

        // Sort
        lst1.sort();
        std::sort(vec1.begin(), vec1.end());
        REQUIRE(std::equal(lst1.begin(), lst1.end(), vec1.begin()));

        lst2.sort();
        std::sort(vec2.begin(), vec2.end());
        REQUIRE(std::equal(lst2.begin(), lst2.end(), vec2.begin()));

        // Merge
        lst1.merge(lst2);
        vec1.insert(vec1.end(), vec2.begin(), vec2.end());
        std::sort(vec1.begin(), vec1.end());

        REQUIRE(lst1.size() == data_size);
        REQUIRE(lst2.empty());
        REQUIRE(std::equal(lst1.begin(), lst1.end(), vec1.begin()));
    }

    SECTION("Unique") {
        j::list<int> lst;
        std::vector<int> vec;
        for (size_t i = 0; i < data_size / 5; ++i) {
            for(int j=0; j<5; ++j) {
                lst.push_back(static_cast<int>(i));
            }
        }

        size_t removed_count = lst.unique();
        REQUIRE(removed_count == data_size - (data_size / 5));
        REQUIRE(lst.size() == data_size / 5);

        int expected_val = 0;
        for(int val : lst) {
            REQUIRE(val == expected_val++);
        }
    }

    SECTION("Splice") {
        j::list<int> lst1, lst2;
        for(int i=0; i<500; ++i) lst1.push_back(i);
        for(int i=500; i<1000; ++i) lst2.push_back(i);

        auto it = lst1.begin();
        std::advance(it, 250);

        lst1.splice(it, lst2);

        REQUIRE(lst1.size() == 1000);
        REQUIRE(lst2.empty());

        auto check_it = lst1.begin();
        for(int i=0; i<250; ++i) {
            REQUIRE(*check_it++ == i);
        }
        for(int i=500; i<1000; ++i) {
            REQUIRE(*check_it++ == i);
        }
        for(int i=250; i<500; ++i) {
            REQUIRE(*check_it++ == i);
        }
    }

    SECTION("Reverse") {
        j::list<int> lst;
        std::vector<int> vec;
        for(size_t i=0; i<data_size; ++i) {
            lst.push_back(static_cast<int>(i));
            vec.push_back(static_cast<int>(i));
        }

        lst.reverse();
        REQUIRE(std::equal(lst.begin(), lst.end(), vec.rbegin()));
        std::reverse(vec.begin(), vec.end());

        REQUIRE(lst.size() == data_size);
        REQUIRE(std::equal(lst.begin(), lst.end(), vec.begin()));
    }
}