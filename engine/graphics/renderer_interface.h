#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "core/error_handling/result.h"
#include "graphics/resource_handle.h"

namespace kow {

struct MeshData {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    uint32_t vertex_stride = 0;
    uint32_t vertex_count = 0;
    uint32_t index_count = 0;
};

struct MaterialData {
    std::string name;
    float base_color[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    float metallic = 0.0f;
    float roughness = 0.5f;
    float specular = 0.5f;
    ResourceHandle base_color_texture;
    ResourceHandle normal_texture;
    ResourceHandle metallic_roughness_texture;
};

struct Model {
    std::string name;
    std::vectorMeshData> meshes;
    std::vector<MaterialData> materials;
    ResourceHandle handle;
};

struct TextureData {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    std::vector<uint8_t> data;
    ResourceHandle handle;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;
    virtual void present() = 0;

    virtual void clear(float r, float g, float b, float a) = 0;
    virtual void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

    virtual void draw_mesh(const MeshData& mesh, const MaterialData& material,
                           const Mat4& model_matrix) = 0;
    virtual void draw_mesh_instanced(const MeshData& mesh, const MaterialData& material,
                                     const std::vector<Mat4>& instances) = 0;

    virtual void set_camera(const Vec3& position, const Vec3& target, 
                            float fov, float aspect) = 0;
    virtual void set_ambient_light(const Vec3& color, float intensity) = 0;
    virtual void add_directional_light(const Vec3& direction, const Vec3& color, float intensity) = 0;

    virtual ResourceHandle create_texture(const TextureData& data) = 0;
    virtual void destroy_texture(ResourceHandle handle) = 0;

    virtual uint32_t get_frame_count() const = 0;
    virtual float get_fps() const = 0;
    virtual bool is_initialized() const = 0;
};

class RendererAdapter : public IRenderer {
public:
    using InitFn = std::function<bool()>;
    using DrawMeshFn = std::function<void(const MeshData&, const MaterialData&, const Mat4&)>;

    void set_on_initialize(InitFn fn) { on_initialize_ = std::move(fn); }
    void set_on_begin_frame(std::function<void()> fn) { on_begin_frame_ = std::move(fn); }
    void set_on_end_frame(std::function<void()> fn) { on_end_frame_ = std::move(fn); }
    void set_on_present(std::function<void()> fn) { on_present_ = std::move(fn); }
    void set_on_draw_mesh(DrawMeshFn fn) { on_draw_mesh_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return false;
    }

    void shutdown() override {}
    void begin_frame() override { if (on_begin_frame_) on_begin_frame_(); }
    void end_frame() override { if (on_end_frame_) on_end_frame_(); }
    void present() override { if (on_present_) on_present_(); }

    void clear(float, float, float, float) override {}
    void set_viewport(uint32_t, uint32_t, uint32_t, uint32_t) override {}

    void draw_mesh(const MeshData& mesh, const MaterialData& material,
                   const Mat4& model_matrix) override {
        if (on_draw_mesh_) on_draw_mesh_(mesh, material, model_matrix);
    }

    void draw_mesh_instanced(const MeshData& mesh, const MaterialData& material,
                             const std::vector<Mat4>& instances) override {
        for (const auto& instance : instances) {
            draw_mesh(mesh, material, instance);
        }
    }

    void set_camera(const Vec3&, const Vec3&, float, float) override {}
    void set_ambient_light(const Vec3&, float) override {}
    void add_directional_light(const Vec3&, const Vec3&, float) override {}

    ResourceHandle create_texture(const TextureData&) override { return ResourceHandle{0}; }
    void destroy_texture(ResourceHandle) override {}

    uint32_t get_frame_count() const override { return 0; }
    float get_fps() const override { return 0.0f; }
    bool is_initialized() const override { return false; }

private:
    InitFn on_initialize_;
    std::function<void()> on_begin_frame_;
    std::function<void()> on_end_frame_;
    std::function<void()> on_present_;
    DrawMeshFn on_draw_mesh_;
};

class IGraphicsDevice {
public:
    virtual ~IGraphicsDevice() = default;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual bool is_available() const = 0;
    virtual std::string_view name() const = 0;
};

class NullGraphicsDevice : public IGraphicsDevice {
public:
    bool initialize() override { return true; }
    void shutdown() override {}
    bool is_available() const override { return true; }
    std::string_view name() const override { return "Null Device"; }
};

} // namespace kow