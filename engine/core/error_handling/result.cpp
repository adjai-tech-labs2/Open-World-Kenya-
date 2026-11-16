#include "core/error_handling/result.h"

namespace kow {

std::string error_to_string(const Error& error) {
    return std::format("Error {}: {}", static_cast<uint32_t>(error.code), error.message);
}

} // namespace kow