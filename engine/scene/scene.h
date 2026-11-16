#pragma once

#include "ecs/entity_manager.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace kow {

struct Transform {
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // quaternion
    float scale[3] = {1.0f, 1.0f, 1.0f};

    void set_position(float x, float y, float z) {
        position[0] = x; position[1] = y; position[2] = z;
    }

    void set_rotation(float x, float y, float z, float w) {
        rotation[0] = x; rotation[1] = y; rotation[2] = z; rotation[3] = w;
    }

    void set_scale(float x, float y, float z) {
        scale[0] = x; scale[1] = y; scale[2] = z;
    }
};

class Scene {
public:
    Scene(const std::string& name);
    ~Scene();

    const std::string& name() const { return name_; }

    EntityHandle create_entity();
    void destroy_entity(EntityHandle entity);

    template<typename T>
    void add_component(EntityHandle entity, T component) {
        manager_.add_component(entity, std::move(component));
    }

    template<typename T>
    T* get_component(EntityHandle entity) {
        return manager_.get_component<T>(entity);
    }

    template<typename T>
    bool has_component(EntityHandle entity) const {
        return manager_.has_component<T>(entity);
    }

    template<typename T>
    std::vector<EntityHandle> get_entities_with_component() {
        return manager_.get_entities_with_component<T>();
    }

    void clear();
    size_t entity_count() const;

    void set_active(bool active) { active_ = active; }
    bool is_active() const { return active_; }

private:
    std::string name_;
    EntityManager manager_;
    bool active_ = true;
};

class SceneListener {
public:
    virtual ~SceneListener() = default;
    virtual void on_scene_loaded(Scene* scene) {}
    virtual void on_scene_unloaded(Scene* scene) {}
    virtual void on_entity_added(Scene* scene, EntityHandle entity) {}
    virtual void on_entity_removed(Scene* scene, EntityHandle entity) {}
};

class SceneListenerAdapter : public SceneListener {
public:
    using LoadFn = std::function<void(Scene*)>;
    using EntityFn = std::function<void(Scene*, EntityHandle)>;

    void set_on_load(LoadFn fn) { on_load_ = std::move(fn); }
    void set_on_entity_added(EntityFn fn) { on_entity_added_ = std::move(fn); }
    void set_on_entity_removed(EntityFn fn) { on_entity_removed_ = std::move(fn); }

    void on_scene_loaded(Scene* scene) override {
        if (on_load_) on_load_(scene);
    }
    void on_entity_added(Scene* scene, EntityHandle entity) override {
        if (on_entity_added_) on_entity_added_(scene, entity);
    }
    void on_entity_removed(Scene* scene, EntityHandle entity) override {
        if (on_entity_removed_) on_entity_removed_(scene, entity);
    }

private:
    LoadFn on_load_;
    EntityFn on_entity_added_;
    EntityFn on_entity_removed_;
};

class SceneConfig {
public:
    std::string default_scene_path = "scenes/default.scene";
    bool auto_create_default = true;
    bool enable_streaming = false;
};

class ISceneLoader {
public:
    virtual ~ISceneLoader() = default;
    virtual Result<std::unique_ptr<Scene>> load_scene(const std::filesystem::path& path) = 0;
    virtual void unload_scene(Scene* scene) = 0;
};

class SceneLoaderAdapter : public ISceneLoader {
public:
    using LoadFn = std::function<Result<std::unique_ptr<Scene>>(const std::filesystem::path&)>;

    void set_on_load(LoadFn fn) { on_load_ = std::move(fn); }

    Result<std::unique_ptr<Scene>> load_scene(const std::filesystem::path& path) override {
        if (on_load_) return on_load_(path);
        return failure(ErrorCode::NotSupported, "No scene loader configured");
    }

    void unload_scene(Scene* scene) override {
        // Default: destroy scene
        delete scene;
    }

private:
    LoadFn on_load_;
};

class ScenePipeline {
public:
    void add_listener(std::unique_ptr<SceneListener> listener) {
        listeners_.push_back(std::move(listener));
    }

    void on_scene_loaded(Scene* scene) {
        for (auto& listener : listeners_) {
            listener->on_scene_loaded(scene);
        }
    }

    void on_entity_added(Scene* scene, EntityHandle entity) {
        for (auto& listener : listeners_) {
            listener->on_entity_added(scene, entity);
        }
    }

    void on_entity_removed(Scene* scene, EntityHandle entity) {
        for (auto& listener : listeners_) {
            listener->on_entity_removed(scene, entity);
        }
    }

private:
    std::vector<std::unique_ptr<SceneListener>> listeners_;
};

class SceneDataManager {
public:
    static SceneDataManager& instance();

    void save_scene(Scene* scene, const std::filesystem::path& path);
    Result<std::unique_ptr<Scene>> load_scene(const std::filesystem::path& path);

private:
    SceneDataManager() = default;
};

} // namespace kow