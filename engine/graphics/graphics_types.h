#pragma once

#include <cstdint>
#include <vector>
#include "graphics/resource_handle.h"

namespace kow {

enum class PrimitiveType : uint8_t {
    Points = 0,
    Lines = 1,
    LineStrip = 2,
    Triangles = 3,
    TriangleStrip = 4,
    TriangleFan = 5
};

enum class IndexType : uint8_t {
    Uint16 = 0,
    Uint32 = 1
};

enum class Format : uint8_t {
    R8 = 0,
    RG8 = 1,
    RGB8 = 2,
    RGBA8 = 3,
    R16 = 4,
    RG16 = 5,
    RGB16 = 6,
    RGBA16 = 7,
    R32 = 8,
    RG32 = 9,
    RGB32 = 10,
    RGBA32 = 11,
    Depth32 = 12,
    Depth24Stencil8 = 13
};

struct VertexAttribute {
    uint32_t location = 0;
    uint32_t binding = 0;
    Format format = Format::RGBA32;
    uint32_t offset = 0;
};

struct VertexBinding {
    uint32_t binding = 0;
    uint32_t stride = 0;
};

struct BufferUsage {
    uint32_t vertex_buffer = 0;
    uint32_t index_buffer = 0;
    uint32_t uniform_buffer = 0;
    uint32_t storage_buffer = 0;
};

struct DrawCommand {
    uint32_t vertex_count = 0;
    uint32_t instance_count = 1;
    uint32_t first_vertex = 0;
    uint32_t first_index = 0;
};

struct PipelineState {
    uint32_t cull_mode = 0; // 0:none, 1:back, 2:front
    uint32_t depth_test = 1;
    uint32_t depth_write = 1;
    uint32_t blend_mode = 0; // 0:none, 1:alpha, 2:additive
    uint32_t fill_mode = 2; // 0:wireframe, 2:solid
};

} // namespace kow