/*
 * @ Created by jaehyung409 on 25. 10. 8..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This benchmark code compares skip_list implementations with and without tree_selector.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <numeric>
#include <random>
import j;

constexpr size_t N = 1000000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Skip List O vs N: Insert Operations") {
    SECTION("Insert single element") {
        BENCHMARK("j::set insert (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::set insert (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset insert (no selector - false)") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            return s.size();
        };
        BENCHMARK("j::multiset insert (with selector - true)") {
            j::multiset<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            return s.size();
        };
    }
    SECTION("Insert with hint") {
        BENCHMARK("j::set insert with hint (no selector - false)") {
            j::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.insert(hint, static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::set insert with hint (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.insert(hint, static_cast<int>(i));
            return s.size();
        };
    }
    SECTION("Insert random order") {
        BENCHMARK("j::set insert random (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(dis(gen));
            return s.size();
        };
        BENCHMARK("j::set insert random (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(dis(gen));
            return s.size();
        };
    }
}

TEST_CASE("Skip List O vs N: Emplace Operations") {
    SECTION("Emplace single element") {
        BENCHMARK("j::set emplace (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::set emplace (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset emplace (no selector - false)") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i % 500));
            return s.size();
        };
        BENCHMARK("j::multiset emplace (with selector - true)") {
            j::multiset<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i % 500));
            return s.size();
        };
    }
    SECTION("Emplace with hint") {
        BENCHMARK("j::set emplace_hint (no selector - false)") {
            j::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.emplace_hint(hint, static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::set emplace_hint (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.emplace_hint(hint, static_cast<int>(i));
            return s.size();
        };
    }
}

TEST_CASE("Skip List O vs N: Erase Operations") {
    SECTION("Erase by key") {
        BENCHMARK("j::set erase by key (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::set erase by key (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset erase by key (no selector - false)") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            for (size_t i = 0; i < 500; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset erase by key (with selector - true)") {
            j::multiset<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            for (size_t i = 0; i < 500; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
    }
    SECTION("Erase by iterator") {
        BENCHMARK("j::set erase by iterator (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            while (!s.empty()) s.erase(s.begin());
            return s.size();
        };
        BENCHMARK("j::set erase by iterator (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            while (!s.empty()) s.erase(s.begin());
            return s.size();
        };
    }
    SECTION("Erase range") {
        BENCHMARK("j::set erase range (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            auto it = s.begin();
            std::advance(it, N / 2);
            s.erase(s.begin(), it);
            return s.size();
        };
        BENCHMARK("j::set erase range (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            auto it = s.begin();
            std::advance(it, N / 2);
            s.erase(s.begin(), it);
            return s.size();
        };
    }
}

TEST_CASE("Skip List O vs N: Find Operations") {
    SECTION("Find existing elements") {
        BENCHMARK("j::set find (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("j::set find (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
    }
    SECTION("Find non-existing elements") {
        BENCHMARK("j::set find missing (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("j::set find missing (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Skip List O vs N: Contains Operations") {
    SECTION("Contains existing elements") {
        BENCHMARK("j::set contains (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
        BENCHMARK("j::set contains (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Skip List O vs N: Count Operations") {
    SECTION("Count in set") {
        BENCHMARK("j::set count (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) {
                total += s.count(static_cast<int>(i));
            }
            return total;
        };
        BENCHMARK("j::set count (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) {
                total += s.count(static_cast<int>(i));
            }
            return total;
        };
    }
    SECTION("Count in multiset") {
        BENCHMARK("j::multiset count (no selector - false)") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) {
                total += s.count(static_cast<int>(i));
            }
            return total;
        };
        BENCHMARK("j::multiset count (with selector - true)") {
            j::multiset<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) {
                total += s.count(static_cast<int>(i));
            }
            return total;
        };
    }
}

TEST_CASE("Skip List O vs N: Bound Operations") {
    SECTION("Lower bound") {
        BENCHMARK("j::set lower_bound (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = s.lower_bound(static_cast<int>(i));
                if (it != s.end()) ++count;
            }
            return count;
        };
        BENCHMARK("j::set lower_bound (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = s.lower_bound(static_cast<int>(i));
                if (it != s.end()) ++count;
            }
            return count;
        };
    }
    SECTION("Upper bound") {
        BENCHMARK("j::set upper_bound (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = s.upper_bound(static_cast<int>(i));
                if (it != s.end()) ++count;
            }
            return count;
        };
        BENCHMARK("j::set upper_bound (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = s.upper_bound(static_cast<int>(i));
                if (it != s.end()) ++count;
            }
            return count;
        };
    }
    SECTION("Equal range") {
        BENCHMARK("j::set equal_range (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
        BENCHMARK("j::set equal_range (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
    }
}

TEST_CASE("Skip List O vs N: Iteration Operations") {
    SECTION("Forward iteration") {
        BENCHMARK("j::set forward iteration (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& value : s) {
                sum += value;
            }
            return sum;
        };
        BENCHMARK("j::set forward iteration (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& value : s) {
                sum += value;
            }
            return sum;
        };
    }
    SECTION("Reverse iteration") {
        BENCHMARK("j::set reverse iteration (no selector - false)") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.rbegin(); it != s.rend(); ++it) {
                sum += *it;
            }
            return sum;
        };
        BENCHMARK("j::set reverse iteration (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.rbegin(); it != s.rend(); ++it) {
                sum += *it;
            }
            return sum;
        };
    }
}

TEST_CASE("Skip List O vs N: Mixed Operations Pattern") {
    SECTION("Insert-Find-Erase cycle") {
        BENCHMARK("j::set mixed operations (no selector - false)") {
            j::set<int> s;
            size_t operations = 0;
            for (size_t i = 0; i < N / 3; ++i) {
                s.insert(static_cast<int>(i));
                ++operations;
                if (s.find(static_cast<int>(i)) != s.end()) ++operations;
                if (i % 2 == 0 && !s.empty()) {
                    s.erase(s.begin());
                    ++operations;
                }
            }
            return operations;
        };
        BENCHMARK("j::set mixed operations (with selector - true)") {
            j::set<int, std::less<int>, std::allocator<int>, j::use_skip_list> s;
            size_t operations = 0;
            for (size_t i = 0; i < N / 3; ++i) {
                s.insert(static_cast<int>(i));
                ++operations;
                if (s.find(static_cast<int>(i)) != s.end()) ++operations;
                if (i % 2 == 0 && !s.empty()) {
                    s.erase(s.begin());
                    ++operations;
                }
            }
            return operations;
        };
    }
}
