#include "alloy/alloc/allocator.hpp"

#include <new>

namespace alloy::alloc {
[[nodiscard]] void* general_purpose_allocator::allocate(size_t size, size_t alignment) noexcept {
    if (size == 0) {
        return nullptr;
    }

    return ::operator new(size, std::align_val_t{alignment}, std::nothrow);
}

void general_purpose_allocator::deallocate(void* ptr, size_t /*size*/, size_t alignment) noexcept {
    ::operator delete(ptr, std::align_val_t{alignment}, std::nothrow);
}

[[nodiscard]] bool general_purpose_allocator::is_equal(const allocator_backend& other) const noexcept {
    return dynamic_cast<const general_purpose_allocator*>(&other) != nullptr;
}

auto default_allocator_backend() noexcept -> allocator_backend& {
    static general_purpose_allocator instance;
    return instance;
}
} // namespace alloy::alloc