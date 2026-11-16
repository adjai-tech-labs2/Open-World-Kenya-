#include "UUID.hpp"
#include <sstream>
#include <iomanip>

namespace kow::core {

std::string UUID::to_string() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(16) << high << "-" << std::setw(16) << low;
    return oss.str();
}

} // namespace kow::core
