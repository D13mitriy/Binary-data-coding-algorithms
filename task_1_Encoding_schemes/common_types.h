#pragma once
#include <variant>
#include <string>
#include <format>
#include <filesystem>
#include <iostream>

struct AppError {
    size_t line = 0;
    size_t column = 0;
    std::string message;
    
    std::string to_string() const {
        if (line > 0 && column > 0)
            return std::format("Рядок {}, символ {:02}: {}", line, column, message);
        if (line > 0)
            return std::format("Рядок {}: {}", line, message);
        return message;
    }
};

struct Void {};

template<typename T = Void>
class [[nodiscard]] Result {
    std::variant<T, AppError> content;
public:
    static Result ok(T v = T{}) { return Result(v); }
    static Result err(AppError e) { return Result(e); }
    
    bool is_ok() const { return std::holds_alternative<T>(content); }
    const T& value() const { return std::get<T>(content); }
    const AppError& error() const { return std::get<AppError>(content); }
    
private:
    Result(T v) : content(v) {}
    Result(AppError e) : content(e) {}
};

struct Config {
    std::filesystem::path input_path;
    std::filesystem::path output_path;
    size_t line_length = 76; 
};
