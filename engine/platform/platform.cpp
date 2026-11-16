#include "platform/platform.h"
#include "core/logging/logger.h"
#include <iostream>
#include <sstream>

namespace kow {

PlatformManager& PlatformManager::instance() {
    static PlatformManager manager;
    return manager;
}

void PlatformManager::initialize() {
    if (!platform_) {
        platform_ = std::make_unique<PlatformAdapter>();
    }
    platform_->initialize();
    KOW_LOG_INFO("platform", "Platform initialized: {}", platform_->info().os_name);
}

void PlatformManager::shutdown() {
    if (platform_) {
        platform_->shutdown();
        KOW_LOG_INFO("platform", "Platform shutdown");
    }
}

void PlatformManager::pump_events() {
    if (platform_) platform_->pump_events();
}

bool PlatformManager::should_close() const {
    return platform_ ? platform_->should_close() : true;
}

void PlatformManager::set_window_title(const std::string& title) {
    if (platform_) platform_->set_window_title(title);
}

void PlatformManager::set_window_size(int width, int height) {
    if (platform_) platform_->set_window_size(width, height);
}

void PlatformManager::get_window_size(int& width, int& height) const {
    if (platform_) platform_->get_window_size(width, height);
}

bool PlatformManager::is_window_focused() const {
    return platform_ ? platform_->is_window_focused() : false;
}

void* PlatformManager::native_window() const {
    return platform_ ? platform_->native_window() : nullptr;
}

double PlatformManager::get_time() const {
    return platform_ ? platform_->get_time() : 0.0;
}

const PlatformInfo& PlatformManager::info() const {
    static PlatformInfo empty;
    return platform_ ? platform_->info() : empty;
}

} // namespace kow