#pragma once
#include "core/Result.hpp"
#include <string>

namespace kow::graphics {

// Interface only: no rendering backend implemented or claimed.
class IRenderer {
public:
    virtual ~IRenderer() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual kow::core::Result<void> initialize() = 0;
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;
    virtual kow::core::Result<void> shutdown() = 0;
    [[nodiscard]] virtual bool is_initialized() const = 0;
};

// Null/stub renderer for headless bootstrap and testing. Performs no GPU work.
class NullRenderer final : public IRenderer {
public:
    [[nodiscard]] std::string name() const override { return "NullRenderer (no backend)"; }
    kow::core::Result<void> initialize() override { initialized_ = true; return kow::core::Ok(); }
    void begin_frame() override {}
    void end_frame() override {}
    kow::core::Result<void> shutdown() override { initialized_ = false; return kow::core::Ok(); }
    [[nodiscard]] bool is_initialized() const override { return initialized_; }
private:
    bool initialized_{false};
};

} // namespace kow::graphics
