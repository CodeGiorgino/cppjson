#include "json.hpp"

namespace json {
json_field::json_field(const char* key, std::any value)
    : _key(key), _value(value) {}

json_field::json_field(const char* key,
                       std::initializer_list<json_field> fields)
    : _key(key) {
    for (const auto& field : fields) {
        if (_fields.contains(field._key))
            throw std::invalid_argument(
                std::format("error: cannot insert multiple fields with the "
                            "same key: `{}`",
                            field._key));

        _fields.emplace(_key, std::make_shared<json_field>(field));
    }
}

json_doc::json_doc(json_field root)
    : _root(std::make_shared<json_field>(root)) {}
}  // namespace json
