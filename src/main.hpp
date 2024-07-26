#pragma once

#include <any>
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace json {
struct json_node {
    json_node() = default;
    virtual ~json_node() = default;
};

struct json_value final : json_node {
    json_value() = default;
    json_value(std::any value)
        : _value(value) {}

    ~json_value() = default;

    auto value() const noexcept -> std::any;

    private:
    std::any _value{};
};

struct json_object final : json_node {
    json_object() = default;
    json_object(std::unordered_map<std::string, json_node> childrens)
        : _childrens(childrens) {}

    ~json_object() = default;

    auto operator[](std::string key) noexcept -> std::optional<json_node*>;
    auto size() const noexcept -> size_t;
    auto try_emplace(std::string key, json_node value) noexcept
        -> std::optional<json_node*>;

   private:
    std::unordered_map<std::string, json_node> _childrens{};
};

struct json_array final : json_node {
    json_array() = default;
    json_array(std::vector<json_node> childrens) : _childrens(childrens){};

    ~json_array() = default;

    auto operator[](size_t index) noexcept -> std::optional<json_node*>;
    auto size() const noexcept -> size_t;
    auto push_back(json_node value) noexcept -> json_node*;

   private:
    std::vector<json_node> _childrens{};
};
}  // namespace json
