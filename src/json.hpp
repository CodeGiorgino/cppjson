#pragma once

#include <any>
#include <filesystem>
#include <map>
#include <memory>

namespace json {
class json_field;
using json_field_ptr = std::shared_ptr<json_field>;

class json_field {
   public:
    json_field(const char* key, std::any value);
    json_field(const char* key, std::initializer_list<json_field> fields);

   private:
    const char* _key{};
    std::any _value{};
    std::map<const char*, json_field_ptr> _fields{};
};

class json_doc final {
   public:
    json_doc() = default;
    json_doc(std::filesystem::path filepath);
    json_doc(json_field root);

   private:
    json_field_ptr _root{};
};
}  // namespace json
