#include <iostream>  // for operator<<, basic_ostream, cout, ostream
#include <string>  // for allocator, char_traits, operator+, operator<<

#include "lib.hpp"  // for Library

auto main() -> int
{
    auto const LIB = Library{};
    auto const MESSAGE = "Hello from " + LIB.m_name + "!";
    std::cout << MESSAGE << '\n';

    return 0;
}
