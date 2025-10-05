/*
 * @ Created by jaehyung409 on 25. 10. 4..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This benchmark code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <numeric>
#include <algorithm>
#include <set>
#include <random>
import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Set Benchmarks: Insert Operations") {
    SECTION("Insert single element") {
        BENCHMARK("j::set insert") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set insert") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset insert") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            return s.size();
        };
        BENCHMARK("std::multiset insert") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            return s.size();
        };
    }
    SECTION("Insert with hint") {
        BENCHMARK("j::set insert with hint") {
            j::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.insert(hint, static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set insert with hint") {
            std::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.insert(hint, static_cast<int>(i));
            return s.size();
        };
    }
    SECTION("Insert random order") {
        BENCHMARK("j::set insert random") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(dis(gen));
            return s.size();
        };
        BENCHMARK("std::set insert random") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(dis(gen));
            return s.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Emplace Operations") {
    SECTION("Emplace single element") {
        BENCHMARK("j::set emplace") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set emplace") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset emplace") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i % 500));
            return s.size();
        };
        BENCHMARK("std::multiset emplace") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.emplace(static_cast<int>(i % 500));
            return s.size();
        };
    }
    SECTION("Emplace with hint") {
        BENCHMARK("j::set emplace_hint") {
            j::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.emplace_hint(hint, static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set emplace_hint") {
            std::set<int> s;
            auto hint = s.end();
            for (size_t i = 0; i < N; ++i) hint = s.emplace_hint(hint, static_cast<int>(i));
            return s.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Erase Operations") {
    SECTION("Erase by key") {
        BENCHMARK("j::set erase by key") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set erase by key") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("j::multiset erase by key") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            for (size_t i = 0; i < 500; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::multiset erase by key") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 500));
            for (size_t i = 0; i < 500; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
    }
    SECTION("Erase by iterator") {
        BENCHMARK("j::set erase by iterator") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            while (!s.empty()) s.erase(s.begin());
            return s.size();
        };
        BENCHMARK("std::set erase by iterator") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            while (!s.empty()) s.erase(s.begin());
            return s.size();
        };
    }
    SECTION("Erase range") {
        BENCHMARK("j::set erase range") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            auto it = s.begin();
            std::advance(it, N / 2);
            s.erase(s.begin(), it);
            return s.size();
        };
        BENCHMARK("std::set erase range") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            auto it = s.begin();
            std::advance(it, N / 2);
            s.erase(s.begin(), it);
            return s.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Find Operations") {
    SECTION("Find existing elements") {
        BENCHMARK("j::set find") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::set find") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
    }
    SECTION("Find non-existing elements") {
        BENCHMARK("j::set find missing") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::set find missing") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
    }
    SECTION("Random find pattern") {
        BENCHMARK("j::set find random") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(dis(gen) % (2 * N)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::set find random") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.find(dis(gen) % (2 * N)) != s.end()) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Set Benchmarks: Contains Operations") {
    SECTION("Contains existing elements") {
        BENCHMARK("j::set contains") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
        BENCHMARK("std::set contains") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Set Benchmarks: Count Operations") {
    SECTION("Count in set") {
        BENCHMARK("j::set count") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
        BENCHMARK("std::set count") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
    }
    SECTION("Count in multiset") {
        BENCHMARK("j::multiset count") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
        BENCHMARK("std::multiset count") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
    }
}

TEST_CASE("Set Benchmarks: Bound Operations") {
    SECTION("Lower bound") {
        BENCHMARK("j::set lower_bound") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.lower_bound(static_cast<int>(i)) != s.end()) ++count;
            }
            return count;
        };
        BENCHMARK("std::set lower_bound") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.lower_bound(static_cast<int>(i)) != s.end()) ++count;
            }
            return count;
        };
    }
    SECTION("Upper bound") {
        BENCHMARK("j::set upper_bound") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.upper_bound(static_cast<int>(i)) != s.end()) ++count;
            }
            return count;
        };
        BENCHMARK("std::set upper_bound") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i * 2));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                if (s.upper_bound(static_cast<int>(i)) != s.end()) ++count;
            }
            return count;
        };
    }
    SECTION("Equal range") {
        BENCHMARK("j::set equal_range") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
        BENCHMARK("std::set equal_range") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
    }
    SECTION("Equal range in multiset") {
        BENCHMARK("j::multiset equal_range") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
        BENCHMARK("std::multiset equal_range") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 100));
            size_t count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto [first, last] = s.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
    }
}

TEST_CASE("Set Benchmarks: Iteration Operations") {
    SECTION("Forward iteration") {
        BENCHMARK("j::set forward iter") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.begin(); it != s.end(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::set forward iter") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.begin(); it != s.end(); ++it) sum += *it;
            return sum;
        };
    }
    SECTION("Reverse iteration") {
        BENCHMARK("j::set reverse iter") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.rbegin(); it != s.rend(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::set reverse iter") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = s.rbegin(); it != s.rend(); ++it) sum += *it;
            return sum;
        };
    }
    SECTION("Range-based for loop") {
        BENCHMARK("j::set range-for") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (int x : s) sum += x;
            return sum;
        };
        BENCHMARK("std::set range-for") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (int x : s) sum += x;
            return sum;
        };
    }
}

TEST_CASE("Set Benchmarks: Memory/State Operations") {
    SECTION("Clear") {
        BENCHMARK("j::set clear") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            s.clear();
            return s.size();
        };
        BENCHMARK("std::set clear") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            s.clear();
            return s.size();
        };
    }
    SECTION("Swap") {
        BENCHMARK("j::set swap") {
            j::set<int> s1, s2;
            for (size_t i = 0; i < N; ++i) s1.insert(static_cast<int>(i));
            for (size_t i = N; i < 2 * N; ++i) s2.insert(static_cast<int>(i));
            s1.swap(s2);
            return s1.size() + s2.size();
        };
        BENCHMARK("std::set swap") {
            std::set<int> s1, s2;
            for (size_t i = 0; i < N; ++i) s1.insert(static_cast<int>(i));
            for (size_t i = N; i < 2 * N; ++i) s2.insert(static_cast<int>(i));
            s1.swap(s2);
            return s1.size() + s2.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Copy/Move Operations") {
    SECTION("Copy construction") {
        BENCHMARK("j::set copy construct") {
            j::set<int> original;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            j::set<int> copy = original;
            return copy.size();
        };
        BENCHMARK("std::set copy construct") {
            std::set<int> original;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            std::set<int> copy = original;
            return copy.size();
        };
    }
    SECTION("Move construction") {
        BENCHMARK("j::set move construct") {
            j::set<int> original;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            j::set<int> moved = std::move(original);
            return moved.size();
        };
        BENCHMARK("std::set move construct") {
            std::set<int> original;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            std::set<int> moved = std::move(original);
            return moved.size();
        };
    }
    SECTION("Copy assignment") {
        BENCHMARK("j::set copy assign") {
            j::set<int> original, target;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            target = original;
            return target.size();
        };
        BENCHMARK("std::set copy assign") {
            std::set<int> original, target;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            target = original;
            return target.size();
        };
    }
    SECTION("Move assignment") {
        BENCHMARK("j::set move assign") {
            j::set<int> original, target;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            target = std::move(original);
            return target.size();
        };
        BENCHMARK("std::set move assign") {
            std::set<int> original, target;
            for (size_t i = 0; i < N; ++i) original.insert(static_cast<int>(i));
            target = std::move(original);
            return target.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Construction Variants") {
    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 0);
    SECTION("Range construction") {
        BENCHMARK("j::set range construct") {
            j::set<int> s(v.begin(), v.end());
            return s.size();
        };
        BENCHMARK("std::set range construct") {
            std::set<int> s(v.begin(), v.end());
            return s.size();
        };
    }
    SECTION("Initializer list construction") {
        BENCHMARK("j::set initializer_list construct") {
            j::set<int> s = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            return s.size();
        };
        BENCHMARK("std::set initializer_list construct") {
            std::set<int> s = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            return s.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Insert Range Operations") {
    std::vector<int> v(100);
    std::iota(v.begin(), v.end(), 0);
    SECTION("Insert iterator range") {
        BENCHMARK("j::set insert range") {
            j::set<int> s;
            s.insert(v.begin(), v.end());
            return s.size();
        };
        BENCHMARK("std::set insert range") {
            std::set<int> s;
            s.insert(v.begin(), v.end());
            return s.size();
        };
    }
    SECTION("Insert initializer_list") {
        BENCHMARK("j::set insert ilist") {
            j::set<int> s;
            s.insert({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            return s.size();
        };
        BENCHMARK("std::set insert ilist") {
            std::set<int> s;
            s.insert({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            return s.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Extract Operations") {
    SECTION("Extract by iterator") {
        BENCHMARK("j::set extract iterator") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s.extract(s.begin());
                if (!nh.empty()) ++count;
            }
            return count;
        };
        BENCHMARK("std::set extract iterator") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s.extract(s.begin());
                if (!nh.empty()) ++count;
            }
            return count;
        };
    }
    SECTION("Extract by key") {
        BENCHMARK("j::set extract key") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s.extract(static_cast<int>(i));
                if (!nh.empty()) ++count;
            }
            return count;
        };
        BENCHMARK("std::set extract key") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s.extract(static_cast<int>(i));
                if (!nh.empty()) ++count;
            }
            return count;
        };
    }
}

TEST_CASE("Set Benchmarks: Node Handle Insert") {
    SECTION("Insert node_type") {
        BENCHMARK("j::set insert node_type") {
            j::set<int> s1, s2;
            for (size_t i = 0; i < N; ++i) s1.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s1.extract(s1.begin());
                if (!nh.empty()) {
                    s2.insert(std::move(nh));
                    ++count;
                }
            }
            return count;
        };
        BENCHMARK("std::set insert node_type") {
            std::set<int> s1, s2;
            for (size_t i = 0; i < N; ++i) s1.insert(static_cast<int>(i));
            size_t count = 0;
            for (size_t i = 0; i < N / 10; ++i) {
                auto nh = s1.extract(s1.begin());
                if (!nh.empty()) {
                    s2.insert(std::move(nh));
                    ++count;
                }
            }
            return count;
        };
    }
}

TEST_CASE("Set Benchmarks: Merge Operations") {
    SECTION("Merge set to set") {
        BENCHMARK("j::set merge") {
            j::set<int> s1, s2;
            for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i));
            for (size_t i = N / 4; i < 3 * N / 4; ++i) s2.insert(static_cast<int>(i));
            s1.merge(s2);
            return s1.size() + s2.size();
        };
        BENCHMARK("std::set merge") {
            std::set<int> s1, s2;
            for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i));
            for (size_t i = N / 4; i < 3 * N / 4; ++i) s2.insert(static_cast<int>(i));
            s1.merge(s2);
            return s1.size() + s2.size();
        };
    }
    // SECTION("Merge multiset to set") {
    //     BENCHMARK("j::set merge from multiset") {
    //         j::set<int> s1;
    //         j::multiset<int> s2;
    //         for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i));
    //         for (size_t i = 0; i < N; ++i) s2.insert(static_cast<int>(i % (N / 2)));
    //         s1.merge(s2);
    //         return s1.size() + s2.size();
    //     };
    //     BENCHMARK("std::set merge from multiset") {
    //         std::set<int> s1;
    //         std::multiset<int> s2;
    //         for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i));
    //         for (size_t i = 0; i < N; ++i) s2.insert(static_cast<int>(i % (N / 2)));
    //         s1.merge(s2);
    //         return s1.size() + s2.size();
    //     };
    // }
    SECTION("Merge multiset to multiset") {
        BENCHMARK("j::multiset merge") {
            j::multiset<int> s1, s2;
            for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i % 100));
            for (size_t i = 0; i < N / 2; ++i) s2.insert(static_cast<int>(i % 100));
            s1.merge(s2);
            return s1.size() + s2.size();
        };
        BENCHMARK("std::multiset merge") {
            std::multiset<int> s1, s2;
            for (size_t i = 0; i < N / 2; ++i) s1.insert(static_cast<int>(i % 100));
            for (size_t i = 0; i < N / 2; ++i) s2.insert(static_cast<int>(i % 100));
            s1.merge(s2);
            return s1.size() + s2.size();
        };
    }
}

TEST_CASE("Set Benchmarks: Algorithm Integration") {
    SECTION("std::find") {
        BENCHMARK("j::set std::find") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(s.begin(), s.end(), target);
                if (it != s.end()) ++found_count;
            }
            return found_count;
        };
        BENCHMARK("std::set std::find") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(s.begin(), s.end(), target);
                if (it != s.end()) ++found_count;
            }
            return found_count;
        };
    }
    SECTION("std::accumulate") {
        BENCHMARK("j::set std::accumulate") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return std::accumulate(s.begin(), s.end(), 0);
        };
        BENCHMARK("std::set std::accumulate") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return std::accumulate(s.begin(), s.end(), 0);
        };
    }
    SECTION("std::count_if") {
        BENCHMARK("j::set std::count_if") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return std::count_if(s.begin(), s.end(), [](int x) { return x % 2 == 0; });
        };
        BENCHMARK("std::set std::count_if") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i));
            return std::count_if(s.begin(), s.end(), [](int x) { return x % 2 == 0; });
        };
    }
}

TEST_CASE("Set Benchmarks: Mixed Operations Pattern") {
    SECTION("Interleaved insert/erase") {
        BENCHMARK("j::set insert/erase mix") {
            j::set<int> s;
            for (size_t i = 0; i < N; ++i) {
                s.insert(static_cast<int>(i));
                if (i % 3 == 0 && !s.empty()) s.erase(s.begin());
            }
            return s.size();
        };
        BENCHMARK("std::set insert/erase mix") {
            std::set<int> s;
            for (size_t i = 0; i < N; ++i) {
                s.insert(static_cast<int>(i));
                if (i % 3 == 0 && !s.empty()) s.erase(s.begin());
            }
            return s.size();
        };
    }
    SECTION("Interleaved insert/find") {
        BENCHMARK("j::set insert/find mix") {
            j::set<int> s;
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                s.insert(static_cast<int>(i));
                if (i > 0 && s.find(static_cast<int>(i / 2)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::set insert/find mix") {
            std::set<int> s;
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                s.insert(static_cast<int>(i));
                if (i > 0 && s.find(static_cast<int>(i / 2)) != s.end()) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Set Benchmarks: Large Scale Operations") {
    constexpr size_t LARGE_N = 10000;
    SECTION("Large insert sequential") {
        BENCHMARK("j::set large insert seq") {
            j::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::set large insert seq") {
            std::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            return s.size();
        };
    }
    SECTION("Large insert random") {
        BENCHMARK("j::set large insert random") {
            j::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(dis(gen));
            return s.size();
        };
        BENCHMARK("std::set large insert random") {
            std::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(dis(gen));
            return s.size();
        };
    }
    SECTION("Large find operations") {
        BENCHMARK("j::set large find") {
            j::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < LARGE_N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::set large find") {
            std::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            size_t found = 0;
            for (size_t i = 0; i < LARGE_N; ++i) {
                if (s.find(static_cast<int>(i)) != s.end()) ++found;
            }
            return found;
        };
    }
    SECTION("Large iteration") {
        BENCHMARK("j::set large iterate") {
            j::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (int x : s) sum += x;
            return sum;
        };
        BENCHMARK("std::set large iterate") {
            std::set<int> s;
            for (size_t i = 0; i < LARGE_N; ++i) s.insert(static_cast<int>(i));
            size_t sum = 0;
            for (int x : s) sum += x;
            return sum;
        };
    }
}

TEST_CASE("Set Benchmarks: Multiset Specific Operations") {
    SECTION("Multiset duplicate inserts") {
        BENCHMARK("j::multiset many duplicates") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 10));
            return s.size();
        };
        BENCHMARK("std::multiset many duplicates") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 10));
            return s.size();
        };
    }
    SECTION("Multiset count duplicates") {
        BENCHMARK("j::multiset count duplicates") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 50));
            size_t total = 0;
            for (size_t i = 0; i < 50; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
        BENCHMARK("std::multiset count duplicates") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 50));
            size_t total = 0;
            for (size_t i = 0; i < 50; ++i) total += s.count(static_cast<int>(i));
            return total;
        };
    }
    SECTION("Multiset erase all duplicates") {
        BENCHMARK("j::multiset erase all dupes") {
            j::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 50));
            for (size_t i = 0; i < 50; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
        BENCHMARK("std::multiset erase all dupes") {
            std::multiset<int> s;
            for (size_t i = 0; i < N; ++i) s.insert(static_cast<int>(i % 50));
            for (size_t i = 0; i < 50; ++i) s.erase(static_cast<int>(i));
            return s.size();
        };
    }
}

