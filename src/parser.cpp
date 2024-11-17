#include "parser.hpp"

#include <cctype>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <regex>
#include <string>

#include "json.hpp"

#define UNUSED(var) (void)var
#define TODO(name)          \
    throw std::logic_error( \
        std::format("Function not implemented yet: `{}`", name))

namespace json {
auto parse_array(const std::string& source, uint& idx) -> node;
auto parse_object(const std::string& source, uint& idx) -> node;

auto parse_value(const std::string& source, uint& idx) -> node {
    auto startIdx{idx};

    std::string buf{};
    bool quoted{false};
    while (const auto& ch = source[idx]) {
        if (ch == '"') {
            if (quoted)
                quoted = false;
            else {
                quoted = true;
                startIdx = idx;
            }
        }

        if (!quoted) {
            if (isspace(ch)) {
                idx++;
                continue;
            }

            if (ch == ',' || ch == ']' || ch == '}') break;
            if (ch == '[') return parse_array(source, idx);
            if (ch == '{') return parse_object(source, idx);
        }

        buf += ch;
        idx++;
    }

    if (buf.empty())
        throw invalid_json_exception(std::format(
            "missing or empty object field value at position {}", startIdx));

    if (buf == "null") return node{};
    if (buf == "true") return node{true};
    if (buf == "false") return node{false};
    if (std::regex_match(buf, std::regex("^[+-]?[0-9]+$")))
        return node{std::stoi(buf)};
    if (std::regex_match(buf, std::regex("^[+-]?[0-9]+(?:\\.[0-9]+)?$")))
        return node{std::stof(buf)};
    if (buf[0] == '"') {
        if (quoted)
            throw invalid_json_exception(std::format(
                "unclosed object field value found, opened at position {}",
                startIdx));

        return node{buf.substr(1, buf.length() - 2)};
    }

    throw invalid_json_exception(
        std::format("cannot parse object field value `{}` "
                    "at position {}",
                    buf, startIdx));
}

auto parse_array(const std::string& source, uint& idx) -> node {
    const auto startIdx{idx};

    array retval{};
    uint itemsCount{0};
    bool isClosed{false};
    while (const auto& ch = source[++idx]) {
        if (isspace(ch) || ch == '\r' || ch == '\n') continue;
        if (ch == ',') {
            if (itemsCount != 0) continue;
            throw invalid_json_exception(
                std::format("item separator found with no previous item "
                            "declared at position {}",
                            idx));
        }

        retval.push_back(parse_value(source, idx));
        itemsCount++;

        if (source[idx] == ']') {
            isClosed = true;
            break;
        }
    };

    if (!isClosed)
        throw invalid_json_exception(std::format(
            "unclosed array found, opened at position {}", startIdx));

    return node{retval};
}

auto parse_object(const std::string& source, uint& idx) -> node {
    const auto startIdx{idx};

    object retval{};
    uint itemsCount{0};
    bool isClosed{false};
    while (const auto& ch = source[++idx]) {
        if (isspace(ch) || ch == '\r' || ch == '\n') continue;
        if (ch == ',') {
            if (itemsCount != 0) continue;
            throw invalid_json_exception(
                std::format("item separator found with no previous item "
                            "declared at position {}",
                            idx));
        }

        if (ch != '"')
            throw invalid_json_exception(
                std::format("expected open quote for object key "
                            "declaration, but got `{}` at position {}",
                            ch, idx));

        auto keyStartIdx{idx};
        std::string key{};
        bool quoted{true};
        while (const auto& chKey = source[++idx]) {
            if (chKey == '"') {
                quoted = false;
                break;
            }

            key += chKey;
        }

        if (quoted)
            throw invalid_json_exception(
                std::format("unclosed object key found, opened at position {}",
                            keyStartIdx));

        if (key.empty())
            throw invalid_json_exception(std::format(
                "missing or empty object key at position {}", keyStartIdx));

        while (const auto& ch = source[++idx]) {
            if (isspace(ch)) continue;
            if (ch == ':') break;
            throw invalid_json_exception(std::format(
                "expected field initialiser operator `:`, but got `{}` "
                "at position {}",
                ch, idx));
        }

        if (retval.contains(key))
            throw invalid_json_exception(std::format(
                "duplicate key found in object at position {}: `{}`", idx,
                key));

        retval.emplace(key, parse_value(source, ++idx));
        itemsCount++;

        // FIXME: last child object
        if (source[idx] == '}') {
            isClosed = true;
            break;
        }
    }

    if (!isClosed)
        throw invalid_json_exception(std::format(
            "unclosed object found, opened at position {}", startIdx));

    return node{retval};
}

auto deserialize(const char* filepath) -> node {
    std::filesystem::path path{filepath};
    if (!std::filesystem::exists(path))
        throw invalid_json_exception(
            std::format("cannot find file `{}`", filepath));

    if (!path.has_extension() || path.extension() != ".json")
        throw invalid_json_exception(
            "the file must have the `.json` extension");

    std::ifstream filestream{path};
    if (!filestream)
        throw invalid_json_exception(
            std::format("cannot open file `{}`", filepath));

    const auto filesize{std::filesystem::file_size(path)};
    if (filesize == 0) {
        filestream.close();
        throw invalid_json_exception(
            std::format("the file `{}` is empty", filepath));
    }

    std::string content{};
    std::string line{};
    while (std::getline(filestream, line)) {
        if (line.empty()) continue;
        content += line;
    };

    node root{};
    uint idx{0};
    if (content[0] == '[') {
        root = parse_array(content, idx);
    } else if (content[0] == '{') {
        root = parse_object(content, idx);
    } else {
        filestream.close();
        throw invalid_json_exception(
            std::format("expected array or object declaration as json root, "
                        "but got `{}`",
                        content[0]));
    }

    filestream.close();
    return root;
}

auto deserialize(std::string content) -> node {
    node root{};
    uint idx{0};
    if (content[0] == '[') {
        root = parse_array(content, idx);
    } else if (content[0] == '{') {
        root = parse_object(content, idx);
    } else
        throw invalid_json_exception(
            std::format("expected array or object declaration as json root, "
                        "but got `{}`",
                        content[0]));

    return root;
}
}  // namespace json
