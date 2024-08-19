# cppjson
Small c++ library to interact with json file

## Installation
Clone the repository and execute the build script, then just copy the build directory in to your
project and link the library

> [reb](https://github.com/CodeGiorgino/reb) build is supported, just run `reb run rel`

## Usage
### Types
Supported json values:
- `(void*)nullptr`
- `bool          `
- `int           `
- `float         `
- `std::string   `
- `array_t       `
- `object_t      `

Additional types:
| Name | Underlying type |
| :--- | :-------------- |
| array_t  | `std::vector<json_node>                    ` |
| entry_t  | `std::pair<std::string, json_node>         ` |
| object_t | `std::unordered_map<std::string, json_node>` |

### Insert nodes
You can insert nodes to `array_t` nodes using the overloaded `operator<<` which expects a `json_node` \
You can insert nodes to `object_t` nodes using the overloaded `operator<<` which expects a `entry_t` \
In every other case, a `runtime_error` is thrown

### Access nodes
You can access nodes of `array_t` nodes using the overloaded `operator[]` which expects a `size_t` \
You can access nodes of `object_t` nodes using the overloaded `operator[]` which expects a `std::string`,
if no node was found a new one is created with value `(void*)nullptr` \
In every other case, a `runtime_error` is thrown

### Access nodes' value
You can access a node's value by casting it to the contained type \
Supported casts:
- `bool       `
- `int        ` 
- `float      `
- `std::string`
- `array_t    `
- `object_t   ` 


### Example
Usage example from a json string:
```c++
#include <ios>
#include <iostream>

#include "deps/parser.hpp"
#include "desp/cppjson.hpp"

using std::literals::string_literals::operator""s;

auto main(void) -> int {
    const auto raw = R"({
    "int_value": 1,
    "nested": {
        "string_value": "hello"
    }
})";

    auto root = json::parser::deserialize(raw);
    std::cout << "int_value: " << (int)root["int_value"s] << std::endl;
    std::cout << "string_value: "
              << (std::string)root["nested"s]["string_value"s] << std::endl;

    root["new_bool_value"s] = true;
    std::cout << std::boolalpha
              << "new_bool_value: " << (bool)root["new_bool_value"s]
              << std::endl;
    return 0;
}
```
