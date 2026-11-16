#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace kow::game::characters {

enum class CharacterRole { Pedestrian, Driver, Passenger, Police, Vendor, Officer };

struct CharacterSpec {
    std::uint32_t id{0};
    std::string name;
    std::string sheng_name;
    CharacterRole role{CharacterRole::Pedestrian};
    std::string home_district;
};

class CharacterRegistry {
public:
    kow::core::Result<void> register_character(const CharacterSpec& spec);
    [[nodiscard]] const CharacterSpec* character(std::uint32_t id) const;
    [[nodiscard]] std::size_t character_count() const { return characters_.size(); }

private:
    std::unordered_map<std::uint32_t, CharacterSpec> characters_;
};

} // namespace kow::game::characters
