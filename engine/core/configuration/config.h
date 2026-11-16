#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <variant>
#include <concepts>
#include "core/error_handling/result.h"

namespace kow {

using ConfigValue = std::variant<std::monostate, bool, int64_t, double, std::string>;

struct ConfigEntry {
    ConfigValue value;
    std::string description;
};

class Config {
public:
    static Config& instance();

    VoidResult load_from_file(const std::filesystem::path& path);
    VoidResult save_to_file(const std::filesystem::path& path) const;

    void set(const std::string& key, ConfigValue value, std::string desc = "");
    void set_bool(const std::string& key, bool value);
    void set_int(const std::string& key, int64_t value);
    void set_float(const std::string& key, double value);
    void set_string(const std::string& key, std::string value);

    bool has(const std::string& key) const;
    bool get_bool(const std::string& key, bool default_value = false) const;
    int64_t get_int(const std::string& key, int64_t default_value = 0) const;
    double get_float(const std::string& key, double default_value = 0.0) const;
    std::string get_string(const std::string& key, std::string default_value = "") const;
    ConfigValue get_value(const std::string& key) const;

    template<typename T>
    Result<T> get_typed(const std::string& key) const;

    void register_default(const std::string& key, ConfigValue value, std::string desc = "");
    void reset_to_defaults();

    const std::unordered_map<std::string, ConfigEntry>& entries() const { return entries_; }

private:
    Config() = default;
    std::unordered_map<std::string, ConfigEntry> entries_;
    std::unordered_map<std::string, ConfigValue> defaults_;
};

#define KOW_CONFIG_BOOL(key, default) Config::instance().get_bool(key, default)
#define KOW_CONFIG_INT(key, default) Config::instance().get_int(key, default)
#define KOW_CONFIG_FLOAT(key, default) Config::instance().get_float(key, default)
#define KOW_CONFIG_STRING(key, default) Config::instance().get_string(key, default)

} // namespace kow