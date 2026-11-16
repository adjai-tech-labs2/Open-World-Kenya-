#include "core/configuration/config.h"
#include "core/filesystem/filesystem.h"
#include "core/logging/logger.h"
#include <fstream>

namespace kow {

Config& Config::instance() {
    static Config config;
    return config;
}

void Config::set(const std::string& key, ConfigValue value, std::string desc) {
    entries_[key] = ConfigEntry{std::move(value), std::move(desc)};
}

void Config::set_bool(const std::string& key, bool value) {
    set(key, value);
}

void Config::set_int(const std::string& key, int64_t value) {
    set(key, value);
}

void Config::set_float(const std::string& key, double value) {
    set(key, value);
}

void Config::set_string(const std::string& key, std::string value) {
    set(key, std::move(value));
}

bool Config::has(const std::string& key) const {
    return entries_.find(key) != entries_.end();
}

bool Config::get_bool(const std::string& key, bool default_value) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return default_value;
    if (std::holds_alternative<bool>(it->second.value)) {
        return std::get<bool>(it->second.value);
    }
    return default_value;
}

int64_t Config::get_int(const std::string& key, int64_t default_value) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return default_value;
    if (std::holds_alternative<int64_t>(it->second.value)) {
        return std::get<int64_t>(it->second.value);
    }
    return default_value;
}

double Config::get_float(const std::string& key, double default_value) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return default_value;
    if (std::holds_alternative<double>(it->second.value)) {
        return std::get<double>(it->second.value);
    }
    return default_value;
}

std::string Config::get_string(const std::string& key, std::string default_value) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return default_value;
    if (std::holds_alternative<std::string>(it->second.value)) {
        return std::get<std::string>(it->second.value);
    }
    return default_value;
}

ConfigValue Config::get_value(const std::string& key) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return std::monostate{};
    return it->second.value;
}

void Config::register_default(const std::string& key, ConfigValue value, std::string desc) {
    defaults_[key] = value;
    if (entries_.find(key) == entries_.end()) {
        entries_[key] = ConfigEntry{value, std::move(desc)};
    }
}

void Config::reset_to_defaults() {
    for (const auto& [key, value] : defaults_) {
        if (auto it = entries_.find(key); it != entries_.end()) {
            it->second.value = value;
        }
    }
}

VoidResult Config::load_from_file(const std::filesystem::path& path) {
    auto result = Filesystem::instance().read_text(path);
    if (!result) return failure(result.error());

    // Simple JSON-like parsing (to be replaced with proper JSON parser)
    // For now, support key=value format
    std::istringstream stream(*result);
    std::string line;
    
    while (std::getline(stream, line)) {
        // Remove comments and whitespace
        auto comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        
        // Trim
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        auto end = line.find_last_not_of(" \t\r\n");
        line = line.substr(start, end - start + 1);
        
        auto eq_pos = line.find('=');
        if (eq_pos == std::string::npos) continue;
        
        std::string key = line.substr(0, eq_pos);
        std::string value = line.substr(eq_pos + 1);
        
        // Trim key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        if (value == "true") set_bool(key, true);
        else if (value == "false") set_bool(key, false);
        else if (value.find('.') != std::string::npos) set_float(key, std::stod(value));
        else {
            try { set_int(key, std::stoll(value)); }
            catch (...) { set_string(key, value); }
        }
    }
    
    return success();
}

VoidResult Config::save_to_file(const std::filesystem::path& path) const {
    std::ostringstream stream;
    stream << "# KOW Engine Configuration\n";
    
    for (const auto& [key, entry] : entries_) {
        if (!entry.description.empty()) {
            stream << "# " << entry.description << "\n";
        }
        std::visit([&](const auto& value) {
            if constexpr (std::is_same_v<decltype(value), bool>) {
                stream << key << "=" << (value ? "true" : "false") << "\n";
            } else if constexpr (std::is_integral_v<decltype(value)>) {
                stream << key << "=" << value << "\n";
            } else if constexpr (std::is_floating_point_v<decltype(value)>) {
                stream << key << "=" << value << "\n";
            } else if constexpr (std::is_same_v<decltype(value), std::string>) {
                stream << key << "=" << value << "\n";
            }
        }, entry.value);
    }
    
    return Filesystem::instance().write_text(path, stream.str());
}

} // namespace kow