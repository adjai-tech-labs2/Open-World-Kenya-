#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "core/error_handling/result.h"

namespace kow {

struct KeyEvent {
    int key_code = 0;
    bool pressed = false;
    bool repeated = false;
    double timestamp = 0.0;
};

struct MouseEvent {
    float x = 0.0f;
    float y = 0.0f;
    float delta_x = 0.0f;
    float delta_y = 0.0f;
    int button = 0;
    bool pressed = false;
    double timestamp = 0.0;
};

struct GamepadEvent {
    int button = 0;
    bool pressed = false;
    float axis[6] = {0, 0, 0, 0, 0, 0};
    double timestamp = 0.0;
};

struct InputState {
    bool keys[256] = {false};
    bool mouse_buttons[8] = {false};
    float mouse_x = 0.0f;
    float mouse_y = 0.0f;
    float mouse_delta_x = 0.0f;
    float mouse_delta_y = 0.0f;
    float gamepad_axes[6] = {0, 0, 0, 0, 0, 0};
    bool gamepad_buttons[16] = {false};
    bool gamepad_connected = false;
};

class IInputSystem {
public:
    virtual ~IInputSystem() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float delta_time) = 0;

    virtual bool is_key_pressed(int key_code) const = 0;
    virtual bool is_key_down(int key_code) const = 0;
    virtual bool is_key_released(int key_code) const = 0;

    virtual bool is_mouse_button_down(int button) const = 0;
    virtual bool is_mouse_button_pressed(int button) const = 0;
    virtual float mouse_x() const = 0;
    virtual float mouse_y() const = 0;
    virtual float mouse_delta_x() const = 0;
    virtual float mouse_delta_y() const = 0;

    virtual bool is_gamepad_connected(int gamepad_index = 0) const = 0;
    virtual bool is_gamepad_button_down(int button, int gamepad_index = 0) const = 0;
    virtual float gamepad_axis(int axis, int gamepad_index = 0) const = 0;

    virtual void set_cursor_position(float x, float y) = 0;
    virtual void set_cursor_visible(bool visible) = 0;
    virtual bool is_cursor_visible() const = 0;

    virtual void set_grab_mode(bool grab) = 0;
    virtual bool is_grab_mode() const = 0;

    virtual const InputState& state() const = 0;
    virtual bool is_initialized() const = 0;
};

class InputSystemAdapter : public IInputSystem {
public:
    using InitializeFn = std::function<bool()>;
    using UpdateFn = std::function<void(float)>;

    void set_on_initialize(InitializeFn fn) { on_initialize_ = std::move(fn); }
    void set_on_update(UpdateFn fn) { on_update_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return true;
    }

    void shutdown() override {}
    void update(float delta_time) override {
        if (on_update_) on_update_(delta_time);
    }

    bool is_key_pressed(int) const override { return false; }
    bool is_key_down(int) const override { return false; }
    bool is_key_released(int) const override { return false; }

    bool is_mouse_button_down(int) const override { return false; }
    bool is_mouse_button_pressed(int) const override { return false; }
    float mouse_x() const override { return 0.0f; }
    float mouse_y() const override { return 0.0f; }
    float mouse_delta_x() const override { return 0.0f; }
    float mouse_delta_y() const override { return 0.0f; }

    bool is_gamepad_connected(int) const override { return false; }
    bool is_gamepad_button_down(int, int) const override { return false; }
    float gamepad_axis(int, int) const override { return 0.0f; }

    void set_cursor_position(float, float) override {}
    void set_cursor_visible(bool) override {}
    bool is_cursor_visible() const override { return true; }

    void set_grab_mode(bool) override {}
    bool is_grab_mode() const override { return false; }

    const InputState& state() const override { return state_; }
    bool is_initialized() const override { return false; }

private:
    InputState state_;
};

class InputManager {
public:
    static InputManager& instance();

    void initialize();
    void shutdown();
    void update(float delta_time);

    IInputSystem* input() { return input_.get(); }
    const IInputSystem* input() const { return input_.get(); }

    void set_input_system(std::unique_ptr<IInputSystem> input) { input_ = std::move(input); }

private:
    InputManager() = default;
    std::unique_ptr<IInputSystem> input_;
};

#define KOW_INPUT kow::InputManager::instance().input()

} // namespace kow