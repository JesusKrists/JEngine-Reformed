#pragma once

#include <string_view>
#include <utility>

#include "Graphics/Renderer.hpp"
#include "Memory.hpp"
#include "Types.hpp"

namespace JE
{
    class IEventProcessor;

    class IGraphicsContext
    {
      public:
        IGraphicsContext(const IGraphicsContext& other) = delete;
        IGraphicsContext(IGraphicsContext&& other) = delete;
        auto operator=(const IGraphicsContext& other) -> IGraphicsContext& = delete;
        auto operator=(IGraphicsContext&& other) -> IGraphicsContext& = delete;

        IGraphicsContext() = default;
        virtual ~IGraphicsContext() = default;

        virtual auto Created() const -> bool = 0;
        virtual auto SwapBuffers() -> bool = 0;
    };

    class IWindow : public IRenderTarget
    {
      public:
        static constexpr auto DEFAULT_WINDOW_SIZE = Size2D{1280, 720};

        IWindow(const IWindow& other) = delete;
        IWindow(IWindow&& other) = delete;
        auto operator=(const IWindow& other) -> IWindow& = delete;
        auto operator=(IWindow&& other) -> IWindow& = delete;

        IWindow() = default;
        ~IWindow() override = default;

        virtual auto Created() const -> bool = 0;
        virtual auto GraphicsContext() -> IGraphicsContext& = 0;
    };

    class IPlatform
    {
        friend class App;
        friend auto CreateWindow(std::string_view title, const Size2D& size) -> IWindow*;

      public:
        IPlatform(const IPlatform& other) = delete;
        IPlatform(IPlatform&& other) = delete;
        auto operator=(const IPlatform& other) -> IPlatform& = delete;
        auto operator=(IPlatform&& other) -> IPlatform& = delete;

        IPlatform() = default;
        virtual ~IPlatform() = default;

        virtual auto Name() const -> std::string_view = 0;
        virtual auto Initialized() const -> bool = 0;
        virtual auto GetLastError() const -> std::string_view = 0;

      private:
        virtual auto Initialize() -> bool = 0;
        virtual auto PollEvents(IEventProcessor& eventProcessor) -> bool = 0;
        virtual auto CreateWindow(std::string_view title, const Size2D& size) -> IWindow* = 0;
    };

    namespace detail  // NOLINT(readability-identifier-naming)
    {

        void SetCustomEnginePlatform(Scope<IPlatform> enginePlatform);

        template<typename T, typename... Args>
        inline void InjectCustomEnginePlatform(Args&&... args)
        {
            static_assert(std::is_base_of_v<IPlatform, T>, "Custom Engine Platform has to derive from IPlatform");
            SetCustomEnginePlatform(CreateScope<T>(std::forward<Args>(args)...));
        }

    }  // namespace detail

    auto EnginePlatform() -> IPlatform&;
    auto CreateWindow(std::string_view title, const Size2D& size = IWindow::DEFAULT_WINDOW_SIZE) -> IWindow*;

}  // namespace JE