#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include "Base.hpp"

enum class TestEnum
{
    ZERO,
    ONE,
    TWO,
    COUNT
};

TEST_CASE(  // NOLINT(cert-err58-cpp,
            // cppcoreguidelines-avoid-non-const-global-variables)
    "Test Base macros constexpr",
    "[Base]")
{
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}