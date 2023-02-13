#pragma once

#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_video.h>

#include "Assert.hpp"
#include "Logger.hpp"
#include "Types.hpp"

namespace JE
{

namespace detail
{
struct SDLPlatform
{
    SDLPlatform(const SDLPlatform& other) = delete;
    SDLPlatform(SDLPlatform&& other) = delete;
    auto operator=(const SDLPlatform& other) -> SDLPlatform& = delete;
    auto operator=(SDLPlatform&& other) -> SDLPlatform& = delete;

    SDLPlatform() = default;

    inline void
    // cppcheck-suppress functionStatic
    Initialize()  // NOLINT(readability-convert-member-functions-to-static)
    {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            EngineLogger()->error("Failed to initialize SDL: {}",
                                  SDL_GetError());
            return;
        }

        sPlatformInitialized = true;
    }

    template<typename T>
    inline auto PollEvents(T func) -> bool
    {
        SDL_Event event;
        const bool EVENTS_PENDING = SDL_PollEvent(&event) == 1;
        if (EVENTS_PENDING) {
            func();
        }

        return EVENTS_PENDING;
    }

    ~SDLPlatform()
    {
        // cppcheck-suppress knownConditionTrueFalse
        if (sPlatformInitialized) {
            SDL_Quit();
        }

        sPlatformInitialized = false;
    }

    static inline bool
        sPlatformInitialized =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        false;
};

struct SDLOpenGLGraphicsContext
{
    static constexpr auto OPENGL_MAJOR_VERSION = 4;
    static constexpr auto OPENGL_MINOR_VERSION = 5;

    static constexpr auto BITS_PER_COLOR = 8;
    static constexpr auto DEPTH_BUFFER_BITS = 24;

    SDLOpenGLGraphicsContext(const SDLOpenGLGraphicsContext& other) = delete;
    SDLOpenGLGraphicsContext(SDLOpenGLGraphicsContext&& other) = delete;
    auto operator=(const SDLOpenGLGraphicsContext& other)
        -> SDLOpenGLGraphicsContext& = delete;
    auto operator=(SDLOpenGLGraphicsContext&& other)
        -> SDLOpenGLGraphicsContext& = delete;

    SDLOpenGLGraphicsContext() = default;

    static inline void InitializeOpenGLParameters()
    {
        std::uint32_t flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
        if constexpr (!RELEASE_BUILD) {
            flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                            static_cast<std::int32_t>(flags));
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_COLOR);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_COLOR);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_COLOR);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_COLOR);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_BITS);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, BITS_PER_COLOR);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

    inline void Initialize(SDL_Window* window)
    {
        ASSERT(SDLPlatform::sPlatformInitialized);

        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

        auto* previousContext = SDL_GL_GetCurrentContext();
        auto* previousWindow = SDL_GL_GetCurrentWindow();

        mContext = SDL_GL_CreateContext(window);
        if (mContext == nullptr) {
            EngineLogger()->error("Failed to create SDL OpenGL context: {}",
                                  SDL_GetError());
            return;
        }

        if (SDL_GL_MakeCurrent(window, mContext) != 0) {
            EngineLogger()->error(
                "Failed to make SDL OpenGL context current: {}",
                SDL_GetError());
        }

        // cppcheck-suppress knownConditionTrueFalse
        if (!sGraphicsContextInitialized) {
            if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
                EngineLogger()->error("Failed to initialize GLAD");
                return;
            }

            ASSERT(GLVersion.major == OPENGL_MAJOR_VERSION);
            ASSERT(GLVersion.minor == OPENGL_MINOR_VERSION);
        }

        if (previousContext != nullptr && previousWindow != nullptr) {
            if (SDL_GL_MakeCurrent(previousWindow, previousContext) != 0) {
                EngineLogger()->error(
                    "Failed to make previous SDL OpenGL context current: {}",
                    SDL_GetError());
            }
        }

        sGraphicsContextInitialized = true;
    }

    ~SDLOpenGLGraphicsContext()
    {
        ASSERT(SDLPlatform::sPlatformInitialized);

        if (mContext != nullptr) {
            SDL_GL_DeleteContext(mContext);
        }

        sGraphicsContextInitialized = false;
    }

    SDL_GLContext mContext = nullptr;

    static inline bool
        sGraphicsContextInitialized =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        false;
};

}  // namespace detail

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
        ASSERT(detail::SDLPlatform::sPlatformInitialized);

        // cppcheck-suppress knownConditionTrueFalse
        if (!detail::SDLOpenGLGraphicsContext::sGraphicsContextInitialized) {
            detail::SDLOpenGLGraphicsContext::InitializeOpenGLParameters();
        }

        mWindow = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
            SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
            size.x,
            size.y,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (mWindow == nullptr) {
            EngineLogger()->error("Failed to create SDL window: {}",
                                  SDL_GetError());
            return;
        }

        mGraphicsContext.Initialize(mWindow);
    }

    ~SDLWindow()
    {
        ASSERT(detail::SDLPlatform::sPlatformInitialized);
        if (mWindow != nullptr) {
            SDL_DestroyWindow(mWindow);
        }
    }

    SDL_Window* mWindow = nullptr;
    detail::SDLOpenGLGraphicsContext mGraphicsContext;
};

inline auto EnginePlatform() -> detail::SDLPlatform&
{
    static detail::SDLPlatform s_Platform;
    return s_Platform;
}

}  // namespace JE