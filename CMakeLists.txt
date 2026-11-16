cmake_minimum_required(VERSION 3.20)
project(kow_engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(KOW_BUILD_TESTS "Build unit tests" ON)
option(KOW_BUILD_SANDBOX "Build sandbox application" ON)
option(KOW_ENABLE_VULKAN "Enable Vulkan renderer" OFF)
option(KOW_ENABLE_PHYSICS "Enable physics backend" OFF)
option(KOW_ENABLE_AUDIO "Enable audio backend" OFF)
option(KOW_ENABLE_SCRIPTING "Enable Lua scripting" OFF)

# Engine library
set(KOW_ENGINE_SOURCES
    engine/core/application/application.cpp
    engine/core/lifecycle/engine_lifecycle.cpp
    engine/core/logging/logger.cpp
    engine/core/configuration/config.cpp
    engine/core/error_handling/result.cpp
    engine/core/types/version.cpp
    engine/core/modules/module_registry.cpp
    engine/core/filesystem/filesystem.cpp
    engine/core/time/timer.cpp
    engine/ecs/entity_manager.cpp
    engine/ecs/component_storage.cpp
    engine/scene/scene.cpp
    engine/scene/scene_manager.cpp
    engine/scene/transform.cpp
    engine/graphics/renderer_interface.cpp
    engine/graphics/resource_handle.cpp
    engine/graphics/graphics_types.cpp
    engine/input/input_abstraction.cpp
    engine/assets/asset_manager.cpp
    engine/assets/asset_handle.cpp
    engine/physics/physics_world.cpp
    engine/audio/audio_system.cpp
    engine/scripting/scripting_interface.cpp
    engine/world/world_manager.cpp
    engine/world/sector.cpp
    engine/platform/platform.cpp
    engine/platform/window.cpp
    engine/platform/filesystem.cpp
    engine/diagnostics/profiler.cpp
    engine/memory/allocator.cpp
    engine/memory/memory_stats.cpp
)

set(KOW_ENGINE_HEADERS
    engine/core/application/application.h
    engine/core/lifecycle/engine_lifecycle.h
    engine/core/logging/logger.h
    engine/core/configuration/config.h
    engine/core/error_handling/result.h
    engine/core/types/version.h
    engine/core/modules/module_registry.h
    engine/core/filesystem/filesystem.h
    engine/core/time/timer.h
    engine/ecs/entity_manager.h
    engine/ecs/component_storage.h
    engine/scene/scene.h
    engine/scene/scene_manager.h
    engine/scene/transform.h
    engine/graphics/renderer_interface.h
    engine/graphics/resource_handle.h
    engine/graphics/graphics_types.h
    engine/input/input_abstraction.h
    engine/assets/asset_manager.h
    engine/assets/asset_handle.h
    engine/physics/physics_world.h
    engine/audio/audio_system.h
    engine/scripting/scripting_interface.h
    engine/world/world_manager.h
    engine/world/sector.h
    engine/platform/platform.h
    engine/platform/window.h
    engine/platform/filesystem.h
    engine/diagnostics/profiler.h
    engine/memory/allocator.h
    engine/memory/memory_stats.h
)

add_library(kow_engine STATIC ${KOW_ENGINE_SOURCES} ${KOW_ENGINE_HEADERS})
target_include_directories(kow_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

if(KOW_BUILD_TESTS)
    enable_testing()
    
    add_executable(kow_tests
        tests/unit/test_main.cpp
    )
    target_link_libraries(kow_tests PRIVATE kow_engine)
    target_include_directories(kow_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    
    # Try to find Google Test
    find_package(GTest QUIET)
    if(GTest_FOUND)
        target_link_libraries(kow_tests PRIVATE GTest::GTest GTest::Main)
        include(GoogleTest)
        gtest_discover_tests(kow_tests)
    else()
        # Use bundled Google Test
        message(STATUS "Google Test not found, using bundled version")
        # Download Google Test
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/v1.14.0.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
        target_link_libraries(kow_tests PRIVATE gtest gtest_main)
        include(GoogleTest)
        gtest_discover_tests(kow_tests)
    endif()
endif()

if(KOW_BUILD_SANDBOX)
    add_executable(kow_sandbox
        tools/sandbox/main.cpp
        tools/sandbox/application.cpp
    )
    target_link_libraries(kow_sandbox PRIVATE kow_engine)
    target_include_directories(kow_sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/sandbox)
endif()