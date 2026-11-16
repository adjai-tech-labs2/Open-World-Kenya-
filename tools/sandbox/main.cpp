#include "core/application/application.h"
#include "core/lifecycle/engine_lifecycle.h"
#include "core/logging/logger.h"
#include "core/time/timer.h"
#include "core/configuration/config.h"
#include "core/filesystem/filesystem.h"
#include "core/modules/module_registry.h"
#include "core/memory/allocator.h"
#include "core/diagnostics/profiler.h"
#include "core/types/version.h"
#include "ecs/entity_manager.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "graphics/renderer_interface.h"
#include "graphics/resource_handle.h"
#include "input/input_abstraction.h"
#include "assets/asset_manager.h"
#include "physics/physics_world.h"
#include "audio/audio_system.h"
#include "scripting/scripting_interface.h"
#include "world/world_manager.h"
#include "platform/platform.h"
#include "platform/window.h"
#include "platform/filesystem.h"
#include "diagnostics/profiler.h"
#include "memory/allocator.h"
#include "memory/memory_stats.h"
#include <iostream>

int main() {
    using namespace kow;

    std::cout << "========================================" << std::endl;
    std::cout << "  KOW Engine - Sandbox Application" << std::endl;
    std::cout << "  Version: " << version_to_string(get_engine_version()) << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Initialize engine
    KOW_LOG_INFO("sandbox", "Initializing KOW Engine...");
    auto& engine = Engine::instance();
    if (!engine.initialize()) {
        std::cerr << "ERROR: Engine initialization failed!" << std::endl;
        return 1;
    }

    // Print diagnostics
    KOW_LOG_INFO("sandbox", "=== Engine Diagnostics ===");
    KOW_LOG_INFO("sandbox", "Version: {}", version_to_string(engine.version()));
    KOW_LOG_INFO("sandbox", "Modules registered: {}", ModuleRegistry::instance().module_count());

    // Create a test scene
    KOW_LOG_INFO("sandbox", "Creating test scene...");
    auto scene = std::make_unique<Scene>("TestScene");
    
    // Create some entities
    KOW_LOG_INFO("sandbox", "Creating entities...");
    EntityHandle entity1 = scene->create_entity();
    EntityHandle entity2 = scene->create_entity();
    EntityHandle entity3 = scene->create_entity();
    
    KOW_LOG_INFO("sandbox", "Created {} entities", scene->entity_count());
    
    // Add components
    Transform transform1;
    transform1.set_position(0.0f, 0.0f, 0.0f);
    scene->add_component(entity1, transform1);
    
    Transform transform2;
    transform2.set_position(10.0f, 0.0f, 0.0f);
    scene->add_component(entity2, transform2);
    
    // Verify components
    Transform* t1 = scene->get_component<Transform>(entity1);
    if (t1) {
        KOW_LOG_INFO("sandbox", "Entity 1 position: ({}, {}, {})", 
            t1->position[0], t1->position[1], t1->position[2]);
    }

    // Test entity destruction
    KOW_LOG_INFO("sandbox", "Destroying entity 3...");
    scene->destroy_entity(entity3);
    KOW_LOG_INFO("sandbox", "Remaining entities: {}", scene->entity_count());

    // Test configuration
    KOW_LOG_INFO("sandbox", "=== Configuration ===");
    KOW_LOG_INFO("sandbox", "Window width: {}", Config::instance().get_int("engine.window_width"));
    KOW_LOG_INFO("sandbox", "Max FPS: {}", Config::instance().get_int("engine.max_fps"));

    // Test memory tracking
    KOW_LOG_INFO("sandbox", "=== Memory Stats ===");
    void* test_alloc = KOW_ALLOC(1024, MemoryPool::Game, "TestAllocation");
    KOW_LOG_INFO("sandbox", "Allocated 1024 bytes for testing");
    KOW_FREE(test_alloc, 1024);

    // Test profiler
    KOW_LOG_INFO("sandbox", "=== Profiler ===");
    {
        KOW_PROFILE("TestOperation");
        // Simulate work
        for (int i = 0; i < 1000000; ++i) {
            volatile int x = i * 2;
            (void)x;
        }
    }
    
    auto stats = Profiler::instance().get_stats();
    for (const auto& stat : stats) {
        KOW_LOG_INFO("sandbox", "  {}: {} calls, {:.3f}ms avg", 
            stat.name, stat.call_count, stat.avg_time_ms);
    }

    // Test filesystem
    KOW_LOG_INFO("sandbox", "=== Filesystem ===");
    Filesystem& fs = Filesystem::instance();
    KOW_LOG_INFO("sandbox", "Root path: {}", fs.root().string());

    // Test world manager
    KOW_LOG_INFO("sandbox", "=== World Manager ===");
    WorldManager& world = WorldManager::instance();
    world.initialize();
    world.set_player_position(Vec3{0, 0, 0});
    KOW_LOG_INFO("sandbox", "Loaded sectors: {}", world.loaded_sector_count());

    // Test asset manager
    KOW_LOG_INFO("sandbox", "=== Asset Manager ===");
    AssetManager& assets = AssetManager::instance();
    KOW_LOG_INFO("sandbox", "Cache size: {}", assets.cache_size());

    // Test input system
    KOW_LOG_INFO("sandbox", "=== Input System ===");
    InputManager& input = InputManager::instance();
    input.initialize();

    // Test audio system
    KOW_LOG_INFO("sandbox", "=== Audio System ===");
    AudioManager& audio = AudioManager::instance();
    audio.initialize();

    // Test physics
    KOW_LOG_INFO("sandbox", "=== Physics ===");
    PhysicsManager& physics = PhysicsManager::instance();
    physics.initialize();

    // Test scripting
    KOW_LOG_INFO("sandbox", "=== Scripting ===");
    ScriptingManager& scripting = ScriptingManager::instance();
    scripting.initialize();

    // Test rendering
    KOW_LOG_INFO("sandbox", "=== Renderer ===");
    RendererManager& renderer = RendererManager::instance();
    renderer.initialize();

    // Test platform
    KOW_LOG_INFO("sandbox", "=== Platform ===");
    PlatformManager& platform = PlatformManager::instance();
    platform.initialize();

    KOW_LOG_INFO("sandbox", "All systems initialized successfully!");

    // Shutdown
    KOW_LOG_INFO("sandbox", "Shutting down...");
    renderer.shutdown();
    scripting.shutdown();
    physics.shutdown();
    audio.shutdown();
    input.shutdown();
    world.shutdown();
    engine.shutdown();

    KOW_LOG_INFO("sandbox", "Sandbox test complete!");
    std::cout << "Sandbox test complete!" << std::endl;
    return 0;
}