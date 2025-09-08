/*
 * @ Created by jaehyung409 on 25. 8. 22.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <numeric>
#include <algorithm>
#include <deque>
#include <random>
import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Deque Benchmarks: Push/Pop Operations") {
    SECTION("Push back/front comparison") {
        BENCHMARK("j::deque push_back") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque push_back") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("j::deque push_front") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_front(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque push_front") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_front(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
    }
    SECTION("Pop back/front comparison") {
        BENCHMARK("j::deque pop_back") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.pop_back();
            return d.size();
        };
        BENCHMARK("std::deque pop_back") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.pop_back();
            return d.size();
        };
        BENCHMARK("j::deque pop_front") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.pop_front();
            return d.size();
        };
        BENCHMARK("std::deque pop_front") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.pop_front();
            return d.size();
        };
    }
}

TEST_CASE("Deque Benchmarks: Insert/Erase Operations") {
    SECTION("Insert at beginning/end") {
        BENCHMARK("j::deque insert begin") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.insert(d.begin(), static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque insert begin") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.insert(d.begin(), static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("j::deque insert end") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.insert(d.end(), static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque insert end") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.insert(d.end(), static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
    }
    SECTION("Erase from beginning/end") {
        BENCHMARK("j::deque erase begin") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.erase(d.begin());
            return d.size();
        };
        BENCHMARK("std::deque erase begin") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.erase(d.begin());
            return d.size();
        };
        BENCHMARK("j::deque erase end") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.erase(--d.end());
            return d.size();
        };
        BENCHMARK("std::deque erase end") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            while (!d.empty()) d.erase(--d.end());
            return d.size();
        };
    }
}

TEST_CASE("Deque Benchmarks: Access/Iteration") {
    SECTION("Random access operator[]/at()") {
        BENCHMARK("j::deque operator[]") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) sum += d[i];
            return sum;
        };
        BENCHMARK("std::deque operator[]") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) sum += d[i];
            return sum;
        };
        BENCHMARK("j::deque at()") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) sum += d.at(i);
            return sum;
        };
        BENCHMARK("std::deque at()") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) sum += d.at(i);
            return sum;
        };
    }
    SECTION("Forward/Reverse iteration") {
        BENCHMARK("j::deque forward iter") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.begin(); it != d.end(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::deque forward iter") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.begin(); it != d.end(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("j::deque reverse iter") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.rbegin(); it != d.rend(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::deque reverse iter") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.rbegin(); it != d.rend(); ++it) sum += *it;
            return sum;
        };
    }
}

TEST_CASE("Deque Benchmarks: Insert/Erase Variants") {
    std::vector<int> v(100, 42);
    SECTION("Insert multiple (count)") {
        BENCHMARK("j::deque insert count") {
            j::deque<int> d;
            d.insert(d.begin(), 100, 42);
            return d.size();
        };
        BENCHMARK("std::deque insert count") {
            std::deque<int> d;
            d.insert(d.begin(), 100, 42);
            return d.size();
        };
    }
    SECTION("Insert with iterator range") {
        BENCHMARK("j::deque insert iterator range") {
            j::deque<int> d;
            d.insert(d.begin(), v.begin(), v.end());
            return d.size();
        };
        BENCHMARK("std::deque insert iterator range") {
            std::deque<int> d;
            d.insert(d.begin(), v.begin(), v.end());
            return d.size();
        };
    }
    SECTION("Insert with initializer_list") {
        BENCHMARK("j::deque insert ilist") {
            j::deque<int> d;
            d.insert(d.begin(), {1,2,3,4,5});
            return d.size();
        };
        BENCHMARK("std::deque insert ilist") {
            std::deque<int> d;
            d.insert(d.begin(), {1,2,3,4,5});
            return d.size();
        };
    }
    SECTION("Erase range") {
        BENCHMARK("j::deque erase range") {
            j::deque<int> d(N, 1);
            d.erase(d.begin(), d.begin() + N/2);
            return d.size();
        };
        BENCHMARK("std::deque erase range") {
            std::deque<int> d(N, 1);
            d.erase(d.begin(), d.begin() + N/2);
            return d.size();
        };
    }
}

TEST_CASE("Deque Benchmarks: Memory/State Operations") {
    SECTION("shrink_to_fit") {
        BENCHMARK("j::deque shrink_to_fit") {
            j::deque<int> d(N, 1);
            d.shrink_to_fit();
            return d.size();
        };
        BENCHMARK("std::deque shrink_to_fit") {
            std::deque<int> d(N, 1);
            d.shrink_to_fit();
            return d.size();
        };
    }
    SECTION("clear") {
        BENCHMARK("j::deque clear") {
            j::deque<int> d(N, 1);
            d.clear();
            return d.size();
        };
        BENCHMARK("std::deque clear") {
            std::deque<int> d(N, 1);
            d.clear();
            return d.size();
        };
    }
    SECTION("swap") {
        BENCHMARK("j::deque swap") {
            j::deque<int> d1(N, 1), d2(N, 2);
            d1.swap(d2);
            return d1.front() + d2.front();
        };
        BENCHMARK("std::deque swap") {
            std::deque<int> d1(N, 1), d2(N, 2);
            d1.swap(d2);
            return d1.front() + d2.front();
        };
    }
}

TEST_CASE("Deque Benchmarks: Memory/Copy/Move") {
    SECTION("Construction/Destruction") {
        BENCHMARK("j::deque construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                j::deque<int> d;
                for (size_t j = 0; j < N / 100; ++j) d.push_back(static_cast<int>(j));
            }
            return 1;
        };
        BENCHMARK("std::deque construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                std::deque<int> d;
                for (size_t j = 0; j < N / 100; ++j) d.push_back(static_cast<int>(j));
            }
            return 1;
        };
    }
    SECTION("Copy/Move operations") {
        BENCHMARK("j::deque copy") {
            j::deque<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            j::deque<int> copy = original;
            return copy.size();
        };
        BENCHMARK("std::deque copy") {
            std::deque<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            std::deque<int> copy = original;
            return copy.size();
        };
        BENCHMARK("j::deque move") {
            j::deque<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            j::deque<int> moved = std::move(original);
            return moved.size();
        };
        BENCHMARK("std::deque move") {
            std::deque<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            std::deque<int> moved = std::move(original);
            return moved.size();
        };
    }
}

TEST_CASE("Deque Benchmarks: Insert at Middle") {
    SECTION("Insert at middle") {
        BENCHMARK("j::deque insert middle") {
            j::deque<int> d;
            for (size_t i = 0; i < 100; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) {
                auto it = d.begin() + d.size() / 2;
                d.insert(it, 999);
            }
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque insert middle") {
            std::deque<int> d;
            for (size_t i = 0; i < 100; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) {
                auto it = d.begin() + d.size() / 2;
                d.insert(it, 999);
            }
            return d.size() + d.front() + d.back();
        };
    }
}

TEST_CASE("Deque Benchmarks: Erase at Middle") {
    SECTION("Erase from middle") {
        BENCHMARK("j::deque erase middle") {
            j::deque<int> d;
            for (size_t i = 0; i < 2 * N; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N && !d.empty(); ++i) {
                auto it = d.begin() + d.size() / 2;
                d.erase(it);
            }
            return d.size();
        };
        BENCHMARK("std::deque erase middle") {
            std::deque<int> d;
            for (size_t i = 0; i < 2 * N; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N && !d.empty(); ++i) {
                auto it = d.begin() + d.size() / 2;
                d.erase(it);
            }
            return d.size();
        };
    }
}

TEST_CASE("Deque Benchmarks: Random Access Pattern") {
    SECTION("Random access pattern") {
        BENCHMARK("j::deque random access") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(dis(gen));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                auto index = dis(gen) % d.size();
                sum += d[index];
            }
            return sum;
        };
        BENCHMARK("std::deque random access") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(dis(gen));
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                auto index = dis(gen) % d.size();
                sum += d[index];
            }
            return sum;
        };
    }
}

TEST_CASE("Deque Benchmarks: Iterator Arithmetic") {
    SECTION("Iterator arithmetic") {
        BENCHMARK("j::deque iterator arithmetic") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            auto it = d.begin();
            for (size_t i = 0; i < N; ++i) sum += *(it + i);
            return sum;
        };
        BENCHMARK("std::deque iterator arithmetic") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            auto it = d.begin();
            for (size_t i = 0; i < N; ++i) sum += *(it + i);
            return sum;
        };
    }
}

TEST_CASE("Deque Benchmarks: Algorithm Integration") {
    SECTION("std::sort") {
        BENCHMARK("j::deque std::sort") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(dis(gen));
            std::sort(d.begin(), d.end());
            return d.size();
        };
        BENCHMARK("std::deque std::sort") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(dis(gen));
            std::sort(d.begin(), d.end());
            return d.size();
        };
    }
    SECTION("std::find") {
        BENCHMARK("j::deque std::find") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(d.begin(), d.end(), target);
                if (it != d.end()) ++found_count;
            }
            return found_count;
        };
        BENCHMARK("std::deque std::find") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(d.begin(), d.end(), target);
                if (it != d.end()) ++found_count;
            }
            return found_count;
        };
    }
    SECTION("std::accumulate") {
        BENCHMARK("j::deque std::accumulate") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            return std::accumulate(d.begin(), d.end(), 0);
        };
        BENCHMARK("std::deque std::accumulate") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            return std::accumulate(d.begin(), d.end(), 0);
        };
    }
}

TEST_CASE("Deque Benchmarks: Emplace Operations") {
    SECTION("emplace_back/emplace_front") {
        BENCHMARK("j::deque emplace_back") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.emplace_back(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque emplace_back") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.emplace_back(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("j::deque emplace_front") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.emplace_front(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque emplace_front") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.emplace_front(static_cast<int>(i));
            return d.size() + d.front() + d.back();
        };
    }
}

TEST_CASE("Deque Benchmarks: Iterator-based Traversal") {
    SECTION("for-range forward iteration") {
        BENCHMARK("j::deque for-range forward") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (int x : d) sum += x;
            return sum;
        };
        BENCHMARK("std::deque for-range forward") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (int x : d) sum += x;
            return sum;
        };
    }
    SECTION("for-range reverse iteration") {
        BENCHMARK("j::deque for-range reverse") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.rbegin(); it != d.rend(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::deque for-range reverse") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = d.rbegin(); it != d.rend(); ++it) sum += *it;
            return sum;
        };
    }
    SECTION("const_iterator forward iteration") {
        BENCHMARK("j::deque const_iterator forward") {
            j::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            const j::deque<int>& cd = d;
            size_t sum = 0;
            for (auto it = cd.cbegin(); it != cd.cend(); ++it) sum += *it;
            return sum;
        };
        BENCHMARK("std::deque const_iterator forward") {
            std::deque<int> d;
            for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
            const std::deque<int>& cd = d;
            size_t sum = 0;
            for (auto it = cd.cbegin(); it != cd.cend(); ++it) sum += *it;
            return sum;
        };
    }
}

TEST_CASE("Deque Benchmarks: Iterator-based Insert/Erase") {
    SECTION("iterator erase middle repeatedly") {
        BENCHMARK("j::deque iterator erase middle") {
            j::deque<int> d;
            for (size_t i = 0; i < 2 * N; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N && !d.empty(); ++i) {
                auto it = d.begin() + d.size() / 2;
                d.erase(it);
            }
            return d.size();
        };
        BENCHMARK("std::deque iterator erase middle") {
            std::deque<int> d;
            for (size_t i = 0; i < 2 * N; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N && !d.empty(); ++i) {
                auto it = d.begin() + d.size() / 2;
                d.erase(it);
            }
            return d.size();
        };
    }
    SECTION("iterator insert middle repeatedly") {
        BENCHMARK("j::deque iterator insert middle") {
            j::deque<int> d;
            for (size_t i = 0; i < 100; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) {
                auto it = d.begin() + d.size() / 2;
                d.insert(it, 999);
            }
            return d.size() + d.front() + d.back();
        };
        BENCHMARK("std::deque iterator insert middle") {
            std::deque<int> d;
            for (size_t i = 0; i < 100; ++i) d.push_back(static_cast<int>(i));
            for (size_t i = 0; i < N; ++i) {
                auto it = d.begin() + d.size() / 2;
                d.insert(it, 999);
            }
            return d.size() + d.front() + d.back();
        };
    }
}
