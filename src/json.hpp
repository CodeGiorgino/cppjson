#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <variant>
#include <vector>

namespace json {
class json_node;
using json_node_ptr = std::shared_ptr<json_node>;

using array_t = std::vector<json_node>;
using object_t = std::map<const char*, json_node>;

using value_t =
    std::variant<void*, bool, int, float, std::string, array_t, object_t>;

class json_node final {
   public:
    enum class value_tag : uint8_t {
        JsonNull,
        JsonBool,
        JsonInt,
        JsonFloat,
        JsonString,
        JsonArray,
        JsonObject
    };

   public:
    json_node();
    json_node(const char* key, value_tag tag, value_t value);

    [[nodiscard]]
    auto key() const noexcept -> const char*;
    [[nodiscard]]
    auto tag() const noexcept -> value_tag;
    [[nodiscard]]
    auto value() const noexcept -> value_t;

   private:
    const char* _key{};
    value_tag _tag{json_node::value_tag::JsonNull};
    value_t _value{(void*)NULL};
};

class json_doc final {
   public:
    json_doc() = default;
    ~json_doc() = default;

    json_doc(std::filesystem::path filepath);
    json_doc(json_node root);
    json_doc(json_node&& root);

    [[nodiscard]]
    auto load_file(std::filesystem::path filepath) -> bool;
    auto load_root(json_node root) -> void;
    auto load_root(json_node&& root) -> void;

   private:
    json_node_ptr _root{};
};
}  // namespace json
