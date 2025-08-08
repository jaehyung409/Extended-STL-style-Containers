/*
 * @ Created by jaehyung409 on 25. 8. 3.
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */
 
#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <vector>
#include <random>

import j.vector;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Vector Benchmarks: Push Operations") {
    SECTION("Push back comparison") {
        BENCHMARK("j::vector push_back") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector push_back") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };
    }

    SECTION("Emplace back comparison") {
        BENCHMARK("j::vector emplace_back") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.emplace_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector emplace_back") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.emplace_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };
    }

    SECTION("Push with reserve") {
        BENCHMARK("j::vector push_back reserved") {
            j::vector<int> v;
            v.reserve(N);
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector push_back reserved") {
            std::vector<int> v;
            v.reserve(N);
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };
    }
}

TEST_CASE("Vector Benchmarks: Insert Operations") {
    SECTION("Insert at beginning") {
        BENCHMARK("j::vector insert begin") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.begin(), static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector insert begin") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.begin(), static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };
    }

    SECTION("Insert at middle") {
        BENCHMARK("j::vector insert middle") {
            j::vector<int> v;
            for (size_t i = 0; i < 100; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < N; ++i) {
                auto it = v.begin() + v.size() / 2;
                v.insert(it, 999);
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector insert middle") {
            std::vector<int> v;
            for (size_t i = 0; i < 100; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < N; ++i) {
                auto it = v.begin() + v.size() / 2;
                v.insert(it, 999);
            }
            return v.size() + v.front() + v.back();
        };
    }

    SECTION("Insert multiple elements") {
        BENCHMARK("j::vector insert multiple") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.end(), 10, static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector insert multiple") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.end(), 10, static_cast<int>(i));
            }
            return v.size() + v.front() + v.back();
        };
    }

    SECTION("Insert range") {
        BENCHMARK("j::vector insert range") {
            j::vector<int> v;
            std::vector<int> data = {1, 2, 3, 4, 5};
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.end(), data.begin(), data.end());
            }
            return v.size() + v.front() + v.back();
        };

        BENCHMARK("std::vector insert range") {
            std::vector<int> v;
            std::vector<int> data = {1, 2, 3, 4, 5};
            for (size_t i = 0; i < N; ++i) {
                v.insert(v.end(), data.begin(), data.end());
            }
            return v.size() + v.front() + v.back();
        };
    }
}

TEST_CASE("Vector Benchmarks: Erase Operations") {
    SECTION("Pop back comparison") {
        BENCHMARK("j::vector pop_back") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            while (!v.empty()) {
                v.pop_back();
            }
            return v.size();
        };

        BENCHMARK("std::vector pop_back") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            while (!v.empty()) {
                v.pop_back();
            }
            return v.size();
        };
    }

    SECTION("Erase from beginning") {
        BENCHMARK("j::vector erase begin") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            while (!v.empty()) {
                v.erase(v.begin());
            }
            return v.size();
        };

        BENCHMARK("std::vector erase begin") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            while (!v.empty()) {
                v.erase(v.begin());
            }
            return v.size();
        };
    }

    SECTION("Erase from middle") {
        BENCHMARK("j::vector erase middle") {
            j::vector<int> v;
            for (size_t i = 0; i < 2 * N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < N && !v.empty(); ++i) {
                auto it = v.begin() + v.size() / 2;
                v.erase(it);
            }
            return v.size();
        };

        BENCHMARK("std::vector erase middle") {
            std::vector<int> v;
            for (size_t i = 0; i < 2 * N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < N && !v.empty(); ++i) {
                auto it = v.begin() + v.size() / 2;
                v.erase(it);
            }
            return v.size();
        };
    }

    SECTION("Erase range") {
        BENCHMARK("j::vector erase range") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10 && v.size() >= N / 10; ++i) {
                auto first = v.begin();
                auto last = v.begin() + N / 10;
                v.erase(first, last);
            }
            return v.size();
        };

        BENCHMARK("std::vector erase range") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10 && v.size() >= N / 10; ++i) {
                auto first = v.begin();
                auto last = v.begin() + N / 10;
                v.erase(first, last);
            }
            return v.size();
        };
    }
}

TEST_CASE("Vector Benchmarks: Access Operations") {
    SECTION("Random access operator[]") {
        BENCHMARK("j::vector operator[]") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += v[i];
            }
            return sum;
        };

        BENCHMARK("std::vector operator[]") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += v[i];
            }
            return sum;
        };
    }

    SECTION("Random access at()") {
        BENCHMARK("j::vector at()") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += v.at(i);
            }
            return sum;
        };

        BENCHMARK("std::vector at()") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += v.at(i);
            }
            return sum;
        };
    }

    SECTION("Random access pattern") {
        BENCHMARK("j::vector random access") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                auto index = dis(gen) % v.size();
                sum += v[index];
            }
            return sum;
        };

        BENCHMARK("std::vector random access") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i) {
                auto index = dis(gen) % v.size();
                sum += v[index];
            }
            return sum;
        };
    }
}

