#include "json.hpp"

#include <filesystem>
#include <memory>
#include <stdexcept>

#define UNUSED(var) (void)var
#define TODO() throw std::logic_error("Function not implemented yet")

/* json_node implementation */
namespace json {
node::node() : _tag(node_tag::JsonNull), _value((void*)NULL) {}
node::node(node_tag tag, value_t value) : _tag(tag), _value(value) {}

auto node::tag() const noexcept -> node_tag {
    return _tag;
}
}  // namespace json

/* json_doc implementation */
namespace json {
doc::doc(std::filesystem::path filepath) {
    UNUSED(filepath);
    TODO();
}

doc::doc(node root) : _root(std::make_shared<node>(root)) {}

doc::doc(node&& root) : _root(std::make_shared<node>(std::move(root))) {}

auto doc::load_file(std::filesystem::path filepath) -> bool {
    UNUSED(filepath);
    TODO();
}
}  // namespace json
