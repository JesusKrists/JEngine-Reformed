#include <SDL_video.h>
#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <exception>
#include <string>

#include <fmt/core.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_main.h>

#define JE_ASSERT_BREAK_ON_FAIL false
#include "Assert.hpp"
#include "Base.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

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
    auto const LIB = Library{};

    REQUIRE(ASSERT(LIB.mName == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.mName == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}

namespace JE
{

struct Size2D
{
    int x = 0;
    int y = 0;
};

struct SDLPlatform
{
    SDLPlatform(const SDLPlatform& other) = delete;
    SDLPlatform(SDLPlatform&& other) = delete;
    auto operator=(const SDLPlatform& other) -> SDLPlatform& = delete;
    auto operator=(SDLPlatform&& other) -> SDLPlatform& = delete;

    SDLPlatform()
    {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            EngineLogger()->error("Failed to initialize SDL: {}",
                                  SDL_GetError());
            return;
        }

        sPlatformInitialized = true;
    }

    ~SDLPlatform() { SDL_Quit(); }

    static inline bool
        sPlatformInitialized =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        false;
};

struct SDLWindow
{
    static constexpr auto DEFAULT_WINDOW_SIZE = Size2D{640, 480};

    SDLWindow(const SDLWindow& other) = delete;
    SDLWindow(SDLWindow&& other) = delete;
    auto operator=(const SDLWindow& other) -> SDLWindow& = delete;
    auto operator=(SDLWindow&& other) -> SDLWindow& = delete;

    explicit SDLWindow(const std::string& title,
                       const Size2D& size = DEFAULT_WINDOW_SIZE)
    {
        ASSERT(SDLPlatform::sPlatformInitialized);
        mWindow = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   size.x,
                                   size.y,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (mWindow == nullptr) {
            EngineLogger()->error("Failed to create SDL window: {}",
                                  SDL_GetError());
        }
    }

    ~SDLWindow() { SDL_DestroyWindow(mWindow); }

    SDL_Window* mWindow = nullptr;
};

}  // namespace JE

TEST_CASE("Test SDL initialization", "[SDL]")
{
    const JE::SDLPlatform PLATFORM;
    REQUIRE(SDL_WasInit(0) != 0);

    const JE::SDLWindow WINDOW{"TestWindow"};
    REQUIRE(WINDOW.mWindow != nullptr);
}