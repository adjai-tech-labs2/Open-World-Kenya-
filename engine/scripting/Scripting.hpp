#pragma once
#include "core/Result.hpp"
#include <string>

namespace kow::scripting {

// Interface only: no script VM/runtime backend implemented or claimed.
class IScriptingRuntime {
public:
    virtual ~IScriptingRuntime() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual kow::core::Result<void> initialize() = 0;
    virtual kow::core::Result<void> execute_string(const std::string& /*source*/) = 0;
    virtual kow::core::Result<void> shutdown() = 0;
    [[nodiscard]] virtual bool is_initialized() const = 0;
};

class NullScriptingRuntime final : public IScriptingRuntime {
public:
    [[nodiscard]] std::string name() const override { return "NullScriptingRuntime (no VM backend)"; }
    kow::core::Result<void> initialize() override { initialized_ = true; return kow::core::Ok(); }
    kow::core::Result<void> execute_string(const std::string&) override {
        return kow::core::Err("scripting.no_backend", "No scripting backend integrated");
    }
    kow::core::Result<void> shutdown() override { initialized_ = false; return kow::core::Ok(); }
    [[nodiscard]] bool is_initialized() const override { return initialized_; }
private:
    bool initialized_{false};
};

} // namespace kow::scripting
