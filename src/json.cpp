#include "json.hpp"

#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#define UNUSED(var) (void)var
#define TODO(name)          \
    throw std::logic_error( \
        std::format("Function not implemented yet: `{}`", name))

/* json_node implementation */
namespace json {
auto node::operator=(const node& other) noexcept -> node& {
    _tag = other._tag;
    _value = other._value;
    return *this;
}

auto node::operator=(node&& other) noexcept -> node& {
    _tag = other._tag;
    _value = std::move(other._value);
    return *this;
}

node::node() : _tag(node_tag::JsonNull), _value((void*)NULL) {}

auto node::tag() const noexcept -> node_tag {
    return _tag;
}

auto node::operator[](size_t idx) -> node& {
    if (_tag != node_tag::JsonArray)
        throw std::runtime_error(
            std::format("cannot access node value field by index of type `{}`",
                        typeid(idx).name()));

    auto& value{std::get<array>(_value)};
    if (idx >= value.size())
        throw std::out_of_range(
            std::format("index out of range: node field size {} ({} was given)",
                        value.size(), idx)
                .c_str());

    return value[idx];
}

auto node::operator[](size_t idx) const -> const node& {
    if (_tag != node_tag::JsonArray)
        throw std::runtime_error(
            std::format("cannot access node value field by index of type `{}`",
                        typeid(idx).name()));

    const auto& value{std::get<array>(_value)};
    if (idx >= value.size())
        throw std::out_of_range(
            std::format("index out of range: node field size {} ({} was given)",
                        value.size(), idx)
                .c_str());

    return value[idx];
}

auto node::operator[](const char* key) -> node& {
    if (_tag != node_tag::JsonObject)
        throw std::runtime_error(
            std::format("cannot access node value field by key of type `{}`",
                        typeid(key).name()));

    auto& value{std::get<object>(_value)};
    if (not value.contains(key))
        throw std::out_of_range(
            std::format("key `{}` not in dictionary", key).c_str());

    return value.at(key);
}

auto node::operator[](const char* key) const -> const node& {
    if (_tag != node_tag::JsonObject)
        throw std::runtime_error(
            std::format("cannot access node value field by key of type `{}`",
                        typeid(key).name()));

    const auto& value{std::get<object>(_value)};
    if (not value.contains(key))
        throw std::out_of_range(
            std::format("key `{}` not in dictionary", key).c_str());

    return value.at(key);
}
}  // namespace json

/* json_doc implementation */
namespace json {
doc::doc(std::filesystem::path filepath) {
    UNUSED(filepath);
    TODO("json::doc::doc(std::filesystem::path)");
}

auto doc::load_file(std::filesystem::path filepath) -> bool {
    UNUSED(filepath);
    TODO("json::doc::load_file(std::filesystem::path)");
}

doc::doc(node root) : _root(std::make_shared<node>(root)) {}
doc::doc(node&& root) : _root(std::make_shared<node>(std::move(root))) {}
}  // namespace json
