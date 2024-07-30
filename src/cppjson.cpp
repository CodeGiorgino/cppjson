#include "cppjson.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <variant>

namespace json {
json_node::json_node() noexcept {
    set_null();
}

json_node::json_node(bool value) noexcept {
    set_null();
    _type = enum_value_type::json_bool;
    _value = value;
}

json_node::json_node(int value) noexcept {
    set_null();
    _type = enum_value_type::json_int;
    _value = value;
}

json_node::json_node(float value) noexcept {
    set_null();
    _type = enum_value_type::json_float;
    _value = value;
}

json_node::json_node(const char* value) noexcept {
    set_null();
    _type = enum_value_type::json_string;
    _value = new std::string{};

    auto* ptr = std::get<std::string*>(_value);
    *ptr = value;
}

json_node::json_node(std::string value) noexcept {
    set_null();
    _type = enum_value_type::json_string;
    _value = new std::string{};

    auto* ptr = std::get<std::string*>(_value);
    *ptr = value;
}

json_node::json_node(object_t value) noexcept {
    set_null();
    _type = enum_value_type::json_object;
    _value = new object_t{};

    auto* ptr = std::get<object_t*>(_value);
    *ptr = value;
}

json_node::json_node(const json_node& node) noexcept {
    *this = node;
}

json_node::~json_node() noexcept {
    set_null();
}

json_node::operator bool() const {
    if (_type != enum_value_type::json_bool) throw std::bad_cast();
    return std::get<bool>(_value);
}

json_node::operator int() const {
    if (_type != enum_value_type::json_int) throw std::bad_cast();
    return std::get<int>(_value);
}

json_node::operator float() const {
    if (_type != enum_value_type::json_float) throw std::bad_cast();
    return std::get<float>(_value);
}

json_node::operator std::string() const {
    if (_type != enum_value_type::json_string) throw std::bad_cast();
    auto* ptr = std::get<std::string*>(_value);
    return *ptr;
}

json_node::operator object_t() const {
    if (_type != enum_value_type::json_object) throw std::bad_cast();
    auto* ptr = std::get<object_t*>(_value);
    return *ptr;
}

auto json_node::operator[](const key_t& key) -> json_node& {
    if (_type != enum_value_type::json_object) *this = object_t{};

    auto* ptr = std::get<object_t*>(_value);
    if (ptr->size() != 0) {
        if ((*ptr)[0].first.index() != key.index())
            throw std::runtime_error(
                "cannot access node by the key_t provided");

        for (auto& entry : *ptr) {
            switch (key.index()) {
                case 0:  // size_t
                    if (std::get<0>(entry.first) == std::get<0>(key))
                        return entry.second;
                    break;
                case 1:  // std::string
                    if (std::get<1>(entry.first) == std::get<1>(key))
                        return entry.second;
                    break;
                default:
                    throw std::logic_error("unreachable");
            }
        }
    }

    ptr->push_back({key, {}});
    return ptr->back().second;
}

auto json_node::operator=(const json_node& node) noexcept -> json_node& {
    set_null();
    _type = node._type;

    if (_type == enum_value_type::json_string) {
        _value = new std::string;

        auto* ptr = std::get<std::string*>(_value);
        *ptr = *std::get<std::string*>(node._value);
    }

    else if (_type == enum_value_type::json_object) {
        _value = new object_t;

        auto* ptr = std::get<object_t*>(_value);
        *ptr = *std::get<object_t*>(node._value);
    }

    else {
        _value = node._value;
    }

    return *this;
}

auto json_node::operator=(const bool& value) noexcept -> json_node& {
    *this = json_node{value};
    return *this;
}

auto json_node::operator=(const int& value) noexcept -> json_node& {
    *this = json_node{value};
    return *this;
}

auto json_node::operator=(const float& value) noexcept -> json_node& {
    *this = json_node{value};
    return *this;
}

auto json_node::operator=(const char* value) noexcept -> json_node& {
    *this = json_node{std::string(value)};
    return *this;
}

auto json_node::operator=(const std::string& value) noexcept -> json_node& {
    *this = json_node{value};
    return *this;
}

auto json_node::operator=(const object_t& value) noexcept -> json_node& {
    *this = json_node{value};
    return *this;
}

auto json_node::dump(size_t indent) const noexcept -> std::string {
    (void)indent;
    assert(true && "not implemented exception");
    return "";
}

auto json_node::set_null() noexcept -> json_node& {
    if (_type == enum_value_type::json_string) {
        auto* ptr = std::get<std::string*>(_value);
        delete ptr;
    }

    if (_type == enum_value_type::json_object) {
        auto* ptr = std::get<object_t*>(_value);
        delete ptr;
    }

    _type = enum_value_type::json_nil;
    _value = (void*)nullptr;
    return *this;
}

auto json_node::is_null() const noexcept -> bool {
    return _type == enum_value_type::json_nil;
}
}  // namespace json
