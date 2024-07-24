#pragma once

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

struct json_value : json_node {
    json_value() = default;
    json_value(std::string literal)
        : _literal(literal) {}

    ~json_value() = default;

    auto get_literal() noexcept -> std::string;
    auto try_get_int() -> std::optional<int>;
    auto try_get_float() -> std::optional<float>;
    auto try_get_bool() noexcept -> std::optional<bool>;

    private:
    std::string _literal{};
};

struct json_object : json_node {
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

struct json_array : json_node {
    json_array() = default;
    json_array(std::vector<json_node> childrens)
        : _childrens(childrens) {};

    ~json_array() = default;

    auto operator[](size_t index) noexcept -> std::optional<json_node*>;
    auto size() const noexcept -> size_t;
    auto push_back(json_node value) noexcept -> json_node*;

    private:
    std::vector<json_node> _childrens{};
};
}  // namespace json
