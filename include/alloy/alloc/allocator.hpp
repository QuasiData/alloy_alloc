#pragma once

#include <cstddef>

namespace alloy::alloc {

class allocator_backend {
public:
    allocator_backend() noexcept = default;
    
    virtual ~allocator_backend() = default;

    allocator_backend(const allocator_backend&) = default;

    allocator_backend(allocator_backend&&) = default;

    auto operator=(const allocator_backend&) -> allocator_backend& = default;

    auto operator=(allocator_backend&&) -> allocator_backend& = default;

    [[nodiscard]] virtual auto allocate(size_t size, size_t alignment) noexcept -> void* = 0;

    virtual void deallocate(void* ptr, size_t size, size_t alignment) noexcept = 0;

    [[nodiscard]] virtual auto is_equal(const allocator_backend& other) const noexcept -> bool = 0;
};

[[nodiscard]] inline auto operator==(const allocator_backend& lhs, const allocator_backend& rhs) noexcept -> bool {
    return &lhs == &rhs || lhs.is_equal(rhs);
}

[[nodiscard]] auto default_allocator_backend() noexcept -> allocator_backend&;

class allocator {
public:
    allocator() noexcept
        : backend_(&default_allocator_backend()) {}

    allocator(allocator_backend& backend) noexcept
        : backend_(&backend) {}

    [[nodiscard]] auto allocate(size_t size, size_t alignment) const noexcept -> void* {
        return backend_->allocate(size, alignment);
    }

    void deallocate(void* ptr, size_t size, size_t alignment) const noexcept {
        backend_->deallocate(ptr, size, alignment);
    }

    [[nodiscard]] auto backend() const noexcept -> allocator_backend& {
        return *backend_;
    }

    [[nodiscard]] friend auto operator==(allocator lhs, allocator rhs) noexcept -> bool {
        return *lhs.backend_ == *rhs.backend_;
    }

private:
    allocator_backend* backend_;
};

class general_purpose_allocator : public allocator_backend {
public:
    [[nodiscard]] auto allocate(size_t size, size_t alignment) noexcept -> void* override;

    void deallocate(void* ptr, size_t size, size_t alignment) noexcept override;

    [[nodiscard]] auto is_equal(const allocator_backend& other) const noexcept -> bool override;
};

[[nodiscard]] inline auto default_allocator() noexcept -> allocator {
    return allocator{};
}
} // namespace alloy::alloc