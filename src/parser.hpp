#pragma once
#include <exception>

#include "json.hpp"

namespace json {
class invalid_json_exception final : public std::exception {
   public:
    invalid_json_exception(const std::string& msg) : _msg(msg) {}
    auto what() const noexcept -> const char* {
        return _msg.c_str();
    }

   private:
    const std::string _msg{};
};

[[nodiscard]]
auto deserialize(const char* filepath) -> node;
[[nodiscard]]
auto deserialize(std::string content) -> node;
}  // namespace json
