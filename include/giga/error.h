#ifndef GIGA_ERROR_H
#define GIGA_ERROR_H

#include <format>
#include <source_location>
#include <stdexcept>
#include <string>

namespace giga {
namespace err {

// Placeholder logger
template<typename Error>
Error FormatException(const std::string& message, std::source_location location = std::source_location::current()) {
    return Error(std::format("{}:{}: {}: {}", location.file_name(), location.line(), location.function_name(), message));
}

} // namespace err
} // namespace giga

#endif // GIGA_ERROR_H
