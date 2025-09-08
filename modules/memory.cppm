/*
 * @ Created by jaehyung409 on 25. 9. 3..
 * @ Copyright (c) 2025 jaehyung409 
 * This software is licensed under the MIT License. 
 */

module;
#include <type_traits>
#include <iterator>
#include <memory>
#include <cstring>

#if defined(__clang__)
export module j:memory;
#else
module j:memory;
#endif

namespace j {
template <class Alloc, class Iter, class Size>
requires std::forward_iterator<Iter>
Iter  uninitialized_default_construct_n(Alloc alloc, Iter first, Size count) {
    if (count == 0) return first;

    using ValueType = typename std::iterator_traits<Iter>::value_type;

    if constexpr (std::is_trivially_default_constructible_v<ValueType>) {
        std::advance(first, count);
        return first;
    } else {
        Iter current = first;
        try {
            for (; count > 0; --count, ++current) {
                std::allocator_traits<Alloc>::construct(alloc, std::to_address(current));
            }
        }
        catch (...) {
            for (Iter it = first; it != current; ++it) {
                std::allocator_traits<Alloc>::destroy(alloc, std::to_address(it));
            }
            throw;
        }
        return current;
    }
}

template <class Alloc, class Iter, class Size>
requires std::forward_iterator<Iter>
Iter  uninitialized_value_construct_n(Alloc alloc, Iter first, Size count) {
    if (count == 0) return first;

    using ValueType = typename std::iterator_traits<Iter>::value_type;

    if constexpr (std::is_trivially_default_constructible_v<ValueType>) {
        std::memset(std::to_address(first), 0, count * sizeof(ValueType));
        return first + count;
    } else {
        Iter current = first;
        try {
            for (; count > 0; --count, ++current) {
                std::allocator_traits<Alloc>::construct(alloc, std::to_address(current));
            }
        }
        catch (...) {
            for (Iter it = first; it != current; ++it) {
                std::allocator_traits<Alloc>::destroy(alloc, std::to_address(it));
            }
            throw;
        }
        return current;
    }
}


template <class Allocator, std::contiguous_iterator Iter, class Size, class T>
Iter uninitialized_fill_n_contiguous(Allocator& alloc, Iter first, Size count, const T &value) {
    if (count == 0) return first;

    using ValueType = typename std::iterator_traits<Iter>::value_type;

    if constexpr (
        std::is_same_v<ValueType, T> &&
        std::is_trivially_constructible_v<ValueType> &&
        std::is_trivially_copyable_v<ValueType> &&
        std::is_standard_layout_v<ValueType>
    ) {
        if (value == ValueType{}) {
            std::memset(std::to_address(first), 0, count * sizeof(ValueType));
            return first + count;
        }
    }
    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        for (Size i = 0; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc, std::to_address(first + i), value);
        }
        return first + count;
    }
    Iter current = first;
    try {
        for (; count > 0; --count, ++current) {
            std::allocator_traits<Allocator>::construct(alloc, std::to_address(current), value);
        }
    }
    catch (...) {
        for (Iter it = first; it != current; ++it) {
            std::allocator_traits<Allocator>::destroy(alloc, std::to_address(it));
        }
        throw;
    }
    return current;
}

template <class Allocator, std::contiguous_iterator InputIt, class Size, std::contiguous_iterator OutputIt>
    OutputIt uninitialized_move_n_contiguous(Allocator alloc, InputIt first, Size count, OutputIt dest) {
    if (count == 0) return dest;

    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        std::memmove(std::to_address(dest), std::to_address(first), count * sizeof(ValueType));
    }
    else {
        OutputIt current = dest;
        try {
            for (Size i = 0; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc, std::addressof(*current), std::move(*(first + i)));
                ++current;
            }
        } catch (...) {
            for (; dest != current; ++dest) {
                std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*dest));
            }
            throw;
        }
    }

    return dest + count;
}

template <std::contiguous_iterator InputIt, class Size, std::contiguous_iterator OutputIt>
OutputIt move_n_contiguous(InputIt first, Size count, OutputIt dest) {
    if (count == 0) return dest;

    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        std::memmove(std::to_address(dest), std::to_address(first), count * sizeof(ValueType));
    }
    else {
        for (Size i = 0; i < count; ++i) {
            *(dest + i) = std::move(*(first + i));
        }
    }

    return dest + count;
}

template <std::contiguous_iterator InputIt, class Size, std::contiguous_iterator OutputIt>
OutputIt move_backward_n_contiguous(InputIt first, Size count, OutputIt dest) {
    if (count == 0) return dest;

    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        std::memmove(std::to_address(dest - count), std::to_address(first), count * sizeof(ValueType));
    }
    else {
        InputIt src = first + count;
        for (Size i = 1; i < count + 1; ++i) {
            *(dest - i) = std::move(*(first + count - i));
        }
    }

    return dest - count;
}

template <class Allocator, std::contiguous_iterator InputIt, class Size, std::contiguous_iterator OutputIt>
    OutputIt uninitialized_copy_n_contiguous(Allocator alloc, InputIt first, Size count, OutputIt dest) {
    if (count == 0) return dest;

    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        std::memcpy(std::to_address(dest), std::to_address(first), count * sizeof(ValueType));
    }
    else {
        OutputIt current = dest;
        try {
            for (Size i = 0; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc, std::addressof(*current), *(first + i));
                ++current;
            }
        } catch (...) {
            for (; dest != current; ++dest) {
                std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*dest));
            }
            throw;
        }
    }

    return dest + count;
}

template <std::contiguous_iterator InputIt, class Size, std::contiguous_iterator OutputIt>
OutputIt copy_n_contiguous(InputIt first, Size count, OutputIt dest) {
    if (count == 0) return dest;

    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    if constexpr (std::is_trivially_copyable_v<ValueType>) {
        std::memcpy(std::to_address(dest), std::to_address(first), count * sizeof(ValueType));
    }
    else {
        for (Size i = 0; i < count; ++i) {
            *(dest + i) = *(first + i);
        }
    }

    return dest + count;
}

}