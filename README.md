# cppjson
Small c++ library to interact with json file

## Table of contents
- [Installation](#installation)
- [Usage](#usage)

### Installation
Clone the repository and execute the build script, then just copy the build directory in to your
project and link the library
```bash
git clone 'https://github.com/CodeGiorgino/cppjson.git'
cd cppjson

./build.sh
cd ..

mkdir test
cp -r cppjson/buil test/deps
cd test

touch main.cpp
tee -a main.cpp << END
#include "deps/cppjson.hpp"

auto int main(void) -> int {
    return 0;
}
END

g++ -Wall -Wextra -std=c++23 -o test main.cpp deps/cppjson.a
./test
```

### Usage
Supported json values:

Name        | Underlying type
:--         | :--
json_nil    | `(void*)nullptr`
json_bool   | `bool`
json_int    | `int`
json_float  | `float`
json_string | `std::string`
json_object | `object_t`

Additional types:

Name     | Underlying type
:--      | :--
key_t    | `std::variant<size_t, std::string>`
entry_t  | `std::pair<key_t, json_node>`
object_t | `std::vector<entry_t>`

Usage example from a json string:

```c++
#include <ios>
#include <iostream>

#include "deps/parser.hpp"
#include "desp/cppjson.hpp"

auto main(void) -> int {
    const auto raw = R"({
    "int_value": 1,
    "nested": {
        "string_value": "hello"
    }
})";

    auto root = json::parser::deserialize(raw);
    std::cout << "int_value: " << (int)root[{"int_value"}] << std::endl;
    std::cout << "string_value: "
              << (std::string)root[{"nested"}][{"string_value"}] << std::endl;

    root[{"new_bool_value"}] = true;
    std::cout << std::boolalpha
              << "new_bool_value: " << (bool)root[{"new_bool_value"}]
              << std::endl;
    return 0;
}
```
