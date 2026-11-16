#include "input/input_abstraction.h"
#include "core/logging/logger.h"

namespace kow {

InputManager& InputManager::instance() {
    static InputManager manager;
    return manager;
}

void InputManager::initialize() {
    if (!input_) {
        input_ = std::make_unique<InputSystemAdapter>();
    }
    input_->initialize();
    KOW_LOG_INFO("input", "Input system initialized");
}

void InputManager::shutdown() {
    if (input_) {
        input_->shutdown();
        KOW_LOG_INFO("input", "Input system shutdown");
    }
}

void InputManager::update(float delta_time) {
    if (input_) {
        input_->update(delta_time);
    }
}

} // namespace kow