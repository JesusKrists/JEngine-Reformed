#include <exception>
#include <string>

#include "Platform.hpp"

#include <fmt/core.h>

#include "Assert.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SDL/SDLPlatform.hpp"

namespace JE
{

// cppcheck-suppress unusedFunction
auto EnginePlatform() -> IPlatform&
{
    static detail::SDLPlatform s_Platform;
    return s_Platform;
}

// cppcheck-suppress unusedFunction
auto CreateWindow(std::string_view title, const Size2D& size) -> Scope<IWindow>
{
    ASSERT(EnginePlatform().Initialized());
    EngineLogger()->debug("Creating SDL Window ({}) of size: {}", title, size);
    return CreateScope<detail::SDLWindow>(std::string{title}, size);
}

}  // namespace JE