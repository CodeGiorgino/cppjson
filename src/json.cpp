#include "json.hpp"

#include <format>
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

node::node() noexcept : _tag(node_tag::JsonNull), _value((void*)NULL) {}

auto node::tag() const noexcept -> node_tag {
    return _tag;
}

auto node::at(uint idx) -> node& {
    if (_tag != node_tag::JsonArray)
        throw node_exception("cannot access non-array nodes items");

    auto& value{std::get<array>(_value)};
    if (idx >= value.size())
        throw std::out_of_range(
            std::format("index out of range: node field size {} ({} was given)",
                        value.size(), idx));

    return value[idx];
}

auto node::at(uint idx) const -> const node& {
    if (_tag != node_tag::JsonArray)
        throw node_exception("cannot access non-array nodes items");

    const auto& value{std::get<array>(_value)};
    if (idx >= value.size())
        throw std::out_of_range(
            std::format("index out of range: node field size {} ({} was given)",
                        value.size(), idx));

    return value[idx];
}

auto node::field(std::string key) -> node& {
    if (_tag != node_tag::JsonObject)
        throw node_exception("cannot access non-object nodes fields");

    auto& value{std::get<object>(_value)};
    if (!value.contains(key))
        throw std::out_of_range(std::format("key `{}` not in dictionary", key));

    return value.at(key);
}

auto node::field(std::string key) const -> const node& {
    if (_tag != node_tag::JsonObject)
        throw node_exception("cannot access non-object nodes fields");

    const auto& value{std::get<object>(_value)};
    if (!value.contains(key))
        throw std::out_of_range(std::format("key `{}` not in dictionary", key));

    return value.at(key);
}
}  // namespace json

/* json_doc implementation */
namespace json {
doc::doc(const char* filepath) {
    UNUSED(filepath);
    TODO("json::doc::doc(const char*)");
}

doc::doc(node root) noexcept : _root(std::make_shared<node>(root)) {}
doc::doc(node&& root) noexcept
    : _root(std::make_shared<node>(std::move(root))) {}

auto doc::load_file(const char* filepath) noexcept -> bool {
    UNUSED(filepath);
    TODO("json::doc::load_file(const char*)");
}

auto doc::load_root(node root) noexcept -> void {
    _root = std::make_shared<node>(root);
}

auto doc::load_root(node&& root) noexcept -> void {
    _root = std::make_shared<node>(std::move(root));
}

auto doc::dump() const noexcept -> std::string {
    TODO("json::doc::dump()");
}
}  // namespace json
