#pragma once
#include "Result.hpp"
#include <string>

namespace kow::core {

class Application;

class IModule {
public:
    virtual ~IModule() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual Result<void> initialize(Application& app) = 0;
    virtual Result<void> shutdown(Application& app) = 0;
    [[nodiscard]] virtual bool is_initialized() const = 0;
};

} // namespace kow::core
