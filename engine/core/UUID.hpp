#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <functional>

namespace kow::core {

template <typename Tag, typename T = std::uint32_t>
struct TypedHandle {
    T index{std::numeric_limits<T>::max()};
    std::uint32_t generation{0};
    [[nodiscard]] bool valid() const { return generation != 0 && index != std::numeric_limits<T>::max(); }
    bool operator==(const TypedHandle& o) const { return index==o.index && generation==o.generation; }
    bool operator!=(const TypedHandle& o) const { return !(*this==o); }
};

struct UUID {
    std::uint64_t high{0};
    std::uint64_t low{0};
    bool operator==(const UUID& o) const { return high==o.high && low==o.low; }
    [[nodiscard]] bool is_nil() const { return high==0 && low==0; }
    [[nodiscard]] std::string to_string() const; // minimal hex formatting
};

inline std::uint64_t fnv1a64(const std::string& s) {
    std::uint64_t h = 1469598103934665603ull;
    for (unsigned char c : s) { h ^= c; h *= 1099511628211ull; }
    return h;
}

inline UUID make_name_uuid(const std::string& name) {
    return UUID{fnv1a64(name), fnv1a64("kow:" + name)};
}

} // namespace kow::core
