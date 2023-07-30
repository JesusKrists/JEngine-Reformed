#include "IRendererAPI.hpp"

#include <spdlog/fmt/bundled/core.h>

#include "Assert.hpp"
#include "Graphics/OpenGLRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{

    namespace
    {
        Scope<IRendererAPI> g_RendererApi;

    }  // namespace

    namespace detail
    {
        void SetCustomRendererAPI(Scope<IRendererAPI> renderer_api)
        {
            ASSERT(!g_RendererApi);

            EngineLogger()->debug("Injecting custom RendererAPI - {}", renderer_api->Name());
            g_RendererApi = std::move(renderer_api);
        }
    }  // namespace detail

    // cppcheck-suppress unusedFunction
    auto RendererAPI() -> IRendererAPI&
    {
        if (!g_RendererApi) {
            g_RendererApi = CreateScope<detail::OpenGLRendererAPI>();
        }
        return *g_RendererApi;
    }

}  // namespace JE