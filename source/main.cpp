#include <cstdint>
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> std::int32_t
{
    auto const LIB = Library{};
    auto const MESSAGE = "Hello from " + LIB.mName + "!";
    std::cout << MESSAGE << '\n';

    return 0;
}
