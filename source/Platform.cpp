#include <exception>

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

namespace detail
{

void SetCustomEnginePlatform(Scope<IPlatform> enginePlatform)
{
    ASSERT(!sEnginePlatform);

    EngineLogger()->debug("Injecting custom engine platform - {}",
                          enginePlatform->Name());
    sEnginePlatform = std::move(enginePlatform);
}

}  // namespace detail

// cppcheck-suppress unusedFunction
auto EnginePlatform() -> IPlatform&
{
    if (sEnginePlatform.get() == nullptr) {
        sEnginePlatform = CreateScope<detail::SDLPlatform>();
    }
    return *sEnginePlatform;
}

// cppcheck-suppress unusedFunction
auto CreateWindow(std::string_view title, const Size2D& size) -> IWindow*
{
    ASSERT(EnginePlatform().Initialized());

    EngineLogger()->info("Creating Window ({}) of size: {}", title, size);
    return EnginePlatform().CreateWindow(title, size);
}

}  // namespace JE