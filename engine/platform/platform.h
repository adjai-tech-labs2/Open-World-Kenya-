#pragma once

#include <string>
#include <memory>
#include <vector>
#include "core/error_handling/result.h"

namespace kow {

struct PlatformInfo {
    std::string os_name;
    std::string os_version;
    std::string cpu_name;
    uint32_t cpu_cores = 0;
    uint64_t total_memory = 0;
    uint64_t available_memory = 0;
    std::string gpu_name;
    uint64_t gpu_memory = 0;
    std::string renderer;
};

class IPlatform {
public:
    virtual ~IPlatform() = default;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void pump_events() = 0;
    virtual bool should_close() const = 0;
    virtual const PlatformInfo& info() const = 0;
    virtual double get_time() const = 0;
    virtual void set_window_title(const std::string& title) = 0;
    virtual void set_window_size(int width, int height) = 0;
    virtual void get_window_size(int& width, int& height) const = 0;
    virtual bool is_window_focused() const = 0;
    virtual void* native_window() const = 0;
};

class PlatformAdapter : public IPlatform {
public:
    using InitializeFn = std::function<bool()>;
    using PumpFn = std::function<void()>;

    void set_on_initialize(InitializeFn fn) { on_initialize_ = std::move(fn); }
    void set_on_pump(PumpFn fn) { on_pump_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return true;
    }

    void shutdown() override {}
    void pump_events() override { if (on_pump_) on_pump_(); }
    bool should_close() const override { return false; }
    const PlatformInfo& info() const override { return info_; }
    double get_time() const override { return 0.0; }
    void set_window_title(const std::string&) override {}
    void set_window_size(int, int) override {}
    void get_window_size(int& w, int& h) const override { w = 1280; h = 720; }
    bool is_window_focused() const override { return true; }
    void* native_window() const override { return nullptr; }

private:
    PlatformInfo info_;
};

class PlatformManager {
public:
    static PlatformManager& instance();

    void initialize();
    void shutdown();
    void pump_events();
    bool should_close() const;
    void set_window_title(const std::string& title);
    void set_window_size(int width, int height);
    void get_window_size(int& width, int& height) const;
    bool is_window_focused() const;
    void* native_window() const;
    double get_time() const;
    const PlatformInfo& info() const;

    IPlatform* platform() { return platform_.get(); }
    const IPlatform* platform() const { return platform_.get(); }
    void set_platform(std::unique_ptr<IPlatform> platform) { platform_ = std::move(platform); }

private:
    PlatformManager() = default;
    std::unique_ptr<IPlatform> platform_;
};

#define KOW_PLATFORM kow::PlatformManager::instance()

} // namespace kow