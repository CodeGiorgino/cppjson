#include "../../build/include/json.hpp"

#include <sys/types.h>

#include <cstring>
#include <format>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>

auto log_info(const char* msg, uint line) noexcept -> void {
    std::cout << std::format("> {}:{}:\tinfo: {}", __FILE__, line, msg)
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
        auto node = json::node((void*)NULL);
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
        bool value{true};
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonBool) return TEST_ERROR();
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
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonInt) return TEST_ERROR();
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
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonFloat) return TEST_ERROR();
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
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonString) return TEST_ERROR();
        try {
            if (node.value<std::string>() != value) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{1};
        try {
            const auto& _ = node.at(0);
        } catch (const json::node_exception&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{1};
        try {
            auto& _ = node.at(0);
        } catch (const json::node_exception&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{1};
        try {
            auto& _ = node.field("first");
        } catch (const json::node_exception&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{1};
        try {
            const auto& _ = node.field("first");
        } catch (const json::node_exception&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{json::array{json::node(), json::node(), json::node()}};
        try {
            auto& _ = node.at(3);
        } catch (const std::out_of_range&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{json::array{json::node(), json::node(), json::node()}};
        try {
            const auto& _ = node.at(3);
        } catch (const std::out_of_range&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{json::object{{"first", json::node()}}};
        try {
            auto& _ = node.field("second");
        } catch (const std::out_of_range&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::node node{json::object{{"first", json::node()}}};
        try {
            const auto& _ = node.field("second");
        } catch (const std::out_of_range&) {
        } catch (...) {
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        json::array value{json::node(), json::node(69), json::node(420.f)};
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonArray) return TEST_ERROR();
        try {
            const auto nodeValue = node.value<json::array>();
            if (value.size() != nodeValue.size()) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            return TEST_ERROR();
        }
        if (node.at(0).value<void*>() != value[0].value<void*>())
            return TEST_ERROR();
        if (node.at(1).value<int>() != value[1].value<int>())
            return TEST_ERROR();
        if (node.at(2).value<float>() != value[2].value<float>())
            return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        json::object value{{"first", json::node()},
                           {"second", json::node(69)},
                           {"third", json::node(420.f)}};
        auto node = json::node(value);
        if (node.tag() != json::node_tag::JsonObject) return TEST_ERROR();
        try {
            const auto nodeValue = node.value<json::object>();
            if (value.size() != nodeValue.size()) return TEST_ERROR();
        } catch (const std::bad_variant_access&) {
            return TEST_ERROR();
        } catch (...) {
            return TEST_ERROR();
        }
        if (node.field("first").value<void*>() != value["first"].value<void*>())
            return TEST_ERROR();
        if (node.field("second").value<int>() != value["second"].value<int>())
            return TEST_ERROR();
        if (node.field("third").value<float>() != value["third"].value<float>())
            return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        auto node = json::node{json::array{json::node{69}, json::node{420.f}}};
        if (node.at(0).value<int>() != 69) return TEST_ERROR();
        if (node.at(1).value<float>() != 420.f) return TEST_ERROR();
        return TEST_OK();
    },
    [] {
        auto value = json::node{json::array{json::node{69}, json::node{420.f}}};
        auto node = value;
        if (node.at(0).value<int>() != 69) return TEST_ERROR();
        if (node.at(1).value<float>() != 420.f) return TEST_ERROR();
        return TEST_OK();
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
              << std::format(" ({:.2f}%)", errorCount * 100.f / tests.size())
              << std::endl
              << std::endl;

    return 0;
}
