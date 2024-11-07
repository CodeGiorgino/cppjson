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
        auto node = json::node();
        if (node.tag() != json::node_tag::JsonNull) return TEST_ERROR();
        try {
            if (node.value<void*>() != NULL) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        auto node = json::node(json::node_tag::JsonNull, (void*)NULL);
        try {
            if (node.value<void*>() != NULL) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        bool value{true};
        auto node = json::node(json::node_tag::JsonBool, value);
        try {
            if (node.value<bool>() != value) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        int value{69};
        auto node = json::node(json::node_tag::JsonInt, value);
        try {
            if (node.value<int>() != value) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        float value{420.f};
        auto node = json::node(json::node_tag::JsonFloat, value);
        try {
            if (node.value<float>() != value) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        std::string value{"test string"};
        auto node = json::node(json::node_tag::JsonString, value);
        try {
            if (node.value<std::string>() != value) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] { return TEST_ERROR(); },
    [] { return TEST_ERROR(); },
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
