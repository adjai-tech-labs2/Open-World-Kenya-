#pragma once

#include <string>
#include <string_view>
#include <expected>
#include <variant>
#include <source_location>

namespace kow {

enum class ErrorCode : uint32_t {
    Success = 0,
    Unknown = 1,
    InvalidArgument = 2,
    InvalidState = 3,
    NotFound = 4,
    OutOfMemory = 5,
    FileNotFound = 6,
    FileCorrupt = 7,
    PermissionDenied = 8,
    Timeout = 9,
    NotSupported = 10,
    GraphicsError = 100,
    PhysicsError = 101,
    AudioError = 102,
    ScriptError = 103,
    AssetError = 104,
    NetworkError = 105
};

struct Error {
    ErrorCode code;
    std::string message;
    std::source_location location;
};

template<typename T>
using Result = std::expected<T, Error>;

using VoidResult = Result<void>;

inline VoidResult success() { return {}; }

inline Result<void> failure(ErrorCode code, std::string message,
                            std::source_location loc = std::source_location::current()) {
    return std::unexpected(Error{code, std::move(message), loc});
}

inline Result<void> failure(std::string message,
                            std::source_location loc = std::source_location::current()) {
    return failure(ErrorCode::Unknown, std::move(message), loc);
}

#define KOW_RETURN_IF_ERROR(expr) \
    do { \
        auto _result = (expr); \
        if (!_result) { \
            return _result; \
        } \
    } while(false)

#define KOW_RETURN_IF_FALSE(cond, code, msg) \
    do { \
        if (!(cond)) { \
            return failure((code), (msg)); \
        } \
    } while(false)

} // namespace kow