TEST_CASE("Vector Benchmarks: Iterator Operations") {
    SECTION("Forward iteration") {
        BENCHMARK("j::vector forward iter") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
            return sum;
        };

        BENCHMARK("std::vector forward iter") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
            return sum;
        };
    }

    SECTION("Reverse iteration") {
        BENCHMARK("j::vector reverse iter") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = v.rbegin(); it != v.rend(); ++it) sum += *it;
            return sum;
        };

        BENCHMARK("std::vector reverse iter") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = v.rbegin(); it != v.rend(); ++it) sum += *it;
            return sum;
        };
    }

    SECTION("Range-based for loop") {
        BENCHMARK("j::vector range-for") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& val : v) sum += val;
            return sum;
        };

        BENCHMARK("std::vector range-for") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& val : v) sum += val;
            return sum;
        };
    }

    SECTION("Iterator arithmetic") {
        BENCHMARK("j::vector iterator arithmetic") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            auto it = v.begin();
            for (size_t i = 0; i < N; ++i) {
                sum += *(it + i);
            }
            return sum;
        };

        BENCHMARK("std::vector iterator arithmetic") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
            size_t sum = 0;
            auto it = v.begin();
            for (size_t i = 0; i < N; ++i) {
                sum += *(it + i);
            }
            return sum;
        };
    }
}

TEST_CASE("Vector Benchmarks: Capacity Operations") {
    SECTION("Resize operations") {
        BENCHMARK("j::vector resize") {
            j::vector<int> v;
            for (size_t i = 0; i < 100; ++i) {
                v.resize(i * 10);
            }
            return v.size();
        };

        BENCHMARK("std::vector resize") {
            std::vector<int> v;
            for (size_t i = 0; i < 100; ++i) {
                v.resize(i * 10);
            }
            return v.size();
        };
    }

    SECTION("Reserve operations") {
        BENCHMARK("j::vector reserve") {
            j::vector<int> v;
            for (size_t i = 1; i <= 100; ++i) {
                v.reserve(i * 10);
                v.push_back(static_cast<int>(i));
            }
            return v.capacity();
        };

        BENCHMARK("std::vector reserve") {
            std::vector<int> v;
            for (size_t i = 1; i <= 100; ++i) {
                v.reserve(i * 10);
                v.push_back(static_cast<int>(i));
            }
            return v.capacity();
        };
    }

    SECTION("Shrink to fit") {
        BENCHMARK("j::vector shrink_to_fit") {
            j::vector<int> v;
            v.reserve(N);
            for (size_t i = 0; i < N / 2; ++i) {
                v.push_back(static_cast<int>(i));
            }
            v.shrink_to_fit();
            return v.capacity();
        };

        BENCHMARK("std::vector shrink_to_fit") {
            std::vector<int> v;
            v.reserve(N);
            for (size_t i = 0; i < N / 2; ++i) {
                v.push_back(static_cast<int>(i));
            }
            v.shrink_to_fit();
            return v.capacity();
        };
    }
}

TEST_CASE("Vector Benchmarks: Memory Operations") {
    SECTION("Construction and destruction") {
        BENCHMARK("j::vector construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                j::vector<int> v;
                for (size_t j = 0; j < N / 100; ++j) v.push_back(static_cast<int>(j));
            }
            return 1;
        };

        BENCHMARK("std::vector construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                std::vector<int> v;
                for (size_t j = 0; j < N / 100; ++j) v.push_back(static_cast<int>(j));
            }
            return 1;
        };
    }

    SECTION("Copy operations") {
        BENCHMARK("j::vector copy") {
            j::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            j::vector<int> copy = original;
            return copy.size();
        };

        BENCHMARK("std::vector copy") {
            std::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            std::vector<int> copy = original;
            return copy.size();
        };
    }

    SECTION("Move operations") {
        BENCHMARK("j::vector move") {
            j::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            j::vector<int> moved = std::move(original);
            return moved.size();
        };

        BENCHMARK("std::vector move") {
            std::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            std::vector<int> moved = std::move(original);
            return moved.size();
        };
    }

    SECTION("Assignment operations") {
        BENCHMARK("j::vector copy assignment") {
            j::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            j::vector<int> target;
            target = original;
            return target.size();
        };

        BENCHMARK("std::vector copy assignment") {
            std::vector<int> original;
            for (size_t i = 0; i < N; ++i) original.push_back(static_cast<int>(i));
            std::vector<int> target;
            target = original;
            return target.size();
        };
    }
}

TEST_CASE("Vector Benchmarks: Algorithm Integration") {
    SECTION("std::sort") {
        BENCHMARK("j::vector std::sort") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::sort(v.begin(), v.end());
            return v.size();
        };

        BENCHMARK("std::vector std::sort") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(dis(gen));
            }
            std::sort(v.begin(), v.end());
            return v.size();
        };
    }

    SECTION("std::find") {
        BENCHMARK("j::vector std::find") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(v.begin(), v.end(), target);
                if (it != v.end()) ++found_count;
            }
            return found_count;
        };

        BENCHMARK("std::vector std::find") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            size_t found_count = 0;
            for (size_t i = 0; i < 100; ++i) {
                auto target = dis(gen) % N;
                auto it = std::find(v.begin(), v.end(), target);
                if (it != v.end()) ++found_count;
            }
            return found_count;
        };
    }

    SECTION("std::accumulate") {
        BENCHMARK("j::vector std::accumulate") {
            j::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return std::accumulate(v.begin(), v.end(), 0);
        };

        BENCHMARK("std::vector std::accumulate") {
            std::vector<int> v;
            for (size_t i = 0; i < N; ++i) {
                v.push_back(static_cast<int>(i));
            }
            return std::accumulate(v.begin(), v.end(), 0);
        };
    }
}
