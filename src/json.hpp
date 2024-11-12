#pragma once

#include <cstddef>
#include <exception>
#include <format>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

namespace json {
class node_exception final : public std::exception {
   public:
    node_exception(const char* msg) : _msg(msg) {}
    auto what() const noexcept -> const char* {
        return _msg;
    }

   private:
    const char* _msg{};
};

class node;
using node_ptr = std::shared_ptr<node>;

using array = std::vector<node>;
using object = std::map<const char*, node>;

using value_t =
    std::variant<void*, bool, int, float, std::string, array, object>;

template <class Tp>
concept is_node_convertible = std::is_constructible_v<value_t, Tp>;

enum class node_tag : uint {
    JsonNull,
    JsonBool,
    JsonInt,
    JsonFloat,
    JsonString,
    JsonArray,
    JsonObject
};

class node final {
   public:
    node() noexcept;
    template <is_node_convertible Tp>
    node(Tp value) {
        _set_value(value);
    }

    node(const node& other) noexcept : _tag(other._tag), _value(other._value) {}
    auto operator=(const node& other) noexcept -> node&;
    auto operator=(node&& other) noexcept -> node&;

    [[nodiscard]] auto tag() const noexcept -> node_tag;

    template <is_node_convertible Tp>
    [[nodiscard]] auto value() const -> Tp {
        return std::get<Tp>(_value);
    }

    auto at(uint idx) -> node&;
    auto at(uint idx) const -> const node&;
    auto field(const char* key) -> node&;
    auto field(const char* key) const -> const node&;

   private:
    template <is_node_convertible Tp>
    auto _set_value(Tp value) -> void {
        if (std::is_same_v<void*, Tp>) {
            _tag = node_tag::JsonNull;
            _value = (void*)NULL;
        } else if (std::is_same_v<bool, Tp>) {
            _tag = node_tag::JsonBool;
            _value = value;
        } else if (std::is_same_v<int, Tp>) {
            _tag = node_tag::JsonInt;
            _value = value;
        } else if (std::is_same_v<float, Tp>) {
            _tag = node_tag::JsonFloat;
            _value = value;
        } else if (std::is_same_v<std::string, Tp>) {
            _tag = node_tag::JsonString;
            _value = value;
        } else if (std::is_same_v<array, Tp>) {
            _tag = node_tag::JsonArray;
            _value = value;
        } else if (std::is_same_v<object, Tp>) {
            _tag = node_tag::JsonObject;
            _value = value;
        } else {
            throw std::logic_error(
                std::format(
                    "unreachable: constructor for type {} is not implemented",
                    typeid(Tp).name())
                    .c_str());
        }
    }

   private:
    node_tag _tag{node_tag::JsonNull};
    value_t _value{(void*)NULL};
};

class doc final {
   public:
    doc() = default;
    ~doc() = default;

    doc(const char* filepath);
    doc(node root) noexcept;
    doc(node&& root) noexcept;

    [[nodiscard]]
    auto load_file(const char* filepath) noexcept -> bool;
    auto load_root(node root) noexcept -> void;
    auto load_root(node&& root) noexcept -> void;

    [[nodiscard]]
    auto dump() const noexcept -> std::string;

   private:
    node_ptr _root{};
};
}  // namespace json
