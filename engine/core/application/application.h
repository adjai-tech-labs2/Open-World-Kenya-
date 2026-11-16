#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "core/modules/module_registry.h"
#include "core/time/timer.h"
#include "core/configuration/config.h"
#include "core/filesystem/filesystem.h"
#include "core/logging/logger.h"

namespace kow {

class IEngine {
public:
    virtual ~IEngine() = default;
    virtual bool initialize() = 0;
    virtual void run() = 0;
    virtual void shutdown() = 0;
    virtual const EngineVersion& version() const = 0;
};

class Engine final : public IEngine {
public:
    Engine();
    ~Engine() override;

    bool initialize() override;
    void run() override;
    void shutdown() override;
    const EngineVersion& version() const override { return version_; }

    static Engine& instance();

    bool is_running() const { return running_; }
    float delta_time() const { return Timer::instance().delta_time(); }

private:
    void register_core_modules();
    void setup_logging();
    void setup_config();
    void setup_filesystem();
    void setup_platform();
    
    void process_input();
    void update_game();
    void render_frame();

    EngineVersion version_;
    bool running_ = false;
    bool initialized_ = false;
};

#define KOW_ENGINE kow::Engine::instance()

} // namespace kow