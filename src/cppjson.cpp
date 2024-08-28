#include "cppjson.hpp"

#include <bits/ranges_util.h>

#include <cstddef>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

using string = std::string;

auto pad_left(const string& source, size_t size) noexcept -> std::string {
    string retval{};
    for (size_t i = 0; i < size; ++i) retval += ' ';
    return retval + source;
}

auto pad_left(const char* source, size_t size) noexcept -> string {
    return pad_left(string{source}, size);
}

namespace json {
json_node::json_node() noexcept {
    _value = nullptr;
}

json_node::json_node(const bool& value) noexcept {
    _value = value;
}
json_node::json_node(const int& value) noexcept {
    _value = value;
}
json_node::json_node(const float& value) noexcept {
    _value = value;
}
json_node::json_node(const std::string& value) noexcept {
    _value = std::make_shared<std::string>(value);
}
json_node::json_node(const array_t& value) noexcept {
    _value = std::make_shared<array_t>(value);
}
json_node::json_node(const object_t& value) noexcept {
    _value = std::make_shared<object_t>(value);
}

json_node::json_node(bool&& value) noexcept {
    _value = std::move(value);
}
json_node::json_node(int&& value) noexcept {
    _value = std::move(value);
}
json_node::json_node(float&& value) noexcept {
    _value = std::move(value);
}
json_node::json_node(std::string&& value) noexcept {
    _value = std::make_shared<std::string>(value);
}
json_node::json_node(array_t&& value) noexcept {
    _value = std::make_shared<array_t>(value);
}
json_node::json_node(object_t&& value) noexcept {
    _value = std::make_shared<object_t>(value);
}

auto json_node::operator[](const size_t& index) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::runtime_error(
            "cannot access node by index\r\n"
            "-- not an array_t");

    auto ptr = std::get<std::shared_ptr<array_t>>(_value);
    if (index >= ptr->size())
        throw std::out_of_range(
            "index out of range\r\n"
            "-- array_t size is " +
            std::to_string(ptr->size()));

    return (*ptr)[index];
}

auto json_node::operator[](const char* key) -> json_node& {
    return (*this)[string{key}];
}

auto json_node::operator[](const string& key) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::runtime_error(
            "cannot access node by key\r\n"
            "-- not an object_t");

    auto ptr = std::get<std::shared_ptr<object_t>>(_value);
    if (!ptr->contains(key)) ptr->emplace(key, json_node{});
    return (*ptr)[key];
}

auto json_node::operator<<(const json_node& node) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\r\n"
            "-- not an array_t");

    std::get<std::shared_ptr<array_t>>(_value)->push_back(node);
    return *this;
}

auto json_node::operator<<(json_node&& node) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\r\n"
            "-- not an array_t");

    std::get<std::shared_ptr<array_t>>(_value)->push_back(std::move(node));
    return *this;
}

auto json_node::operator<<(const entry_t& entry) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\r\n"
            "-- not an object_t");

    std::get<std::shared_ptr<object_t>>(_value)->try_emplace(entry.first,
                                                             entry.second);
    return *this;
}

auto json_node::operator<<(entry_t&& entry) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\r\n"
            "-- not an object_t");

    std::get<std::shared_ptr<object_t>>(_value)->try_emplace(
        entry.first, std::move(entry.second));
    return *this;
}

auto json_node::dump(size_t indent, size_t level) const noexcept -> string {
    if (std::holds_alternative<void*>(_value))
        return "null";
    else if (std::holds_alternative<bool>(_value))
        return (bool)(*this) ? "true" : "false";
    else if (std::holds_alternative<int>(_value))
        return std::to_string((int)(*this));
    else if (std::holds_alternative<float>(_value))
        return std::to_string((float)(*this));
    else if (std::holds_alternative<std::shared_ptr<string>>(_value))
        return "\"" + (string)(*this) + "\"";
    else if (std::holds_alternative<std::shared_ptr<array_t>>(_value)) {
        string retval{"["};
        if (indent != 0) retval += "\r\n";

        for (const auto& entry : (array_t)(*this)) {
            if (indent != 0) retval += pad_left("", indent * level);
            retval += entry.dump(indent, level + 1) + ", ";
            if (indent != 0) retval += "\r\n";
        }

        if (((array_t)(*this)).size() == 0)
            retval.erase(retval.end() - 2, retval.end());
        else
            retval.erase(retval.end() - 4);

        if (indent != 0)
            retval += pad_left(
                "", indent * (level == 0 || ((array_t)(*this)).size() == 0
                                  ? 0
                                  : level - 1));
        return retval + "]";
    } else if (std::holds_alternative<std::shared_ptr<object_t>>(_value)) {
        string retval{"{"};
        if (indent != 0) retval += "\r\n";

        for (const auto& [key, value] : (object_t)(*this)) {
            retval += pad_left(
                "\"" + key + "\": " + value.dump(indent, level + 1) + ", ",
                indent * level);
            if (indent != 0) retval += "\r\n";
        }

        if (((object_t)(*this)).size() == 0)
            retval.erase(retval.end() - 2, retval.end());
        else
            retval.erase(retval.end() - 4);

        if (indent != 0)
            retval += pad_left(
                "", indent * (level == 0 || ((object_t)(*this)).size() == 0
                                  ? 0
                                  : level - 1));
        return retval + "}";
    }

    return "";
}

auto json_node::set_null() noexcept -> json_node& {
    _value = nullptr;
    return *this;
}

auto json_node::is_null() const noexcept -> bool {
    return std::holds_alternative<void*>(_value);
}
}  // namespace json
