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

static Scope<IPlatform>
    sEnginePlatform;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

void SetCustomEnginePlatform(Scope<IPlatform> enginePlatform)
{
    ASSERT(!sEnginePlatform);

    EngineLogger()->debug("Injecting custom engine platform - {}",
                          enginePlatform->Name());
    sEnginePlatform = std::move(enginePlatform);
}

// cppcheck-suppress unusedFunction
auto EnginePlatform() -> IPlatform&
{
    if (sEnginePlatform.get() == nullptr) {
        sEnginePlatform = CreateScope<detail::SDLPlatform>();
    }
    return *sEnginePlatform;
}

// cppcheck-suppress unusedFunction
auto CreateWindow(std::string_view title, const Size2D& size) -> Scope<IWindow>
{
    ASSERT(EnginePlatform().Initialized());
    EngineLogger()->debug("Creating SDL Window ({}) of size: {}", title, size);
    return CreateScope<detail::SDLWindow>(std::string{title}, size);
}

}  // namespace JE