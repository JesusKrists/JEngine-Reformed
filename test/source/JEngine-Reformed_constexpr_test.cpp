#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include "Base.hpp"

enum struct TestEnum
{
    ZERO,
    ONE,
    TWO,
    COUNT
};

TEST_CASE("Test Base macros constexpr", "[Base]")
{
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}