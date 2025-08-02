/*
 * @ Created by jaehyung409 on 25. 8. 1.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <random>
#include <vector>
import j.forward_list;

TEST_CASE("Forward List Basic") {
    j::forward_list<int> flist = {1, 2, 3, 4, 5};
    j::forward_list char_flist = {'a', 'b', 'c'};

    SECTION("Constructors") {
        // Default constructor
        j::forward_list<int> default_flist;
        REQUIRE(default_flist.empty());

        // Size constructor
        j::forward_list<int> size_flist(5);
        REQUIRE(!size_flist.empty());
        int count = 0;
        for (auto it = size_flist.begin(); it != size_flist.end(); ++it) {
            REQUIRE(*it == 0);
            ++count;
        }
        REQUIRE(count == 5);

        // Size + value constructor
        j::forward_list<int> size_val_flist(3, 42);
        count = 0;
        for (auto it = size_val_flist.begin(); it != size_val_flist.end(); ++it) {
            REQUIRE(*it == 42);
            ++count;
        }
        REQUIRE(count == 3);

        // Iterator constructor
        std::vector<int> vec = {10, 20, 30};
        j::forward_list<int> iter_flist(vec.begin(), vec.end());
        auto it = iter_flist.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);

        // Copy constructor
        j::forward_list<int> copy_flist(flist);
        auto it1 = flist.begin();
        auto it2 = copy_flist.begin();
        while (it1 != flist.end() && it2 != copy_flist.end()) {
            REQUIRE(*it1++ == *it2++);
        }
        REQUIRE(it1 == flist.end());
        REQUIRE(it2 == copy_flist.end());

        // Move constructor
        j::forward_list<int> move_source = {100, 200, 300};
        j::forward_list<int> move_flist(std::move(move_source));
        REQUIRE(move_source.empty());
        auto move_it = move_flist.begin();
        REQUIRE(*move_it++ == 100);
        REQUIRE(*move_it++ == 200);
        REQUIRE(*move_it++ == 300);

        // Initializer list constructor
        j::forward_list<int> init_flist = {7, 8, 9};
        auto init_it = init_flist.begin();
        REQUIRE(*init_it++ == 7);
        REQUIRE(*init_it++ == 8);
        REQUIRE(*init_it++ == 9);
    }

    SECTION("Assignment operators") {
        // Copy assignment
        j::forward_list<int> assign_flist;
        assign_flist = flist;
        auto it1 = flist.begin();
        auto it2 = assign_flist.begin();
        while (it1 != flist.end() && it2 != assign_flist.end()) {
            REQUIRE(*it1++ == *it2++);
        }

        // Move assignment
        j::forward_list<int> move_source = {10, 20, 30};
        j::forward_list<int> move_assign_flist;
        move_assign_flist = std::move(move_source);
        REQUIRE(move_source.empty());
        auto it = move_assign_flist.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);

        // Initializer list assignment
        j::forward_list<int> init_assign_flist;
        init_assign_flist = {11, 22, 33};
        it = init_assign_flist.begin();
        REQUIRE(*it++ == 11);
        REQUIRE(*it++ == 22);
        REQUIRE(*it++ == 33);
    }

    SECTION("Assign methods") {
        j::forward_list<int> assign_flist;

        // assign with count and value
        assign_flist.assign(4, 99);
        int count = 0;
        for (auto it = assign_flist.begin(); it != assign_flist.end(); ++it) {
            REQUIRE(*it == 99);
            ++count;
        }
        REQUIRE(count == 4);

        // assign with iterators
        std::vector<int> vec = {5, 6, 7};
        assign_flist.assign(vec.begin(), vec.end());
        auto it = assign_flist.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);
        REQUIRE(*it++ == 7);

        // assign with initializer list
        assign_flist.assign({15, 25, 35});
        it = assign_flist.begin();
        REQUIRE(*it++ == 15);
        REQUIRE(*it++ == 25);
        REQUIRE(*it++ == 35);
    }

    SECTION("Iterators") {
        auto it = flist.begin();
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);

        auto before_it = flist.before_begin();
        REQUIRE(++before_it == flist.begin());

        auto const_it = flist.cbegin();
        REQUIRE(*const_it == 1);
        ++const_it;
        REQUIRE(*const_it == 2);

        // Test const iterators
        const j::forward_list<int>& const_flist = flist;
        auto const_begin = const_flist.begin();
        auto const_end = const_flist.end();
        REQUIRE(std::distance(const_begin, const_end) == 5);
    }

    SECTION("Capacity") {
        j::forward_list<int> empty_flist;
        REQUIRE(!flist.empty());
        REQUIRE(empty_flist.empty());

        // max_size should be reasonable
        REQUIRE(flist.max_size() > 0);
    }

    SECTION("Element access") {
        REQUIRE(flist.front() == 1);
        REQUIRE(char_flist.front() == 'a');

        // Test const versions
        const auto& const_flist = flist;
        REQUIRE(const_flist.front() == 1);
    }

    SECTION("Resize") {
        j::forward_list<int> resize_flist = {1, 2, 3};

        // Resize smaller
        resize_flist.resize(2);
        auto it = resize_flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(it == resize_flist.end());

        // Resize larger
        resize_flist.resize(4);
        it = resize_flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 0);
        REQUIRE(*it++ == 0);

        // Resize with value
        resize_flist.resize(6, 42);
        it = resize_flist.begin();
        std::advance(it, 4);
        REQUIRE(*it++ == 42);
        REQUIRE(*it++ == 42);
    }

    SECTION("Comparison operators") {
        j::forward_list<int> flist1 = {1, 2, 3, 4, 5};
        j::forward_list<int> flist2 = {1, 2, 3, 4, 5};
        j::forward_list<int> flist3 = {5, 4, 3, 2, 1};
        j::forward_list<int> flist4 = {1, 2, 3, 4, 6};
        j::forward_list<int> flist5 = {1, 2, 3, 4};

        REQUIRE(flist1 == flist2);
        REQUIRE(flist1 != flist3);
        REQUIRE(flist1 != flist4);
        REQUIRE(flist1 != flist5);

        REQUIRE((flist1 <=> flist2) == std::strong_ordering::equal);
        REQUIRE((flist1 <=> flist3) == std::strong_ordering::less);
        REQUIRE((flist3 <=> flist1) == std::strong_ordering::greater);
        REQUIRE((flist1 <=> flist4) == std::strong_ordering::less);
        REQUIRE((flist1 <=> flist5) == std::strong_ordering::greater);
    }
}

TEST_CASE("Forward List Modifiers") {
    j::forward_list<int> flist;

    SECTION("Push/Pop front") {
        flist.push_front(10);
        flist.push_front(20);
        REQUIRE(flist.front() == 20);

        flist.pop_front();
        REQUIRE(flist.front() == 10);

        flist.pop_front();
        REQUIRE(flist.empty());
    }

    SECTION("Emplace front") {
        flist.emplace_front(100);
        flist.emplace_front(200);
        REQUIRE(flist.front() == 200);
    }

    SECTION("Insert after") {
        flist = {1, 3, 5};
        auto it = flist.begin();

        // Insert single element after first
        auto insert_it = flist.insert_after(it, 2);
        REQUIRE(*insert_it == 2);

        // Check order: should be 1, 2, 3, 5
        auto check_it = flist.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
        REQUIRE(*check_it++ == 5);

        // Insert multiple elements
        it = flist.begin();
        ++it; // after 1
        flist.insert_after(it, 3, 10);

        // Insert with iterators
        std::vector<int> vec = {20, 21};
        it = flist.before_begin();
        flist.insert_after(it, vec.begin(), vec.end());

        // Insert with initializer list
        it = flist.begin();
        flist.insert_after(it, {-1, -2});
    }

    SECTION("Emplace after") {
        flist = {1, 3};
        auto it = flist.begin();
        auto emplace_it = flist.emplace_after(it, 2);
        REQUIRE(*emplace_it == 2);

        auto check_it = flist.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
    }

    SECTION("Erase after") {
        flist = {1, 2, 3, 4, 5};

        // Erase single element after first
        auto it = flist.begin();
        auto erase_it = flist.erase_after(it);
        REQUIRE(*erase_it == 3);

        // Check order: should be 1, 3, 4, 5
        auto check_it = flist.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 3);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);

        // Erase range
        it = flist.begin();
        auto last_it = flist.begin();
        std::advance(last_it, 3);
        flist.erase_after(it, last_it);

        // Should be 1, 5
        check_it = flist.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 5);
    }

    SECTION("Clear") {
        flist = {1, 2, 3};
        flist.clear();
        REQUIRE(flist.empty());
    }

    SECTION("Swap") {
        j::forward_list<int> flist1 = {1, 2, 3};
        j::forward_list<int> flist2 = {4, 5, 6};

        flist1.swap(flist2);
        REQUIRE(flist1.front() == 4);
        REQUIRE(flist2.front() == 1);

        // Test non-member swap
        j::swap(flist1, flist2);
        REQUIRE(flist1.front() == 1);
        REQUIRE(flist2.front() == 4);
    }
}

TEST_CASE("Forward List Operations") {
    SECTION("Splice after") {
        j::forward_list<int> flist1 = {1, 2, 3};
        j::forward_list<int> flist2 = {4, 5, 6};

        // Splice entire list after first element
        auto it = flist1.begin();
        flist1.splice_after(it, flist2);
        REQUIRE(flist2.empty());

        auto check_it = flist1.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);
        REQUIRE(*check_it++ == 6);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);

        // Splice single element
        flist2 = {10, 20, 30};
        it = flist1.before_begin();
        auto splice_it = flist2.begin();
        flist1.splice_after(it, flist2, splice_it);
        REQUIRE(flist1.front() == 20);

        // Splice range
        flist2 = {100, 200, 300};
        it = flist1.before_begin();
        auto first = flist2.before_begin();
        auto last = flist2.begin();
        std::advance(last, 2);
        flist1.splice_after(it, flist2, first, last);
    }

    SECTION("Remove") {
        j::forward_list<int> flist = {1, 2, 2, 3, 2, 4};
        size_t removed = flist.remove(2);
        REQUIRE(removed == 3);

        auto it = flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);

        // Remove non-existent element
        size_t removed2 = flist.remove(99);
        REQUIRE(removed2 == 0);
    }

    SECTION("Remove if") {
        j::forward_list<int> flist = {1, 2, 3, 4, 5, 6};
        size_t removed = flist.remove_if([](int n) { return n % 2 == 0; });
        REQUIRE(removed == 3);

        auto it = flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 5);

        // Test with custom predicate
        j::forward_list<int> flist2 = {10, 15, 20, 25, 30};
        size_t removed2 = flist2.remove_if([](int n) { return n > 20; });
        REQUIRE(removed2 == 2);
    }

    SECTION("Unique") {
        j::forward_list<int> flist = {1, 1, 2, 2, 2, 3, 3};
        size_t removed = flist.unique();
        REQUIRE(removed == 4);

        auto it = flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);

        // Test with custom predicate
        j::forward_list<int> flist2 = {1, 5, 2, 6, 3, 5};
        size_t removed2 = flist2.unique([](int a, int b) { return (a % 2) == (b % 2); });
        REQUIRE(removed2 == 3);
    }

    SECTION("Merge") {
        j::forward_list<int> flist1 = {1, 3, 5};
        j::forward_list<int> flist2 = {2, 4, 6};

        flist1.merge(flist2);
        REQUIRE(flist2.empty());

        auto it = flist1.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);

        // Test merge with custom comparator
        j::forward_list<int> flist3 = {5, 3, 1};
        j::forward_list<int> flist4 = {6, 4, 2};
        flist3.merge(flist4, std::greater<int>());
        REQUIRE(flist4.empty());
    }

    SECTION("Sort") {
        j::forward_list<int> flist = {5, 2, 8, 1, 9, 3};
        flist.sort();

        auto it = flist.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 8);
        REQUIRE(*it++ == 9);

        // Test sort with custom comparator
        flist = {1, 2, 3, 4, 5};
        flist.sort(std::greater<int>());

        it = flist.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);

        // Test empty list sort
        j::forward_list<int> empty_flist;
        empty_flist.sort();
        REQUIRE(empty_flist.empty());

        // Test single element sort
        j::forward_list<int> single_flist = {42};
        single_flist.sort();
        REQUIRE(single_flist.front() == 42);
    }

    SECTION("Reverse") {
        j::forward_list<int> flist = {1, 2, 3, 4, 5};
        flist.reverse();

        auto it = flist.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);

        // Test empty list reverse
        j::forward_list<int> empty_flist;
        empty_flist.reverse();
        REQUIRE(empty_flist.empty());

        // Test single element reverse
        j::forward_list<int> single_flist = {42};
        single_flist.reverse();
        REQUIRE(single_flist.front() == 42);
    }
}

TEST_CASE("Forward List Edge Cases") {
    SECTION("Empty list operations") {
        j::forward_list<int> empty_flist;

        // Test iterators on empty list
        REQUIRE(empty_flist.begin() == empty_flist.end());
        REQUIRE(empty_flist.cbegin() == empty_flist.cend());

        // Clear empty list
        empty_flist.clear();
        REQUIRE(empty_flist.empty());

        // Resize empty list
        empty_flist.resize(3, 42);
        int count = 0;
        for (auto it = empty_flist.begin(); it != empty_flist.end(); ++it) {
            REQUIRE(*it == 42);
            ++count;
        }
        REQUIRE(count == 3);
    }

    SECTION("Single element operations") {
        j::forward_list<int> single_flist = {42};

        REQUIRE(single_flist.front() == 42);
        REQUIRE(!single_flist.empty());

        single_flist.reverse();
        REQUIRE(single_flist.front() == 42);

        single_flist.sort();
        REQUIRE(single_flist.front() == 42);

        single_flist.unique();
        REQUIRE(single_flist.front() == 42);
    }

    SECTION("Before begin operations") {
        j::forward_list<int> flist = {1, 2, 3};
        auto before_it = flist.before_begin();

        // Insert after before_begin
        flist.insert_after(before_it, 0);
        REQUIRE(flist.front() == 0);

        // Emplace after before_begin
        flist.emplace_after(before_it, -1);
        REQUIRE(flist.front() == -1);

        // Erase after before_begin
        flist.erase_after(before_it);
        REQUIRE(flist.front() == 0);
    }
}

TEST_CASE("Forward List Operations on Large Dataset") {
    const size_t data_size = 1000;

    SECTION("Push and Pop") {
        j::forward_list<int> flist;
        for (size_t i = 0; i < data_size; ++i) {
            flist.push_front(static_cast<int>(i));
        }

        for (size_t i = data_size; i > 0; --i) {
            REQUIRE(flist.front() == static_cast<int>(i - 1));
            flist.pop_front();
        }
        REQUIRE(flist.empty());
    }

    SECTION("Insert after and Erase after") {
        j::forward_list<int> flist;
        flist.push_front(0);

        auto it = flist.before_begin();
        for (size_t i = 1; i < data_size; ++i) {
            it = flist.insert_after(it, static_cast<int>(i));
        }

        // Erase every other element
        it = flist.begin();
        size_t count = 0;
        while (std::next(it) != flist.end()) {
            if (count % 2 == 0) {
                it = flist.erase_after(it);
            } else {
                ++it;
            }
            ++count;
        }
    }

    SECTION("Remove and Remove_if") {
        j::forward_list<int> flist;
        for (size_t i = 0; i < data_size; ++i) {
            flist.push_front(static_cast<int>(i % 10)); // 0-9 repeated
        }

        // Remove value '5'
        size_t removed_count = flist.remove(5);
        REQUIRE(removed_count == 100);
        for (auto it = flist.begin(); it != flist.end(); ++it) {
            REQUIRE(*it != 5);
        }

        // Remove even numbers
        removed_count = flist.remove_if([](int n) { return n % 2 == 0; });
        REQUIRE(removed_count == 500);
        for (auto it = flist.begin(); it != flist.end(); ++it) {
            REQUIRE(*it % 2 != 0);
        }
    }

    SECTION("Sort and Merge") {
        j::forward_list<int> flist1;
        j::forward_list<int> flist2;
        std::vector<int> vec1;
        std::vector<int> vec2;

        std::random_device rd;
        std::mt19937 gen(42); // Fixed seed for reproducibility
        std::uniform_int_distribution<> dis(0, 9999);

        for (size_t i = 0; i < data_size / 2; ++i) {
            int val1 = dis(gen);
            int val2 = dis(gen);
            flist1.push_front(val1);
            flist2.push_front(val2);
            vec1.push_back(val1);
            vec2.push_back(val2);
        }

        // Sort
        flist1.sort();
        std::sort(vec1.begin(), vec1.end());
        REQUIRE(std::equal(flist1.begin(), flist1.end(), vec1.begin()));

        flist2.sort();
        std::sort(vec2.begin(), vec2.end());
        REQUIRE(std::equal(flist2.begin(), flist2.end(), vec2.begin()));

        // Merge
        flist1.merge(flist2);
        vec1.insert(vec1.end(), vec2.begin(), vec2.end());
        std::sort(vec1.begin(), vec1.end());

        REQUIRE(flist2.empty());
        REQUIRE(std::equal(flist1.begin(), flist1.end(), vec1.begin()));
    }

    SECTION("Unique") {
        j::forward_list<int> flist;
        for (size_t i = 0; i < data_size / 5; ++i) {
            for(int j = 0; j < 5; ++j) {
                flist.push_front(static_cast<int>(i));
            }
        }

        size_t removed_count = flist.unique();
        REQUIRE(removed_count == data_size - (data_size / 5));
    }

    SECTION("Splice after") {
        j::forward_list<int> flist1, flist2;
        for(int i = 0; i < 500; ++i) flist1.push_front(i);
        for(int i = 500; i < 1000; ++i) flist2.push_front(i);

        auto it = flist1.begin();
        std::advance(it, 250);

        flist1.splice_after(it, flist2);
        REQUIRE(flist2.empty());
    }

    SECTION("Reverse") {
        j::forward_list<int> flist;
        std::vector<int> vec;
        for(size_t i = 0; i < data_size; ++i) {
            flist.push_front(static_cast<int>(i));
            vec.push_back(static_cast<int>(i));
        }

        flist.reverse();
        REQUIRE(std::equal(flist.begin(), flist.end(), vec.begin()));
    }
}
