#include "test_framework.hpp"
#include "core/Application.hpp"
#include "core/IModule.hpp"

using namespace kow::core;

class TestModule final : public IModule {
public:
    explicit TestModule(std::string n, bool fail_init = false, bool fail_shutdown = false)
        : n_(std::move(n)), fail_init_(fail_init), fail_shutdown_(fail_shutdown) {}
    std::string name() const override { return n_; }
    Result<void> initialize(Application&) override {
        if (fail_init_) return Err("test.init_fail", "forced init failure");
        inited_ = true; ++init_count; return Ok();
    }
    Result<void> shutdown(Application&) override {
        if (!inited_) return Ok();
        inited_ = false; ++shutdown_count;
        if (fail_shutdown_) return Err("test.shutdown_fail", "forced shutdown failure");
        return Ok();
    }
    bool is_initialized() const override { return inited_; }
    static inline int init_count = 0;
    static inline int shutdown_count = 0;
private:
    std::string n_;
    bool fail_init_{false};
    bool fail_shutdown_{false};
    bool inited_{false};
};

TEST(application_lifecycle_init_run_shutdown) {
    TestModule::init_count = 0; TestModule::shutdown_count = 0;
    EngineConfig cfg; cfg.app_name = "test"; cfg.max_frames = 2; cfg.log_level = LogLevel::Error;
    Application app(cfg);
    app.modules().add(std::make_unique<TestModule>("m1"));
    app.modules().add(std::make_unique<TestModule>("m2"));
    auto init = app.initialize();
    ASSERT_TRUE(init.ok());
    ASSERT_EQ(static_cast<int>(app.state()), static_cast<int>(AppState::Initialized));
    ASSERT_EQ(TestModule::init_count, 2);
    auto run = app.run();
    ASSERT_TRUE(run.ok());
    ASSERT_EQ(static_cast<int>(app.state()), static_cast<int>(AppState::Running));
    auto shut = app.shutdown();
    ASSERT_TRUE(shut.ok());
    ASSERT_EQ(static_cast<int>(app.state()), static_cast<int>(AppState::ShutDown));
    ASSERT_EQ(TestModule::shutdown_count, 2);
    return true;
}

TEST(application_invalid_config_fails_init) {
    EngineConfig cfg; cfg.app_name = ""; cfg.log_level = LogLevel::Error;
    Application app(cfg);
    auto init = app.initialize();
    ASSERT_FALSE(init.ok());
    ASSERT_EQ(static_cast<int>(app.state()), static_cast<int>(AppState::Failed));
    return true;
}

TEST(application_module_init_failure_rolls_back) {
    TestModule::init_count = 0; TestModule::shutdown_count = 0;
    EngineConfig cfg; cfg.app_name = "test"; cfg.log_level = LogLevel::Error;
    Application app(cfg);
    app.modules().add(std::make_unique<TestModule>("ok"));
    app.modules().add(std::make_unique<TestModule>("bad", true, false));
    auto init = app.initialize();
    ASSERT_FALSE(init.ok());
    ASSERT_EQ(TestModule::init_count, 1);
    ASSERT_EQ(TestModule::shutdown_count, 1); // rolled back initialized module
    return true;
}
