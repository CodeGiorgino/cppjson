#include "main.hpp"

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>

namespace json {
auto json_value::try_get_int() -> std::optional<int> {
    try {
        return std::stoi(_literal);
    } catch (const std::invalid_argument& e) {
        return {};
    }
}

auto json_value::try_get_float() -> std::optional<float> {
    try {
        return std::stof(_literal);
    } catch (const std::invalid_argument& e) {
        return {};
    }
}

auto json_value::try_get_bool() noexcept -> std::optional<bool> {
    if (_literal == "true") return true;
    if (_literal == "false") return false;
    return {};
}

auto json_object::operator[](std::string key) noexcept
    -> std::optional<json_node*> {
    if (auto search = _childrens.find(key); search != _childrens.end())
        return &search->second;
    return {};
}

auto json_object::size() const noexcept -> size_t {
    return _childrens.size();
}

auto json_object::try_emplace(std::string key, json_node value) noexcept
    -> std::optional<json_node*> {
    if (auto result = _childrens.try_emplace(key, value); result.second)
        return &result.first->second;
    return {};
}


auto json_array::operator[](size_t index) noexcept -> std::optional<json_node*> {
    if (index >= _childrens.size()) return {};
    return &_childrens[index];
}

auto json_array::size() const noexcept -> size_t {
    return _childrens.size();
}

auto json_array::push_back(json_node value) noexcept -> json_node* {
    _childrens.push_back(value);
    return &_childrens.back();
}
}  // namespace json
