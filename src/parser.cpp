#include "parser.hpp"

#include <cctype>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>

#include "cppjson.hpp"

namespace json {
namespace parser {
/**
 * @brief Normalise the json string removing spaces and newlines
 *
 * @param raw The raw json string
 * @return The normalised json string
 */
auto normalize_json_string(const std::string &raw) noexcept -> std::string {
    std::string retval{};
    bool quoted = false;
    for (const auto &ch : raw) {
        if (ch == '"') {
            retval += ch;
            quoted ^= true;
            continue;
        }

        if (isspace(ch)) {
            if (quoted) retval += ch;
            continue;
        }

        retval += ch;
    }

    return retval;
}

/**
 * @brief Get the first nested node in the array or the object
 *
 * @param The raw json string
 * @return The end index of the nested node and the node itself
 */
auto get_first_nested_node(const std::string &raw)
    -> std::pair<size_t, std::string> {
    if (raw.empty())
        throw std::invalid_argument("cannot find nested node in empty string");
    if (raw[0] != '{' && raw[0] != '[')
        throw std::invalid_argument(
            "can only find nested node in objects or arrays");

    char ch{};
    size_t brackets_count{}, start{}, end{};
    for (size_t i{}; i < raw.length(); ++i) {
        ch = raw[i];
        if ((ch == '{' || ch == '[') && ++brackets_count == 2)
            start = i;

        else if ((ch == '}' || ch == ']') && --brackets_count == 1) {
            end = i + 1;
            break;
        }
    }

    if (start == 0 || end == 0)
        throw std::invalid_argument("cannot find nested node in json string");

    return {end, raw.substr(start, end - start)};
}

auto deserialize(std::string raw) -> json_node {
    if (raw.length() == 0)
        throw std::invalid_argument("cannot deserialize an empty string");

    raw = normalize_json_string(raw);

    if (raw[0] == '{') {
        json_node retval{object_t{}};
        std::smatch match{};

        while (std::regex_search(raw, match,
                                 std::regex("^\\{\"(.+?)\":(.+?)(,.+)?\\}$"))) {
            if (match[2].str()[0] == '{' || match[2].str()[0] == '[') {
                const auto [end, raw_node] = get_first_nested_node(raw);
                retval[match[1]] = deserialize(raw_node);
                raw = "{" + raw.substr(end);
            }

            else {
                retval[match[1]] = deserialize(match[2]);
                raw = "{" + match[3].str() + "}";
            }

            if (raw[1] == ',') raw.erase(1, 1);
        };

        if (raw != "{}")
            throw std::invalid_argument("cannot deserialize the json string");

        return retval;
    }

    else if (raw[0] == '[') {
        json_node retval{object_t{}};
        size_t array_size = 0;
        std::smatch match{};

        while (
            std::regex_search(raw, match, std::regex("^\\[(.+?)(,.+)?\\]$"))) {
            if (match[1].str()[0] == '{' || match[1].str()[0] == '[') {
                const auto [end, raw_node] = get_first_nested_node(raw);
                retval[array_size++] = deserialize(raw_node);
                raw = "[" + raw.substr(end + 1);
            }

            else {
                retval[array_size++] = deserialize(match[1]);
                raw = "[" + match[2].str() + "]";
            }

            if (raw[1] == ',') raw.erase(1, 1);
        };

        if (raw != "[]")
            throw std::invalid_argument("cannot deserialize the json string");

        return json_node{retval};
    }

    else {
        if (raw == "true")
            return json_node(true);

        else if (raw == "false")
            return json_node(false);

        else if (std::regex_match(raw, std::regex("^-?[0-9]+$")))
            return json_node(std::stoi(raw));

        else if (std::regex_match(raw, std::regex("^-?[0-9]+\\.[0-9]+$")))
            return json_node(std::stof(raw));

        else if (std::smatch match;
                 std::regex_search(raw, match, std::regex("^\"(.+)\"$")))
            return json_node(match[1]);
        else
            throw std::invalid_argument("cannot parse '" + raw +
                                        "' to json value");
    }

    throw std::invalid_argument("cannot deserialize the json string");
}
}  // namespace parser
}  // namespace json
