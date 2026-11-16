#include "test_framework.hpp"
#include "core/Config.hpp"

using namespace kow::core;

TEST(config_validation_valid_defaults) {
    EngineConfig cfg;
    auto res = validate_config(cfg);
    ASSERT_TRUE(res.ok());
    return true;
}

TEST(config_validation_empty_app_name) {
    EngineConfig cfg;
    cfg.app_name = "";
    auto res = validate_config(cfg);
    ASSERT_FALSE(res.ok());
    ASSERT_EQ(res.error().code, std::string("config.invalid_app_name"));
    return true;
}

TEST(config_validation_invalid_target_fps) {
    EngineConfig cfg;
    cfg.target_fps = 0.0;
    ASSERT_FALSE(validate_config(cfg).ok());
    cfg.target_fps = -10.0;
    ASSERT_FALSE(validate_config(cfg).ok());
    cfg.target_fps = 1001.0;
    ASSERT_FALSE(validate_config(cfg).ok());
    cfg.target_fps = 60.0;
    ASSERT_TRUE(validate_config(cfg).ok());
    return true;
}

TEST(config_validation_invalid_max_frames) {
    EngineConfig cfg;
    cfg.max_frames = -1;
    auto res = validate_config(cfg);
    ASSERT_FALSE(res.ok());
    ASSERT_EQ(res.error().code, std::string("config.invalid_max_frames"));
    return true;
}
