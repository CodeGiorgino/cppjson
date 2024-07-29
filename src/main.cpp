#include "main.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

namespace json {
json_node::json_node() noexcept : _type(enum_value_type::json_nil) {}

json_node::json_node(bool value) noexcept
    : _type(enum_value_type::json_bool), _value_bool(value) {}

json_node::json_node(int value) noexcept
    : _type(enum_value_type::json_int), _value_int(value) {}

json_node::json_node(float value) noexcept
    : _type(enum_value_type::json_float), _value_float(value) {}

json_node::json_node(const char* value) noexcept
    : _type(enum_value_type::json_string), _value_string(value) {}

json_node::json_node(std::string value) noexcept
    : _type(enum_value_type::json_string), _value_string(value) {}

json_node::json_node(array_t value) noexcept
    : _type(enum_value_type::json_array), _value_array(value) {}

json_node::json_node(object_t value) noexcept
    : _type(enum_value_type::json_object), _value_object(value) {}

json_node::json_node(const json_node& node) noexcept {
    *this = node;
}

json_node::operator bool() const {
    if (_type != enum_value_type::json_bool) throw std::bad_cast();
    return _value_bool;
}

json_node::operator int() const {
    if (_type != enum_value_type::json_int) throw std::bad_cast();
    return _value_int;
}

json_node::operator float() const {
    if (_type != enum_value_type::json_float) throw std::bad_cast();
    return _value_float;
}

json_node::operator std::string() const {
    if (_type != enum_value_type::json_string) throw std::bad_cast();
    return _value_string;
}

json_node::operator array_t() const {
    if (_type != enum_value_type::json_array) throw std::bad_cast();
    return _value_array;
}

json_node::operator object_t() const {
    if (_type != enum_value_type::json_object) throw std::bad_cast();
    return _value_object;
}

auto json_node::operator[](const char* key) -> json_node& {
    if (_type != enum_value_type::json_object)
        throw std::runtime_error("cannot access non object nodes by key");

    for (auto& entry : _value_object) {
        if (entry.first == key) {
            return entry.second;
        }
    }

    _value_object.push_back({key, json_node{}});
    return _value_object.back().second;
}

auto json_node::operator[](std::string key) -> json_node& {
    return (*this)[key.c_str()];
}

auto json_node::operator=(const json_node& node) noexcept -> json_node& {
    _type = node._type;

    _value_nil = node._value_nil;
    _value_bool = node._value_bool;
    _value_int = node._value_int;
    _value_float = node._value_float;
    _value_string = node._value_string;
    _value_array = node._value_array;
    _value_object = node._value_object;
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

auto json_node::operator=(const array_t& value) noexcept -> json_node& {
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

auto json_node::is_null() const noexcept -> bool {
    return _type == enum_value_type::json_nil;
}
}  // namespace json
