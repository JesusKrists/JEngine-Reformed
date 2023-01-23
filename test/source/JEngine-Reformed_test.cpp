#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#define JE_ASSERT_BREAK_ON_FAIL false
#include <Assert.hpp>
#include <Logger.hpp>

struct Library
{
    std::string m_name = fmt::format("{}", "JEngine-Reformed");
};

TEST_CASE("Test Base macros", "[Base]")
{
    REQUIRE(JE_STRINGIFY_MACRO(test == notest)
            == std::string("test == notest"));
}

TEST_CASE("Test Assert", "[Assert]")
{
    auto const LIB = Library{};

    REQUIRE(ASSERT(LIB.m_name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.m_name == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger().get() != nullptr);
    REQUIRE(JE::AppLogger().get() != nullptr);
}
