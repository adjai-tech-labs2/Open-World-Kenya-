#include "core/Application.hpp"
#include "core/IModule.hpp"
#include "core/Logger.hpp"
#include "core/Result.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/ResourceHandles.hpp"
#include "assets/AssetManager.hpp"
#include "platform/Input.hpp"
#include "physics/PhysicsWorld.hpp"
#include "audio/AudioSystem.hpp"
#include "scripting/Scripting.hpp"
#include "world/WorldManager.hpp"
#include "scene/SceneManager.hpp"
#include "ecs/Registry.hpp"
#include "scene/Transform.hpp"
#include <iostream>
#include <memory>

using namespace kow;

class GraphicsModule final : public core::IModule {
public:
    std::string name() const override { return "Graphics (interface)"; }
    core::Result<void> initialize(core::Application& app) override {
        auto res = renderer_.initialize();
        if (!res.ok()) return res;
        app.logger().info("Renderer active: " + renderer_.name() + " [NO GPU BACKEND]");
        return core::Ok();
    }
    core::Result<void> shutdown(core::Application&) override { return renderer_.shutdown(); }
    bool is_initialized() const override { return renderer_.is_initialized(); }
    graphics::IRenderer& renderer() { return renderer_; }
private:
    graphics::NullRenderer renderer_;
};

class PhysicsModule final : public core::IModule {
public:
    std::string name() const override { return "Physics (interface)"; }
    core::Result<void> initialize(core::Application& app) override {
        physics::PhysicsConfig cfg;
        auto res = world_.initialize(cfg);
        if (res.ok()) app.logger().info("Physics: " + world_.name() + " [NO SIMULATION BACKEND]");
        return res;
    }
    core::Result<void> shutdown(core::Application&) override { return world_.shutdown(); }
    bool is_initialized() const override { return world_.is_initialized(); }
private:
    physics::NullPhysicsWorld world_;
};

class AudioModule final : public core::IModule {
public:
    std::string name() const override { return "Audio (interface)"; }
    core::Result<void> initialize(core::Application& app) override {
        auto res = audio_.initialize();
        if (res.ok()) app.logger().info("Audio: " + audio_.name() + " [NO AUDIO BACKEND]");
        return res;
    }
    core::Result<void> shutdown(core::Application&) override { return audio_.shutdown(); }
    bool is_initialized() const override { return audio_.is_initialized(); }
private:
    audio::NullAudioSystem audio_;
};

class ScriptingModule final : public core::IModule {
public:
    std::string name() const override { return "Scripting (interface)"; }
    core::Result<void> initialize(core::Application& app) override {
        auto res = runtime_.initialize();
        if (res.ok()) app.logger().info("Scripting: " + runtime_.name() + " [NO VM BACKEND]");
        return res;
    }
    core::Result<void> shutdown(core::Application&) override { return runtime_.shutdown(); }
    bool is_initialized() const override { return runtime_.is_initialized(); }
private:
    scripting::NullScriptingRuntime runtime_;
};

class WorldModule final : public core::IModule {
public:
    std::string name() const override { return "World/Sector Management"; }
    core::Result<void> initialize(core::Application& app) override {
        world::Sector nairobi_cbd{{0,0}, "Nairobi_CBD_Prototype", world::SectorState::Registered};
        auto r = manager_.register_sector(nairobi_cbd);
        if (!r.ok()) return r;
        manager_.set_sector_state({0,0}, world::SectorState::Loaded);
        app.logger().info("World: " + manager_.name() + " sectors=" + std::to_string(manager_.sector_count()));
        app.logger().info("Registered logical sector: Nairobi_CBD_Prototype (data-defined placeholder, no streamed geometry)");
        initialized_ = true;
        return core::Ok();
    }
    core::Result<void> shutdown(core::Application&) override { initialized_ = false; return core::Ok(); }
    bool is_initialized() const override { return initialized_; }
private:
    world::WorldManager manager_;
    bool initialized_{false};
};

class SandboxApp final : public core::Application {
public:
    using Application::Application;
    void on_tick() override {
        // bootstrap tick: print diagnostics once
        if (!printed_) {
            logger().info("--- KOW Sandbox Diagnostics ---");
            logger().info("App: " + config().app_name);
            logger().info("Headless: " + std::string(config().headless ? "true" : "false"));
            logger().info("Modules registered: " + std::to_string(modules().modules().size()));
            logger().info("Frame timing active (dt=" + std::to_string(timer().delta_seconds()) + "s)");
            logger().info("Backends: renderer/physics/audio/scripting = interface stubs (no real backends)");
            logger().info("World/city/roads/vehicles/people: NOT built as runtime assets (toolchain/data/backend constraints) - see limitations");
            printed_ = true;
        }
    }
private:
    bool printed_{false};
};

int main() {
    core::EngineConfig cfg;
    cfg.app_name = "kow_sandbox";
    cfg.log_level = core::LogLevel::Info;
    cfg.headless = true;
    cfg.max_frames = 1;

    SandboxApp app(cfg);

    // Foundational interfaces/modules
    app.modules().add(std::make_unique<GraphicsModule>());
    app.modules().add(std::make_unique<PhysicsModule>());
    app.modules().add(std::make_unique<AudioModule>());
    app.modules().add(std::make_unique<ScriptingModule>());
    app.modules().add(std::make_unique<WorldModule>());

    // Demonstrate ECS + Scene + Transform + Asset/Input interfaces in-process
    scene::SceneManager scene_mgr;
    auto scene_res = scene_mgr.create_scene("bootstrap");
    if (!scene_res.ok()) {
        std::cerr << "Scene creation failed: " << scene_res.error().message << std::endl;
        return 1;
    }
    auto* scene = scene_res.value();
    auto e = scene->registry().create();
    scene->registry().add<scene::Transform>(e, scene::Transform{{0,0,0},{0,0,0,1},{1,1,1}});
    assets::NullAssetManager assets;
    (void)assets.register_asset("map/nairobi/osm_placeholder", assets::AssetType::MapData);
    platform::NullInput input;

    std::cout << "KOW Engine " << core::kEngineVersion.to_string() << " (C++20, namespace kow)" << std::endl;
    std::cout << "Scene: " << scene->name() << " entities_alive=" << scene->registry().alive_count() << std::endl;
    std::cout << "Transform component present=" << (scene->registry().has<scene::Transform>(e) ? "true" : "false") << std::endl;
    std::cout << "Asset manager: " << assets.name() << " registered=" << assets.registered_count() << std::endl;
    std::cout << "Input: " << input.name() << std::endl;

    auto init = app.initialize();
    if (!init.ok()) {
        std::cerr << "Initialization failed [" << init.error().code << "]: " << init.error().message << std::endl;
        return 2;
    }
    auto run = app.run();
    if (!run.ok()) {
        std::cerr << "Run failed [" << run.error().code << "]: " << run.error().message << std::endl;
        (void)app.shutdown();
        return 3;
    }
    auto shut = app.shutdown();
    if (!shut.ok()) {
        std::cerr << "Shutdown reported errors [" << shut.error().code << "]: " << shut.error().message << std::endl;
        return 4;
    }
    std::cout << "kow_sandbox: clean shutdown OK" << std::endl;
    return 0;
}
