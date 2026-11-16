#pragma once
#include <cstdint>
#include <string>

namespace kow::platform {

// Input abstraction interface: no platform/window backend polled.
struct InputState {
    bool quit_requested{false};
};

class IInput {
public:
    virtual ~IInput() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual void update() = 0;
    [[nodiscard]] virtual const InputState& state() const = 0;
};

class NullInput final : public IInput {
public:
    [[nodiscard]] std::string name() const override { return "NullInput (no backend)"; }
    void update() override {}
    [[nodiscard]] const InputState& state() const override { return state_; }
private:
    InputState state_{};
};

} // namespace kow::platform
