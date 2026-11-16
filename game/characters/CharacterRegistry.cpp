#include "CharacterRegistry.hpp"

namespace kow::game::characters {

kow::core::Result<void> CharacterRegistry::register_character(const CharacterSpec& spec) {
    if (spec.name.empty()) {
        return kow::core::Err("character.invalid_name", "Character name must not be empty");
    }
    if (characters_.find(spec.id) != characters_.end()) {
        return kow::core::Err("character.exists", "Character id already registered");
    }
    characters_.emplace(spec.id, spec);
    return kow::core::Ok();
}

const CharacterSpec* CharacterRegistry::character(std::uint32_t id) const {
    auto it = characters_.find(id);
    return it == characters_.end() ? nullptr : &it->second;
}

} // namespace kow::game::characters
