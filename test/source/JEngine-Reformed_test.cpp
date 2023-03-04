#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <string>

#include <fmt/core.h>

#define JE_ASSERT_BREAK_ON_FAIL false
#include "Application.hpp"
#include "Assert.hpp"
#include "Base.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Platform.hpp"

struct Library
{
    std::string m_Name = fmt::format("{}", "JEngine-Reformed");
};

enum class TestEnum
{
    ZERO,
    ONE,
    TWO,
    COUNT
};

TEST_CASE(  // NOLINT(cert-err58-cpp,
            // cppcoreguidelines-avoid-non-const-global-variables)
    "Test Base macros",
    "[Base]")
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
    const Library LIB{};

    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}

TEST_CASE(
    "Test Platform initialization, window creation and OpenGL context creation",
    "[Platform][OpenGL]")
{
    REQUIRE(JE::EnginePlatform().Initialize());

    auto window = JE::CreateWindow("TestWindow");
    REQUIRE(window->Created());
    REQUIRE(window->GraphicsContext().Created());
}

TEST_CASE("Test Application creation and main loop", "[Application]")
{
    REQUIRE(JE::Application().Initialized());
    REQUIRE(JE::Application().LoopCount() == 0);

    JE::Application().Loop(1);

    REQUIRE(JE::Application().LoopCount() == 1);
    REQUIRE(JE::Application().EventsProcessed() != 0);
}