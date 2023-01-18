#include <catch2/catch_test_macros.hpp>

// #ifdef _MSC_VER
// #    pragma warning(push)
// #    pragma warning(disable : 4365)
// #endif

#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// #ifdef _MSC_VER
// #    pragma warning(pop)
// #endif

////////////////////////////////////////

#define JE_ASSERT_BREAK_ON_FAIL false
#include <Assert.hpp>

struct Library
{
    std::string name = fmt::format("{}", "JEngine-Reformed");
};

TEST_CASE("Test Base macros", "[Base]")
{
    auto const LIB = Library{};

    REQUIRE(JE_STRINGIFY_MACRO(test == notest)
            == std::string("test == notest"));

    REQUIRE(ASSERT(LIB.name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.name == "JEngine-Old") == false);
}

void InitializeSpdlog()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%T] [%l] %n: %v");

    std::vector<spdlog::sink_ptr> logSinks{2};

    if constexpr (JE_PLATFORM_UNIX) {
        logSinks[0] =
            std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    } else if constexpr (JE_PLATFORM_WINDOWS) {
        logSinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    }

    logSinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "JEngine3D.log", true);
}

TEST_CASE("Test Logger", "[Logger]")
{
    InitializeSpdlog();
}
