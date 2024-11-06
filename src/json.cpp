#include "json.hpp"

#include <filesystem>
#include <memory>
#include <stdexcept>

#define UNUSED(var) (void)var
#define TODO() throw std::logic_error("Function not implemented yet")

/* json_node implementation */
namespace json {
json_node::json_node()
    : _key(NULL), _tag(value_tag::JsonNull), _value((void*)NULL) {}

json_node::json_node(const char* key, value_tag tag, value_t value)
    : _key(key), _tag(tag), _value(value) {}

auto json_node::key() const noexcept -> const char* {
    return _key;
}

auto json_node::tag() const noexcept -> value_tag {
    return _tag;
}

auto json_node::value() const noexcept -> value_t {
    return _value;
}
}  // namespace json

/* json_doc implementation */
namespace json {
json_doc::json_doc(std::filesystem::path filepath) {
    UNUSED(filepath);
    TODO();
}

json_doc::json_doc(json_node root) : _root(std::make_shared<json_node>(root)) {}

json_doc::json_doc(json_node&& root)
    : _root(std::make_shared<json_node>(std::move(root))) {}

auto json_doc::load_file(std::filesystem::path filepath) -> bool {
    UNUSED(filepath);
    TODO();
}
}  // namespace json
