#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
    auto const LIB = Library{};
    auto const MESSAGE = "Hello from " + LIB.m_name + "!";
    std::cout << MESSAGE << '\n';

    return 0;
}
