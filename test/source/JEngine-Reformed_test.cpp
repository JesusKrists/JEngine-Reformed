#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <cstdint>
#include <string>

#include <fmt/core.h>

#define JE_ASSERT_BREAK_ON_FAIL false
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

namespace JE
{

namespace detail
{

struct App
{
    static constexpr auto MAINWINDOW_DEFAULT_TITLE =
        "JEngine-Reformed Application";

    App()
    {
        if (!EnginePlatform().Initialize()) {
            return;
        }

        m_MainWindow = CreateWindow(MAINWINDOW_DEFAULT_TITLE);
        if (!m_MainWindow->Created()) {
            return;
        }

        m_Initialized = true;
    }

    inline void ProcessEvents()
    {
        auto onEvent = []() {};

        while (EnginePlatform().PollEvents(onEvent)) {
            ++m_EventsProcessed;
        }
    }

    inline void Loop(std::int64_t loopCount = -1)
    {
        while (m_LoopCount != loopCount) {
            ProcessEvents();

            ++m_LoopCount;
        }
    }

    bool m_Initialized = false;

    Scope<IWindow> m_MainWindow;
    std::int64_t m_LoopCount = 0;
    std::uint64_t m_EventsProcessed = 0;
};

}  // namespace detail

inline auto Application() -> detail::App&
{
    static detail::App s_Application;
    return s_Application;
}

}  // namespace JE

TEST_CASE("Test Application creation and main loop", "[Application]")
{
    REQUIRE(JE::Application().m_Initialized);
    REQUIRE(JE::Application().m_LoopCount == 0);

    JE::Application().Loop(1);

    REQUIRE(JE::Application().m_LoopCount == 1);
    REQUIRE(JE::Application().m_EventsProcessed != 0);
}