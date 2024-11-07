#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

namespace json {
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
    node();

    template <is_node_convertible Tp>
    node(Tp value) {
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

    [[nodiscard]] auto tag() const noexcept -> node_tag;

    template <is_node_convertible Tp>
    [[nodiscard]] auto value() const -> Tp {
        return std::get<Tp>(_value);
    }

   private:
    node_tag _tag{node_tag::JsonNull};
    value_t _value{(void*)NULL};
};

class doc final {
   public:
    doc() = default;
    ~doc() = default;

    doc(std::filesystem::path filepath);
    doc(node root);
    doc(node&& root);

    [[nodiscard]]
    auto load_file(std::filesystem::path filepath) -> bool;
    auto load_root(node root) -> void;
    auto load_root(node&& root) -> void;

   private:
    node_ptr _root{};
};
}  // namespace json
