#pragma once

#include <string_view>

#include "Memory.hpp"
#include "Types.hpp"

namespace JE
{
namespace detail  // NOLINT(readability-identifier-naming)
{
class IEventProcessor;
}  // namespace detail

class IPlatform
{
  public:
    IPlatform(const IPlatform& other) = delete;
    IPlatform(IPlatform&& other) = delete;
    auto operator=(const IPlatform& other) -> IPlatform& = delete;
    auto operator=(IPlatform&& other) -> IPlatform& = delete;

    IPlatform() = default;
    virtual ~IPlatform() = default;

    virtual auto Initialize() -> bool = 0;
    virtual auto Initialized() const -> bool = 0;
    virtual auto GetLastError() const -> std::string_view = 0;

    virtual auto PollEvents(detail::IEventProcessor& eventProcessor)
        -> bool = 0;
};

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
};

class IWindow
{
  public:
    static constexpr auto DEFAULT_WINDOW_SIZE = Size2D{640, 480};

    IWindow(const IWindow& other) = delete;
    IWindow(IWindow&& other) = delete;
    auto operator=(const IWindow& other) -> IWindow& = delete;
    auto operator=(IWindow&& other) -> IWindow& = delete;

    IWindow() = default;
    virtual ~IWindow() = default;

    virtual auto Created() const -> bool = 0;
    virtual auto GraphicsContext() -> IGraphicsContext& = 0;
};

auto EnginePlatform() -> IPlatform&;
auto CreateWindow(std::string_view title,
                  const Size2D& size = IWindow::DEFAULT_WINDOW_SIZE)
    -> Scope<IWindow>;

}  // namespace JE