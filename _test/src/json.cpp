#include "../../build/include/json.hpp"

#include <sys/types.h>

#include <cstring>
#include <format>
#include <functional>
#include <iostream>
#include <variant>
#include <vector>

auto log_info(const char* msg, uint line) noexcept -> void {
    std::cout << std::format("> {}:{}: info: {}", __FILE__, line, msg)
              << std::endl;
}

#define TEST_OK() (log_info("test \033[1;32mOK\033[0m", __LINE__), true)
#define TEST_ERROR() (log_info("test \033[1;31mFAILED\033[0m", __LINE__), false)

static std::vector<std::function<bool()>> tests{
    [] {
        auto node = json::json_node();
        if (node.key() != NULL) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonNull)
            return TEST_ERROR();
        if (not std::holds_alternative<void*>(node.value()))
            return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        auto node = json::json_node(NULL, json::json_node::value_tag::JsonNull,
                                    (void*)NULL);
        if (node.key() != NULL) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonNull)
            return TEST_ERROR();
        if (not std::holds_alternative<void*>(node.value()))
            return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        bool value{true};
        auto node = json::json_node(
            "bool", json::json_node::value_tag::JsonBool, value);
        if (strcmp(node.key(), "bool") != 0) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonBool)
            return TEST_ERROR();
        if (not std::holds_alternative<bool>(node.value())) return TEST_ERROR();
        if (std::get<bool>(node.value()) != value) return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        int value{69};
        auto node =
            json::json_node("int", json::json_node::value_tag::JsonInt, value);
        if (strcmp(node.key(), "int") != 0) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonInt)
            return TEST_ERROR();
        if (not std::holds_alternative<int>(node.value())) return TEST_ERROR();
        if (std::get<int>(node.value()) != value) return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        float value{420.f};
        auto node = json::json_node(
            "float", json::json_node::value_tag::JsonFloat, value);
        if (strcmp(node.key(), "float") != 0) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonFloat)
            return TEST_ERROR();
        if (not std::holds_alternative<float>(node.value()))
            return TEST_ERROR();
        if (std::get<float>(node.value()) != value) return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        std::string value{"test string"};
        auto node = json::json_node(
            "string", json::json_node::value_tag::JsonString, value);
        if (strcmp(node.key(), "string") != 0) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonString)
            return TEST_ERROR();
        if (not std::holds_alternative<std::string>(node.value()))
            return TEST_ERROR();
        if (std::get<std::string>(node.value()) != value) return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        json::array_t value{};
        auto node = json::json_node(
            "array_t", json::json_node::value_tag::JsonArray, value);
        if (strcmp(node.key(), "array_t") != 0) return TEST_ERROR();
        if (node.tag() != json::json_node::value_tag::JsonArray)
            return TEST_ERROR();
        if (not std::holds_alternative<json::array_t>(node.value()))
            return TEST_ERROR();
        return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        { return TEST_ERROR(); };
    },
};

auto main(void) -> int {
    std::cout << "----------[ Running tests ]----------" << std::endl;

    uint errorCount{0};
    for (const auto& test : tests) {
        errorCount += (uint)!test();
    }

    std::cout << "-------------------------------------" << std::endl
              << "Test suite report:" << std::endl
              << "  Completed:  " << tests.size() << std::endl
              << "  Errors:     " << errorCount
              << std::format(" ({}%)", errorCount * 100.f / tests.size())
              << std::endl
              << "-------------------------------------" << std::endl;

    return 0;
}
