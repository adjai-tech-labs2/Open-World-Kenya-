#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include "core/error_handling/result.h"

namespace kow {

struct ScriptValue {
    enum class Type : uint8_t {
        Nil = 0,
        Bool = 1,
        Number = 2,
        String = 3,
        Table = 4,
        Function = 5,
        Userdata = 6
    };

    Type type = Type::Nil;
    bool bool_value = false;
    double number_value = 0.0;
    std::string string_value;
    void* userdata = nullptr;
};

using ScriptFunction = std::function<ScriptValue(const std::vector<ScriptValue>&)>;

class IScriptingEngine {
public:
    virtual ~IScriptingEngine() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float delta_time) = 0;

    virtual Result<void> execute_string(const std::string& script) = 0;
    virtual Result<void> execute_file(const std::filesystem::path& path) = 0;

    virtual void register_function(const std::string& name, ScriptFunction fn) = 0;
    virtual ScriptValue call_function(const std::string& name, const std::vector<ScriptValue>& args) = 0;

    virtual void set_global(const std::string& name, ScriptValue value) = 0;
    virtual ScriptValue get_global(const std::string& name) = 0;

    virtual bool has_global(const std::string& name) const = 0;
    virtual bool is_initialized() const = 0;
    virtual std::string_view engine_name() const = 0;
};

class ScriptingEngineAdapter : public IScriptingEngine {
public:
    using InitializeFn = std::function<bool()>;
    using ExecuteFn = std::function<Result<void>(const std::string&)>;

    void set_on_initialize(InitializeFn fn) { on_initialize_ = std::move(fn); }
    void set_on_execute(ExecuteFn fn) { on_execute_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return true;
    }

    void shutdown() override {}
    void update(float) override {}

    Result<void> execute_string(const std::string& script) override {
        if (on_execute_) return on_execute_(script);
        return success();
    }

    Result<void> execute_file(const std::filesystem::path& path) override {
        return execute_string(path.string());
    }

    void register_function(const std::string&, ScriptFunction) override {}
    ScriptValue call_function(const std::string&, const std::vector<ScriptValue>&) override {
        return ScriptValue{ScriptValue::Type::Nil};
    }

    void set_global(const std::string&, ScriptValue) override {}
    ScriptValue get_global(const std::string&) override {
        return ScriptValue{ScriptValue::Type::Nil};
    }

    bool has_global(const std::string&) const override { return false; }
    bool is_initialized() const override { return false; }
    std::string_view engine_name() const override { return "Null Scripting"; }
};

class ScriptingManager {
public:
    static ScriptingManager& instance();

    void initialize();
    void shutdown();
    void update(float delta_time);

    IScriptingEngine* engine() { return engine_.get(); }
    const IScriptingEngine* engine() const { return engine_.get(); }

    void set_engine(std::unique_ptr<IScriptingEngine> engine) { engine_ = std::move(engine); }

    template<typename... Args>
    void register_function(const std::string& name, ScriptFunction fn) {
        if (engine_) engine_->register_function(name, std::move(fn));
    }

private:
    ScriptingManager() = default;
    std::unique_ptr<IScriptingEngine> engine_;
};

#define KOW_SCRIPTING kow::ScriptingManager::instance().engine()

} // namespace kow