#include "../../build/include/parser.hpp"

#include <sys/types.h>

#include <cstring>
#include <format>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

auto log_info(const char* msg, uint line) noexcept -> void {
    std::cout << std::format("[?] {}:{}:\tinfo: {}", __FILE__, line, msg)
              << std::endl;
}

auto log_exception(const char* msg) noexcept -> void {
    std::cout << "[!] fatal: unhandled exception: " << std::quoted(msg)
              << std::endl;
}

#define TEST_OK() (log_info("test \033[1;32mOK\033[0m", __LINE__), true)
#define TEST_ERROR() (log_info("test \033[1;31mFAILED\033[0m", __LINE__), false)

static std::vector<std::function<bool()>> tests{
    [] {
        const std::string content{
            R"({
                "null": null
            ])"};

        try {
            const auto _ = json::deserialize(content);
        } catch (const json::invalid_json_exception& ex) {
            return TEST_OK();
        } catch (const std::exception& ex) {
            log_exception(ex.what());
            return TEST_ERROR();
        }
        return TEST_ERROR();
    },
    [] {
        const std::string content{
            R"({
                null": null,
            })"};

        try {
            const auto _ = json::deserialize(content);
        } catch (const json::invalid_json_exception& ex) {
            return TEST_OK();
        } catch (const std::exception& ex) {
            log_exception(ex.what());
            return TEST_ERROR();
        }
        return TEST_ERROR();
    },
    [] {
        const std::string content{
            R"({
                "null": null,
                "bool": [true, false],
                "numbers": {
                    "int": 69,
                    "float": 420.0
                },
                "string": "test string"
        })"};

        try {
            const auto node = json::deserialize(content);
            if (node.field("null").value<void*>() != NULL) return TEST_ERROR();
            const auto& boolValues = node.field("bool");
            if (!boolValues.at(0).value<bool>() ||
                boolValues.at(1).value<bool>())
                return TEST_ERROR();
            const auto& numberValues = node.field("numbers");
            if (numberValues.field("int").value<int>() != 69 ||
                numberValues.field("float").value<float>() != 420.f)
                return TEST_ERROR();
            if (node.field("string").value<std::string>() != "test string")
                return TEST_ERROR();
        } catch (const std::exception& ex) {
            log_exception(ex.what());
            return TEST_ERROR();
        }
        return TEST_OK();
    },
    [] {
        const std::string content{
            R"({
    "list": [
        {
          "dt": 1661857200,
          "sunrise": 1661834187,
          "sunset": 1661882248,
          "temp": {
            "day": 299.66,
            "min": 288.93,
            "max": 299.66,
            "night": 290.31,
            "eve": 297.16,
            "morn": 288.93
          }
        }
    ]
})"};
        try {
            const auto node = json::deserialize(content);
            if (node.field("list")
                    .at(0)
                    .field("temp")
                    .field("max")
                    .value<float>() != 299.66f)
                return TEST_ERROR();
        } catch (const std::exception& ex) {
            log_exception(ex.what());
            return TEST_ERROR();
        }
        return TEST_OK();
    }};

auto main(int argc, char** argv) -> int {
    if (argc > 1) throw std::invalid_argument("unexpected parameters provided");

    std::cout << "----------[ Running tests ]----------" << std::endl;

    uint errorCount{0};
    for (const auto& test : tests) {
        errorCount += (uint)!test();
    }

    std::cout << "-------------------------------------" << std::endl
              << "Test suite report: " << std::quoted(*argv) << std::endl
              << "  Completed:  " << tests.size() << std::endl
              << "  Errors:     " << errorCount
              << std::format(" ({:.2f}%)", errorCount * 100.f / tests.size())
              << std::endl
              << std::endl;

    return 0;
}
