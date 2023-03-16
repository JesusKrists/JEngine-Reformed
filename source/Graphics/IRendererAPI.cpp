#include <exception>

#include "IRendererAPI.hpp"

#include <fmt/core.h>

#include "Assert.hpp"
#include "Graphics/OpenGLRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{
static Scope<IRendererAPI> sRendererApi;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

namespace detail
{
void SetCustomRendererAPI(Scope<IRendererAPI> rendererAPI)
{
    ASSERT(!sRendererApi);

    EngineLogger()->debug("Injecting custom RendererAPI - {}", rendererAPI->Name());
    sRendererApi = std::move(rendererAPI);
}
}  // namespace detail

// cppcheck-suppress unusedFunction
auto RendererAPI() -> IRendererAPI&
{
    if (!sRendererApi) {
        sRendererApi = CreateScope<detail::OpenGLRendererAPI>();
    }
    return *sRendererApi;
}

}  // namespace JE