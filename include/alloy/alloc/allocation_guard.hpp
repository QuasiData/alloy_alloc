#pragma once

#include "alloy/alloc/allocator.hpp"

#include <cstddef>
#include <new>
#include <utility>

namespace alloy::alloc {
class allocation_guard {
public:
    allocation_guard(allocator alloc, std::size_t bytes, std::size_t alignment)
        : alloc_(alloc)
        , bytes_(bytes)
        , alignment_(alignment)
        , ptr_(alloc_.allocate(bytes, alignment)) {
        if (!ptr_) [[unlikely]] {
            throw std::bad_alloc{};
        }
    }

    allocation_guard(const allocation_guard&) = delete;
    
    allocation_guard(allocation_guard&&) = delete;

    auto operator=(const allocation_guard&) -> allocation_guard& = delete;

    auto operator=(allocation_guard&&) -> allocation_guard& = delete;

    ~allocation_guard() noexcept {
        if (ptr_) {
            alloc_.deallocate(ptr_, bytes_, alignment_);
        }
    }

    [[nodiscard]] auto get() const noexcept -> void* {
        return ptr_;
    }

    [[nodiscard]] auto release() noexcept -> void* {
        return std::exchange(ptr_, nullptr);
    }

private:
    allocator alloc_;
    std::size_t bytes_;
    std::size_t alignment_;
    void* ptr_;
};
} // namespace alloy::alloc