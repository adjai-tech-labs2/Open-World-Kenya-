#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include "core/error_handling/result.h"
#include "scene/transform.h"

namespace kow {

enum class ResourceState : uint8_t {
    Unloaded = 0,
    Loading = 1,
    Loaded = 2,
    Failed = 3
};

struct ResourceStats {
    uint64_t memory_usage = 0;
    uint32_t reference_count = 0;
    ResourceState state = ResourceState::Unloaded;
};

class ResourceHandle {
public:
    ResourceHandle() = default;
    explicit ResourceHandle(uint64_t id) : id_(id) {}

    uint64_t id() const { return id_; }
    bool is_valid() const { return id_ != 0; }
    bool operator==(const ResourceHandle& o) const { return id_ == o.id_; }
    bool operator!=(const ResourceHandle& o) const { return id_ != o.id_; }

private:
    uint64_t id_ = 0;
};

class IResource {
public:
    virtual ~IResource() = default;
    virtual ResourceHandle handle() const = 0;
    virtual const std::string& path() const = 0;
    virtual ResourceStats stats() const = 0;
    virtual VoidResult load() = 0;
    virtual void unload() = 0;
    virtual bool is_loaded() const = 0;
};

class IResourceLoader {
public:
    virtual ~IResourceLoader() = default;
    virtual bool can_load(const std::filesystem::path& path) const = 0;
    virtual Result<std::unique_ptr<IResource>> load(const std::filesystem::path& path) = 0;
    virtual std::string_view supported_extension() const = 0;
};

class ResourceManager {
public:
    static ResourceManager& instance();

    void add_loader(std::unique_ptr<IResourceLoader> loader);
    ResourceHandle load_resource(const std::filesystem::path& path);
    void unload_resource(ResourceHandle handle);
    void unload_all();

    IResource* get_resource(ResourceHandle handle);
    const IResource* get_resource(ResourceHandle handle) const;
    bool is_loaded(ResourceHandle handle) const;
    ResourceStats get_stats(ResourceHandle handle) const;

    void update();
    size_t resource_count() const;

private:
    ResourceManager();
    std::vector<std::unique_ptr<IResourceLoader>> loaders_;
    std::vector<std::unique_ptr<IResource>> resources_;
    std::unordered_map<uint64_t, ResourceHandle> path_to_handle_;
    uint64_t next_id_ = 1;
};

#define KOW_RESOURCE_MANAGER kow::ResourceManager::instance()

} // namespace kow