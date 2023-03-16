#pragma once

#include <functional>

#include "Assert.hpp"
#include "IRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{
class IRenderTarget;
struct ColorRGBA;

class Renderer
{
    friend class App;

  public:
    using RenderCommand = std::function<bool()>;

    static constexpr IRendererAPI::AttachmentFlags DEFAULT_ATTACHMENT_FLAGS = IRendererAPI::AttachmentFlag::COLOR
        | IRendererAPI::AttachmentFlag::DEPTH | IRendererAPI::AttachmentFlag::STENCIL;
    Renderer() = default;

    void Begin(IRenderTarget* target, const ColorRGBA& color);
    void End();
    inline auto CommandQueue() const -> const Vector<RenderCommand>& { return m_CommandQueue; }

  private:
    inline void SubmitRenderCommand(const RenderCommand& command)
    {
        ASSERT(m_CurrentRenderTarget != nullptr);
        m_CommandQueue.push_back(command);
    }

    inline void ProcessCommandQueue()
    {
        for (const auto& command : m_CommandQueue) {
            auto success = command();
            if (!success) {
                EngineLogger()->error("Render command failed");
            }
        }
        m_CommandQueue.clear();
    }

    IRenderTarget* m_CurrentRenderTarget = nullptr;
    Vector<RenderCommand> m_CommandQueue;
};

}  // namespace JE