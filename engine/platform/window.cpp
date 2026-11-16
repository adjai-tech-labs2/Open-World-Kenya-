#include "platform/window.h"
#include "core/logging/logger.h"

namespace kow {

WindowManager& WindowManager::instance() {
    static WindowManager manager;
    return manager;
}

void WindowManager::create(const std::string& title, int width, int height) {
    if (!window_) {
        window_ = std::make_unique<WindowAdapter>();
    }
    window_->create(title, width, height);
    KOW_LOG_INFO("window", "Created window: {}x{}", width, height);
}

void WindowManager::destroy() {
    if (window_) {
        window_->destroy();
        KOW_LOG_INFO("window", "Window destroyed");
    }
}

void WindowManager::update() {
    // Window update logic
}

void WindowManager::set_title(const std::string& title) {
    if (window_) window_->set_title(title);
}

void WindowManager::set_size(int width, int height) {
    if (window_) window_->set_size(width, height);
}

void WindowManager::get_size(int& width, int& height) const {
    if (window_) window_->get_size(width, height);
}

bool WindowManager::is_open() const {
    return window_ ? window_->is_open() : false;
}

bool WindowManager::is_focused() const {
    return window_ ? window_->is_focused() : false;
}

void WindowManager::add_listener(std::unique_ptr<WindowListener> listener) {
    listeners_.push_back(std::move(listener));
}

} // namespace kow