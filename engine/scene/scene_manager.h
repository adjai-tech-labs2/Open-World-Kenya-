#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "scene/scene.h"
#include "core/error_handling/result.h"

namespace kow {

class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene);
    std::unique_ptr<Scene> pop();
    void pop_all();
    void clear();

    Scene* top() { return stack_.empty() ? nullptr : stack_.back().get(); }
    const Scene* top() const { return stack_.empty() ? nullptr : stack_.back().get(); }
    bool empty() const { return stack_.empty(); }
    size_t size() const { return stack_.size(); }

    void update(float delta_time);

private:
    std::vector<std::unique_ptr<Scene>> stack_;
};

class SceneStackListener {
public:
    virtual ~SceneStackListener() = default;
    virtual void on_scene_pushed(Scene* scene) = 0;
    virtual void on_scene_popped(Scene* scene) = 0;
};

class SceneStackAdapter : public SceneStackListener {
public:
    using Fn = std::function<void(Scene*)>;
    void set_on_pushed(Fn fn) { on_pushed_ = std::move(fn); }
    void set_on_popped(Fn fn) { on_popped_ = std::move(fn); }

    void on_scene_pushed(Scene* scene) override {
        if (on_pushed_) on_pushed_(scene);
    }
    void on_scene_popped(Scene* scene) override {
        if (on_popped_) on_popped_(scene);
    }

private:
    Fn on_pushed_;
    Fn on_popped_;
};

class ISceneFactory {
public:
    virtual ~ISceneFactory() = default;
    virtual std::unique_ptr<Scene> create_scene(const std::string& name) = 0;
    virtual std::string_view scene_name() const = 0;
};

template<typename T>
class SceneFactory : public ISceneFactory {
public:
    explicit SceneFactory(std::string name) : name_(std::move(name)) {}

    std::unique_ptr<Scene> create_scene(const std::string& name) override {
        return std::make_unique<T>(name);
    }

    std::string_view scene_name() const override { return name_; }

private:
    std::string name_;
};

class ISceneManager {
public:
    virtual ~ISceneManager() = default;
    virtual void push_scene(const std::string& name) = 0;
    virtual void pop_scene() = 0;
    virtual void pop_all_scenes() = 0;
    virtual void clear_scenes() = 0;
    virtual Scene* current_scene() = 0;
    virtual const Scene* current_scene() const = 0;
    virtual bool has_scene(const std::string& name) const = 0;
    virtual void update(float delta_time) = 0;
    virtual void register_factory(std::unique_ptr<ISceneFactory> factory) = 0;
};

class UserManager : public ISceneManager {
public:
    void push_scene(const std::string& name) override;
    void pop_scene() override;
    void pop_all_scenes() override;
    void clear_scenes() override;
    Scene* current_scene() override;
    const Scene* current_scene() const override;
    bool has_scene(const std::string& name) const override;
    void update(float delta_time) override;
    void register_factory(std::unique_ptr<ISceneFactory> factory) override;

    static UserManager& instance();

private:
    UserManager() = default;
    SceneStack stack_;
    std::unordered_map<std::string, std::unique_ptr<ISceneFactory>> factories_;
};

#define KOW_SCENE_MANAGER kow::UserManager::instance()

} // namespace kow