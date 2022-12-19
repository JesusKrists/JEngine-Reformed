#include <catch2/catch_test_macros.hpp>

#include "lib.hpp"

TEST_CASE("Name is JEngine3D", "[library]")
{
    auto const lib = library {};
    REQUIRE(lib.name == "JEngine3D");
}
