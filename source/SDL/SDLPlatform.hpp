#pragma once

#include <glad/glad.h>

#include "Events.hpp"
#include "Memory.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_video.h>

#include "Assert.hpp"
#include "Logger.hpp"
#include "Platform.hpp"
#include "Types.hpp"

namespace JE::detail
{

class SDLPlatform final : public IPlatform
{
  public:
    SDLPlatform(const SDLPlatform& other) = delete;
    SDLPlatform(SDLPlatform&& other) = delete;
    auto operator=(const SDLPlatform& other) -> SDLPlatform& = delete;
    auto operator=(SDLPlatform&& other) -> SDLPlatform& = delete;

    SDLPlatform() = default;

    inline auto Initialize() -> bool override
    {
        EngineLogger()->debug("Initializing SDL platform");

        SDL_SetMainReady();
        m_PlatformInitialized = SDL_Init(SDL_INIT_VIDEO) == 0;
        if (!m_PlatformInitialized) {
            EngineLogger()->error("Failed to initialize SDL: {}",
                                  SDL_GetError());
        }

        return m_PlatformInitialized;
    }

    inline auto Initialized() const -> bool override
    {
        return m_PlatformInitialized;
    }

    inline auto GetLastError() const -> std::string_view override
    {
        return SDL_GetError();
    }

    inline auto PollEvents(IEventProcessor& eventProcessor) -> bool override
    {
        ASSERT(m_PlatformInitialized);

        SDL_Event event;
        const bool EVENTS_PENDING = SDL_PollEvent(&event) == 1;
        if (EVENTS_PENDING) {
            if (event.type == SDL_QUIT) {
                QuitEvent evnt{};
                eventProcessor.ProcessEvent(evnt);
            } else {
                UnknownEvent evnt{};
                eventProcessor.ProcessEvent(evnt);
            }
        }

        return EVENTS_PENDING;
    }

    ~SDLPlatform() override
    {
        if (m_PlatformInitialized) {
            SDL_Quit();
        }
    }

  private:
    bool m_PlatformInitialized = false;
};

class SDLOpenGLGraphicsContext : public IGraphicsContext
{
  public:
    static constexpr auto OPENGL_MAJOR_VERSION = 4;
#if !JE_PLATFORM_APPLE_VALUE
    static constexpr auto OPENGL_MINOR_VERSION = 5;
#else
    static constexpr auto OPENGL_MINOR_VERSION = 1;
#endif

    static constexpr auto BITS_PER_COLOR = 8;
    static constexpr auto DEPTH_BUFFER_BITS = 24;

    SDLOpenGLGraphicsContext(const SDLOpenGLGraphicsContext& other) = delete;
    SDLOpenGLGraphicsContext(SDLOpenGLGraphicsContext&& other) = delete;
    auto operator=(const SDLOpenGLGraphicsContext& other)
        -> SDLOpenGLGraphicsContext& = delete;
    auto operator=(SDLOpenGLGraphicsContext&& other)
        -> SDLOpenGLGraphicsContext& = delete;

    SDLOpenGLGraphicsContext() { InitializeOpenGLParameters(); }

    inline void Initialize(SDL_Window* window)
    {
        auto* previousContext = SDL_GL_GetCurrentContext();
        auto* previousWindow = SDL_GL_GetCurrentWindow();

        m_Context = SDL_GL_CreateContext(window);
        if (m_Context == nullptr) {
            EngineLogger()->error("Failed to create SDL OpenGL context: {}",
                                  SDL_GetError());
            return;
        }

        if (SDL_GL_MakeCurrent(window, m_Context) != 0) {
            EngineLogger()->error(
                "Failed to make SDL OpenGL context current: {}",
                SDL_GetError());
            return;
        }

        // cppcheck-suppress knownConditionTrueFalse
        if (!sGladInitialized) {
            if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
                EngineLogger()->error("Failed to initialize GLAD");
                return;
            }

            EngineLogger()->debug("Requested OpenGL version: {}.{}",
                                  OPENGL_MAJOR_VERSION,
                                  OPENGL_MINOR_VERSION);

            EngineLogger()->debug("Created OpenGL version: {}.{}",
                                  GLVersion.major,
                                  GLVersion.minor);

            ASSERT(GLVersion.major == OPENGL_MAJOR_VERSION);
            ASSERT(GLVersion.minor == OPENGL_MINOR_VERSION);
            sGladInitialized = true;
        }

        if (previousContext != nullptr && previousWindow != nullptr) {
            if (SDL_GL_MakeCurrent(previousWindow, previousContext) != 0) {
                EngineLogger()->error(
                    "Failed to make previous SDL OpenGL context current: {}",
                    SDL_GetError());
                return;
            }
        }
    }

    inline auto Created() const -> bool override
    {
        return sGladInitialized && m_Context != nullptr;
    }

    ~SDLOpenGLGraphicsContext() override
    {
        if (m_Context != nullptr) {
            SDL_GL_DeleteContext(m_Context);
        }
    }

  private:
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
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    }

    SDL_GLContext m_Context = nullptr;
    static inline bool
        sGladInitialized =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        false;
};

class SDLWindow final : public IWindow
{
  public:
    SDLWindow(const SDLWindow& other) = delete;
    SDLWindow(SDLWindow&& other) = delete;
    auto operator=(const SDLWindow& other) -> SDLWindow& = delete;
    auto operator=(SDLWindow&& other) -> SDLWindow& = delete;

    SDLWindow(const std::string& title, const Size2D& size)
        : m_Window(SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
            SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
            size.x,
            size.y,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
    {
        if (m_Window == nullptr) {
            EngineLogger()->error("Failed to create SDL window: {}",
                                  SDL_GetError());
            return;
        }

        m_GraphicsContext->Initialize(m_Window);
    }

    inline auto Created() const -> bool override { return m_Window != nullptr; }

    inline auto GraphicsContext() -> IGraphicsContext& override
    {
        return *m_GraphicsContext;
    }

    ~SDLWindow() override
    {
        m_GraphicsContext.reset();
        if (m_Window != nullptr) {
            SDL_DestroyWindow(m_Window);
        }
    }

  private:
    Scope<SDLOpenGLGraphicsContext> m_GraphicsContext =
        CreateScope<SDLOpenGLGraphicsContext>();
    SDL_Window* m_Window = nullptr;
};

}  // namespace JE::detail