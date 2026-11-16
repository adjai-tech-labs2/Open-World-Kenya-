#pragma once
#include "core/Result.hpp"
#include <string>

namespace kow::audio {

// Interface only: no audio playback/mixing backend implemented or claimed.
class IAudioSystem {
public:
    virtual ~IAudioSystem() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual kow::core::Result<void> initialize() = 0;
    virtual void update(float /*dt*/) = 0;
    virtual kow::core::Result<void> shutdown() = 0;
    [[nodiscard]] virtual bool is_initialized() const = 0;
};

class NullAudioSystem final : public IAudioSystem {
public:
    [[nodiscard]] std::string name() const override { return "NullAudioSystem (no audio backend)"; }
    kow::core::Result<void> initialize() override { initialized_ = true; return kow::core::Ok(); }
    void update(float) override {}
    kow::core::Result<void> shutdown() override { initialized_ = false; return kow::core::Ok(); }
    [[nodiscard]] bool is_initialized() const override { return initialized_; }
private:
    bool initialized_{false};
};

} // namespace kow::audio
