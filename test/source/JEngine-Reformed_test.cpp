#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include "Base.hpp"

#define JE_ASSERT_BREAK_ON_FAIL false
#include <Assert.hpp>
#include <Logger.hpp>

struct Library
{
    std::string m_name = fmt::format("{}", "JEngine-Reformed");
};

enum struct TestEnum
{
    ZERO,
    ONE,
    TWO,
    COUNT
};

TEST_CASE("Test Base macros", "[Base]")
{
    REQUIRE(JE_STRINGIFY_MACRO(test == notest)
            == std::string("test == notest"));

    REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}

TEST_CASE("Test Assert", "[Assert]")
{
    auto const LIB = Library{};

    REQUIRE(ASSERT(LIB.m_name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.m_name == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}
