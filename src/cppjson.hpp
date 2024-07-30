#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace json {
/* Forward declaration */
struct json_node;

/* Helper typedefs */
typedef std::variant<size_t, std::string> key_t;
typedef std::pair<key_t, json_node> entry_t;
typedef std::vector<entry_t> object_t;

/* Json definition */
struct json_node final {
    /* Constructors */
    json_node() noexcept;
    json_node(bool value) noexcept;
    json_node(int value) noexcept;
    json_node(float value) noexcept;
    json_node(const char *value) noexcept;
    json_node(std::string value) noexcept;
    json_node(object_t value) noexcept;
    json_node(const json_node &node) noexcept;

    ~json_node() noexcept;

    /* Type cast overload */
    operator bool() const;
    operator int() const;
    operator float() const;
    operator std::string() const;
    operator object_t() const;

    /**
     * @brief Access the specified object node
     *
     * @param key The key of the node
     * @return The node reference
     */
    auto operator[](const key_t &key) -> json_node &;

    auto operator=(const json_node &node) noexcept -> json_node &;
    auto operator=(const bool &value) noexcept -> json_node &;
    auto operator=(const int &value) noexcept -> json_node &;
    auto operator=(const float &value) noexcept -> json_node &;
    auto operator=(const char *value) noexcept -> json_node &;
    auto operator=(const std::string &value) noexcept -> json_node &;
    auto operator=(const object_t &value) noexcept -> json_node &;

    /* Function members */
    /**
     * @brief Dump the node
     *
     * @param indent The indentation level to follow (if 0 is provided, print on
     * one line)
     * @return The serialized node
     */
    auto dump(size_t indent) const noexcept -> std::string;

    /**
     * @brief Set the object to hold a null value
     *
     * @return The node reference
     */
    auto set_null() noexcept -> json_node &;

    /**
     * @brief Check if the value is set to null
     *
     * @return If the value is set to null
     */
    auto is_null() const noexcept -> bool;

   private:
    /* Json value types */
    enum class enum_value_type : size_t {
        json_nil,
        json_bool,
        json_int,
        json_float,
        json_string,
        json_object
    };

    /* The current value type */
    enum_value_type _type;

    /* The node value */
    std::variant<void *, bool, int, float, std::string *, object_t *> _value;
};
}  // namespace json
