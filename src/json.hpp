#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
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
concept NodeConvertible = std::is_constructible_v<value_t, Tp>;

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
    node(node_tag tag, value_t value);

    [[nodiscard]] auto tag() const noexcept -> node_tag;

    template <NodeConvertible Tp>
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
