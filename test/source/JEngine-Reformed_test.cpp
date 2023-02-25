#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <fmt/core.h>

#define JE_ASSERT_BREAK_ON_FAIL false
#include "Assert.hpp"
#include "Base.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SDLPlatform.hpp"

struct Library
{
    std::string mName = fmt::format("{}", "JEngine-Reformed");
};

enum struct TestEnum
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

    REQUIRE(ASSERT(LIB.mName == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.mName == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}

TEST_CASE("Test SDL initialization", "[SDL]")
{
    JE::EnginePlatform().Initialize();
    REQUIRE(JE::detail::SDLPlatform::sPlatformInitialized != 0);
    REQUIRE(SDL_WasInit(0) != 0);
}

TEST_CASE("Test SDL window creation", "[SDL]")
{
    JE::EnginePlatform().Initialize();

    const JE::SDLWindow WINDOW{"TestWindow"};
    REQUIRE(WINDOW.mWindow != nullptr);
}

TEST_CASE("Test SDL OpenGL context creation", "[SDL][OpenGL]")
{
    JE::EnginePlatform().Initialize();

    const JE::SDLWindow WINDOW{"TestWindow"};

    REQUIRE(JE::detail::SDLOpenGLGraphicsContext::sGraphicsContextInitialized);
    REQUIRE(WINDOW.mGraphicsContext.mContext != nullptr);
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
        EnginePlatform().Initialize();
        mMainWindow = CreateScope<SDLWindow>(MAINWINDOW_DEFAULT_TITLE);
    }

    inline void ProcessEvents()
    {
        auto onEvent = []() {};

        while (EnginePlatform().PollEvents(onEvent)) {
            ++mEventsProcessed;
        }
    }

    inline void Loop(std::int64_t loopCount = -1)
    {
        while (mLoopCount != loopCount) {
            ProcessEvents();

            ++mLoopCount;
        }
    }

    Scope<SDLWindow> mMainWindow;
    std::int64_t mLoopCount = 0;
    std::uint64_t mEventsProcessed = 0;
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
    REQUIRE(JE::Application().mMainWindow->mWindow != nullptr);
    REQUIRE(JE::Application().mLoopCount == 0);

    JE::Application().Loop(1);

    REQUIRE(JE::Application().mLoopCount == 1);
    REQUIRE(JE::Application().mEventsProcessed != 0);
}