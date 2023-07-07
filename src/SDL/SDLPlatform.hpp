#pragma once
#include <glad/gl.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_video.h>

#include "Assert.hpp"
#include "Events.hpp"
#include "Graphics/OpenGLRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Platform.hpp"
#include "Types.hpp"

namespace JE::detail
{

    class SDLOpenGLGraphicsContext final : public IGraphicsContext
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
        auto operator=(const SDLOpenGLGraphicsContext& other) -> SDLOpenGLGraphicsContext& = delete;
        auto operator=(SDLOpenGLGraphicsContext&& other) -> SDLOpenGLGraphicsContext& = delete;

        SDLOpenGLGraphicsContext() { InitializeOpenGLParameters(); }

        inline void Initialize(SDL_Window* window)
        {
            m_Window = window;

            auto* previousWindow = SDL_GL_GetCurrentWindow();
            auto* previousContext = SDL_GL_GetCurrentContext();

            m_Context = SDL_GL_CreateContext(window);
            if (m_Context == nullptr) {
                EngineLogger()->error("Failed to create SDL OpenGL context: {}", SDL_GetError());
                return;
            }

            if (SDL_GL_MakeCurrent(window, m_Context) != 0) {
                EngineLogger()->error("Failed to make SDL OpenGL context current: {}", SDL_GetError());
                return;
            }

            // cppcheck-suppress knownConditionTrueFalse
            if (!sGladInitialized) {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                auto version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
                if (version == 0) {
                    EngineLogger()->error("Failed to initialize GLAD");
                    return;
                }

                EngineLogger()->info("Requested OpenGL version: {}.{} | Created OpenGL version: {}.{}",
                                     OPENGL_MAJOR_VERSION,
                                     OPENGL_MINOR_VERSION,
                                     GLAD_VERSION_MAJOR(version),
                                     GLAD_VERSION_MINOR(version));

                ASSERT(GLAD_VERSION_MAJOR(version) == OPENGL_MAJOR_VERSION);
                ASSERT(GLAD_VERSION_MINOR(version) == OPENGL_MINOR_VERSION);
                sGladInitialized = true;
            }

            if (previousWindow == nullptr || previousContext == nullptr) {
                EngineLogger()->trace("No previous graphics context available");
                return;
            }

            if (SDL_GL_MakeCurrent(previousWindow, previousContext) != 0) {
                EngineLogger()->error("Failed to make previous SDL OpenGL context current: {}", SDL_GetError());
            }
        }

        inline auto Created() const -> bool override { return sGladInitialized && m_Context != nullptr; }

        inline auto SwapBuffers() -> bool override
        {
            MakeContextCurrent();

            const bool OPENGL_SUCCESS = RendererAPI().BindFramebuffer(0);
            SDL_GL_SwapWindow(m_Window);

            RestorePreviousContext();

            return OPENGL_SUCCESS;
        }

        inline void MakeContextCurrent()
        {
            m_PreviousWindow = SDL_GL_GetCurrentWindow();
            m_PreviousContext = SDL_GL_GetCurrentContext();

            SDL_GL_MakeCurrent(m_Window, m_Context);
        }

        inline void RestorePreviousContext()
        {
            if ((m_PreviousWindow == nullptr || m_PreviousContext == nullptr)
                && (m_PreviousWindow == m_Window || m_PreviousContext == m_Context))
            {
                return;
            }

            SDL_GL_MakeCurrent(m_PreviousWindow, m_PreviousContext);
            m_PreviousWindow = nullptr;
            m_PreviousContext = nullptr;
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

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, static_cast<std::int32_t>(flags));
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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

        SDL_Window* m_Window = nullptr;
        SDL_GLContext m_Context = nullptr;

        SDL_Window* m_PreviousWindow = nullptr;
        SDL_GLContext m_PreviousContext = nullptr;

        static inline bool sGladInitialized = false;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    };

    class SDLWindow final : public IWindow
    {
      public:
        SDLWindow(const SDLWindow& other) = delete;
        SDLWindow(SDLWindow&& other) = delete;
        auto operator=(const SDLWindow& other) -> SDLWindow& = delete;
        auto operator=(SDLWindow&& other) -> SDLWindow& = delete;

        SDLWindow(const std::string& title, const Size2D& size)
            : m_Window(SDL_CreateWindow(title.c_str(),
                                        SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
                                        SDL_WINDOWPOS_CENTERED,  // NOLINT(hicpp-signed-bitwise)
                                        size.x,
                                        size.y,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
        {
            if (m_Window == nullptr) {
                EngineLogger()->error("Failed to create SDL window: {}", SDL_GetError());
                return;
            }

            m_GraphicsContext->Initialize(m_Window);
        }

        inline auto Created() const -> bool override { return m_Window != nullptr; }

        inline auto GraphicsContext() -> IGraphicsContext& override { return *m_GraphicsContext; }

        inline void Bind() override { m_GraphicsContext->MakeContextCurrent(); }

        inline void Unbind() override { m_GraphicsContext->RestorePreviousContext(); }

        ~SDLWindow() override
        {
            m_GraphicsContext.reset();
            if (m_Window != nullptr) {
                SDL_DestroyWindow(m_Window);
            }
        }

      private:
        Scope<SDLOpenGLGraphicsContext> m_GraphicsContext = CreateScope<SDLOpenGLGraphicsContext>();
        SDL_Window* m_Window = nullptr;
    };

    class SDLPlatform final : public IPlatform
    {
      public:
        SDLPlatform(const SDLPlatform& other) = delete;
        SDLPlatform(SDLPlatform&& other) = delete;
        auto operator=(const SDLPlatform& other) -> SDLPlatform& = delete;
        auto operator=(SDLPlatform&& other) -> SDLPlatform& = delete;

        SDLPlatform() = default;

        inline auto Name() const -> std::string_view override { return "SDL2 Platform"; }

        inline auto Initialized() const -> bool override { return m_PlatformInitialized; }

        inline auto GetLastError() const -> std::string_view override { return SDL_GetError(); }

        ~SDLPlatform() override
        {
            if (m_PlatformInitialized) {
                m_Windows.clear();
                SDL_Quit();
            }
        }

      private:
        inline auto Initialize() -> bool override
        {
            ASSERT(!m_PlatformInitialized);

            EngineLogger()->debug("Initializing {}", Name());

            SDL_SetMainReady();
            m_PlatformInitialized = SDL_Init(SDL_INIT_VIDEO) == 0;
            if (!m_PlatformInitialized) {
                EngineLogger()->error("Failed to initialize SDL: {}", SDL_GetError());
            }

            return m_PlatformInitialized;
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

        inline auto CreateWindow(std::string_view title, const Size2D& size) -> IWindow* override
        {
            return m_Windows.emplace_back(CreateScope<SDLWindow>(std::string{title}, size)).get();
        }

        bool m_PlatformInitialized = false;
        Vector<Scope<SDLWindow>> m_Windows;
    };

}  // namespace JE::detail