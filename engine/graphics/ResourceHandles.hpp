#pragma once
#include <cstdint>

namespace kow::graphics {

struct ResourceHandle {
    std::uint32_t index{0};
    std::uint32_t generation{0};
    [[nodiscard]] bool valid() const { return generation != 0; }
};

struct MeshHandle : ResourceHandle {};
struct TextureHandle : ResourceHandle {};
struct ShaderHandle : ResourceHandle {};
struct MaterialHandle : ResourceHandle {};

} // namespace kow::graphics
