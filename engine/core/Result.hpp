#pragma once
#include <string>
#include <utility>
#include <variant>

namespace kow::core {

struct Error {
    std::string code;
    std::string message;
};

template <typename T>
class Result {
public:
    Result(T value) : data_(std::move(value)) {}
    Result(Error error) : data_(std::move(error)) {}

    [[nodiscard]] bool ok() const { return std::holds_alternative<T>(data_); }
    [[nodiscard]] explicit operator bool() const { return ok(); }
    [[nodiscard]] const T& value() const { return std::get<T>(data_); }
    [[nodiscard]] T& value() { return std::get<T>(data_); }
    [[nodiscard]] const Error& error() const { return std::get<Error>(data_); }

private:
    std::variant<T, Error> data_;
};

template <>
class Result<void> {
public:
    Result() : error_(std::nullopt) {}
    Result(Error error) : error_(std::move(error)) {}

    [[nodiscard]] bool ok() const { return !error_.has_value(); }
    [[nodiscard]] explicit operator bool() const { return ok(); }
    [[nodiscard]] const Error& error() const { return *error_; }

private:
    std::optional<Error> error_;
};

inline Result<void> Ok() { return Result<void>{}; }
template <typename T>
inline Result<T> Ok(T value) { return Result<T>(std::move(value)); }
inline Error Err(std::string code, std::string message) { return Error{std::move(code), std::move(message)}; }

} // namespace kow::core
