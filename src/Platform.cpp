#include "Platform.hpp"

#include <spdlog/fmt/fmt.h>

#include "Assert.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SDL/SDLPlatform.hpp"

namespace JE
{

    namespace
    {
        Scope<IPlatform> g_EnginePlatform;
    }  // namespace

    namespace detail
    {

        void SetCustomEnginePlatform(Scope<IPlatform> engine_platform)
        {
            ASSERT(!g_EnginePlatform);

            EngineLogger()->debug("Injecting custom engine platform - {}", engine_platform->Name());
            g_EnginePlatform = std::move(engine_platform);
        }

    }  // namespace detail

    // cppcheck-suppress unusedFunction
    auto EnginePlatform() -> IPlatform&
    {
        if (!g_EnginePlatform) {
            g_EnginePlatform = CreateScope<detail::SDLPlatform>();
        }
        return *g_EnginePlatform;
    }

    // cppcheck-suppress unusedFunction
    auto CreateWindow(std::string_view title, const Size2D& size) -> IWindow*
    {
        ASSERT(EnginePlatform().Initialized());

        EngineLogger()->info("Creating Window ({}) of size: {}", title, size);
        return EnginePlatform().CreateWindow(title, size);
    }

}  // namespace JE