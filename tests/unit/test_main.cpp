#include <gtest/gtest.h>
#include "core/types/version.h"
#include "core/error_handling/result.h"
#include "core/configuration/config.h"
#include "core/filesystem/filesystem.h"
#include "core/time/timer.h"
#include "core/logging/logger.h"
#include "ecs/entity_manager.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"

using namespace kow;

// Version tests
TEST(VersionTest, DefaultVersion) {
    EngineVersion version = get_engine_version();
    EXPECT_EQ(version.major, 1);
    EXPECT_EQ(version.minor, 0);
    EXPECT_EQ(version.patch, 0);
}

TEST(VersionTest, VersionToString) {
    EngineVersion version{2, 5, 1, "beta"};
    std::string str = version_to_string(version);
    EXPECT_EQ(str, "2.5.1-beta");
}

// Result tests
TEST(ResultTest, SuccessResult) {
    auto result = success();
    EXPECT_TRUE(result.has_value());
}

TEST(ResultTest, FailureResult) {
    auto result = failure(ErrorCode::FileNotFound, "File not found");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::FileNotFound);
}

// Config tests
TEST(ConfigTest, DefaultValues) {
    Config& config = Config::instance();
    config.reset_to_defaults();
    config.register_default("test.int", 42, "Test integer");
    EXPECT_EQ(config.get_int("test.int"), 42);
}

TEST(ConfigTest, BoolConfig) {
    Config& config = Config::instance();
    config.set_bool("test.bool", true);
    EXPECT_TRUE(config.get_bool("test.bool"));
}

// Timer tests
TEST(TimerTest, TimerStart) {
    Timer& timer = Timer::instance();
    timer.reset();
    timer.start();
    EXPECT_TRUE(timer.is_running());
    EXPECT_FALSE(timer.is_paused());
}

TEST(TimerTest, TimerPauseResume) {
    Timer& timer = Timer::instance();
    timer.pause();
    EXPECT_TRUE(timer.is_paused());
    timer.resume();
    EXPECT_FALSE(timer.is_paused());
}

// Entity tests
TEST(EntityTest, CreateEntity) {
    EntityManager& manager = EntityManager::instance();
    manager.clear();
    EntityHandle entity = manager.create_entity();
    EXPECT_TRUE(entity.is_valid());
    EXPECT_TRUE(manager.is_alive(entity));
}

TEST(EntityTest, DestroyEntity) {
    EntityManager& manager = EntityManager::instance();
    manager.clear();
    EntityHandle entity = manager.create_entity();
    EXPECT_TRUE(manager.is_alive(entity));
    manager.destroy_entity(entity);
    EXPECT_FALSE(manager.is_alive(entity));
}

TEST(EntityTest, GenerationSafety) {
    EntityManager& manager = EntityManager::instance();
    manager.clear();
    EntityHandle entity1 = manager.create_entity();
    manager.destroy_entity(entity1);
    EntityHandle entity2 = manager.create_entity();
    EXPECT_NE(entity1.index, entity2.index);
    EXPECT_NE(entity1.generation, entity2.generation);
}

TEST(EntityTest, ComponentStorage) {
    EntityManager& manager = EntityManager::instance();
    manager.clear();
    EntityHandle entity = manager.create_entity();
    
    manager.add_component(entity, 42);
    EXPECT_TRUE(manager.has_component<int>(entity));
    
    int* value = manager.get_component<int>(entity);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(*value, 42);
    
    manager.remove_component<int>(entity);
    EXPECT_FALSE(manager.has_component<int>(entity));
}

// Scene tests
TEST(SceneTest, CreateScene) {
    Scene scene("TestScene");
    EXPECT_EQ(scene.name(), "TestScene");
    EXPECT_EQ(scene.entity_count(), 0);
}

TEST(SceneTest, CreateEntityInScene) {
    Scene scene("TestScene");
    EntityHandle entity = scene.create_entity();
    EXPECT_TRUE(entity.is_valid());
    EXPECT_EQ(scene.entity_count(), 1);
}

TEST(SceneTest, DestroyEntityInScene) {
    Scene scene("TestScene");
    EntityHandle entity = scene.create_entity();
    scene.destroy_entity(entity);
    EXPECT_EQ(scene.entity_count(), 0);
}

TEST(SceneTest, AddComponentToEntity) {
    Scene scene("TestScene");
    EntityHandle entity = scene.create_entity();
    scene.add_component(entity, Transform{});
    EXPECT_TRUE(scene.has_component<Transform>(entity));
}

// Logger tests
TEST(LoggerTest, LogLevels) {
    Logger& logger = Logger::instance();
    logger.set_min_severity(LogSeverity::Warn);
    EXPECT_EQ(logger.min_severity(), LogSeverity::Warn);
}

TEST(LoggerTest, LogOutput) {
    // Just verify logger doesn't crash
    KOW_LOG_INFO("test", "Test message");
    KOW_LOG_WARN("test", "Test warning");
    KOW_LOG_ERROR("test", "Test error");
}

// Filesystem tests
TEST(FilesystemTest, WriteAndRead) {
    Filesystem& fs = Filesystem::instance();
    fs.set_root(std::filesystem::current_path());
    
    std::vector<uint8_t> data = {1, 2, 3, 4, 5};
    auto write_result = fs.write_file("test_file.bin", data.data(), data.size());
    EXPECT_TRUE(write_result.has_value());
    
    auto read_result = fs.read_file("test_file.bin");
    EXPECT_TRUE(read_result.has_value());
    EXPECT_EQ(read_result->size(), data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ((*read_result)[i], data[i]);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}