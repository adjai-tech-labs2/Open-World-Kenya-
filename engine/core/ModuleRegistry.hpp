#pragma once
#include "IModule.hpp"
#include "Logger.hpp"
#include <memory>
#include <string>
#include <vector>

namespace kow::core {

class Application;

class ModuleRegistry {
public:
    void add(std::unique_ptr<IModule> module);
    Result<void> initialize_all(Application& app, Logger& logger);
    Result<void> shutdown_all(Application& app, Logger& logger);
    [[nodiscard]] const std::vector<std::unique_ptr<IModule>>& modules() const { return modules_; }

private:
    std::vector<std::unique_ptr<IModule>> modules_;
    std::vector<bool> initialized_;
};

} // namespace kow::core
