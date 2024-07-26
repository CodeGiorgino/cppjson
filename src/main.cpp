#include "main.hpp"

#include <cstddef>
#include <optional>
#include <string>

namespace json {
    auto json_value::value() const noexcept -> std::any {
        return _value;
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
