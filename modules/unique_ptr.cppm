/*
 * @ Created by jaehyung409 on 25. 8. 30..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <memory>

#if defined(__clang__)
export module j:unique_ptr;
#else
module j:unique_ptr;
#endif

namespace j {
template <class T, class Allocator> class unique_ptr {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using reference = value_type &;
    using size_type = std::size_t;

  private:
    pointer _ptr;
    allocator_type _alloc;
    size_type _size;

  public:
    explicit constexpr unique_ptr(pointer ptr, allocator_type &alloc, size_type size) noexcept
        : _ptr(ptr), _alloc(alloc), _size(size) {}

    ~unique_ptr() {
        if (_ptr) {
            std::allocator_traits<allocator_type>::deallocate(_alloc, _ptr, _size);
        }
    }

    unique_ptr(const unique_ptr &other) = delete;
    unique_ptr &operator=(const unique_ptr &other) = delete;

    unique_ptr(unique_ptr &&other) noexcept : _ptr(other._ptr), _alloc(std::move(other._alloc)), _size(other._size) {
        other._ptr = nullptr;
    }
    unique_ptr &operator=(unique_ptr &&other) noexcept {
        if (this != &other) {
            if (_ptr) {
                std::allocator_traits<allocator_type>::deallocate(_alloc, _ptr, _size);
            }
            _ptr = other._ptr;
            _alloc = std::move(other._alloc);
            _size = other._size;
            other._ptr = nullptr;
        }
        return *this;
    }

    pointer get() const noexcept {
        return _ptr;
    }
    reference operator*() const noexcept {
        return *_ptr;
    }
    pointer operator->() const noexcept {
        return _ptr;
    }

    pointer release() noexcept {
        pointer tmp = _ptr;
        _ptr = nullptr;
        return tmp;
    }
};
template <class Pointer, class Allocator>
unique_ptr(Pointer p, Allocator &alloc, std::size_t size)
    -> unique_ptr<typename std::pointer_traits<Pointer>::element_type, Allocator>;
} // namespace j