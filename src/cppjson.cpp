#include "cppjson.hpp"

#include <bits/ranges_util.h>

#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <variant>

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

json_node::json_node(const char* value) noexcept {
    _value = std::make_shared<std::string>(value);
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

json_node::json_node(const json_node& node) noexcept {
    if (std::holds_alternative<std::shared_ptr<std::string>>(node._value))
        _value = std::make_shared<std::string>((std::string)node);
    else if (std::holds_alternative<std::shared_ptr<array_t>>(node._value))
        _value = std::make_shared<array_t>((array_t)node);
    else if (std::holds_alternative<std::shared_ptr<object_t>>(node._value))
        _value = std::make_shared<object_t>((object_t)node);
    else
        _value = node._value;
}

json_node::operator bool() const {
    if (auto* ptr = std::get_if<bool>(&_value)) return *ptr;
    throw std::bad_cast();
}

json_node::operator int() const {
    if (auto* ptr = std::get_if<int>(&_value)) return *ptr;
    throw std::bad_cast();
}

json_node::operator float() const {
    if (auto* ptr = std::get_if<float>(&_value)) return *ptr;
    throw std::bad_cast();
}

json_node::operator std::string() const {
    if (!std::holds_alternative<std::shared_ptr<std::string>>(_value))
        throw std::bad_cast();
    return *std::get<std::shared_ptr<std::string>>(_value);
}

json_node::operator array_t() const {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::bad_cast();
    return *std::get<std::shared_ptr<array_t>>(_value);
}

json_node::operator object_t() const {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::bad_cast();
    return *std::get<std::shared_ptr<object_t>>(_value);
}

auto json_node::operator[](const size_t& index) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::runtime_error(
            "cannot access node by index\n"
            "-- not an array_t");

    auto ptr = std::get<std::shared_ptr<array_t>>(_value);
    if (index >= ptr->size())
        throw std::out_of_range(
            "index out of range\n"
            "-- array_t size is " +
            std::to_string(ptr->size()));

    return (*ptr)[index];
}

auto json_node::operator[](const char* key) -> json_node& {
    return (*this)[std::string{key}];
}

auto json_node::operator[](const std::string& key) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::runtime_error(
            "cannot access node by key\n"
            "-- not an object_t");

    auto ptr = std::get<std::shared_ptr<object_t>>(_value);
    if (!ptr->contains(key)) ptr->emplace(key, json_node{});
    return (*ptr)[key];
}

auto json_node::operator<<(const json_node& node) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<array_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\n"
            "-- not an array_t");

    auto ptr = std::get<std::shared_ptr<array_t>>(_value);
    ptr->push_back(node);
    return ptr->back();
}

auto json_node::operator<<(const entry_t& entry) -> json_node& {
    if (!std::holds_alternative<std::shared_ptr<object_t>>(_value))
        throw std::runtime_error(
            "cannot insert node\n"
            "-- not an object_t");

    auto ptr = std::get<std::shared_ptr<object_t>>(_value);
    ptr->try_emplace(entry.first, entry.second);
    return (*ptr)[entry.first];
}

auto json_node::operator=(const json_node& node) noexcept -> json_node& {
    if (std::holds_alternative<std::shared_ptr<std::string>>(node._value))
        _value = std::make_shared<std::string>((std::string)node);
    else if (std::holds_alternative<std::shared_ptr<array_t>>(node._value))
        _value = std::make_shared<array_t>((array_t)node);
    else if (std::holds_alternative<std::shared_ptr<object_t>>(node._value))
        _value = std::make_shared<object_t>((object_t)node);
    else
        _value = node._value;

    return *this;
}

auto json_node::operator=(const bool& value) noexcept -> json_node& {
    _value = value;
    return *this;
}

auto json_node::operator=(const int& value) noexcept -> json_node& {
    _value = value;
    return *this;
}

auto json_node::operator=(const float& value) noexcept -> json_node& {
    _value = value;
    return *this;
}

auto json_node::operator=(const char* value) noexcept -> json_node& {
    _value = std::make_shared<std::string>(value);
    return *this;
}

auto json_node::operator=(const std::string& value) noexcept -> json_node& {
    _value = std::make_shared<std::string>(value);
    return *this;
}

auto json_node::operator=(const array_t& value) noexcept -> json_node& {
    _value = std::make_shared<array_t>(value);
    return *this;
}

auto json_node::operator=(const object_t& value) noexcept -> json_node& {
    _value = std::make_shared<object_t>(value);
    return *this;
}

auto json_node::dump(size_t indent) const noexcept -> std::string {
    (void)indent;
    assert(true && "not implemented exception");
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
