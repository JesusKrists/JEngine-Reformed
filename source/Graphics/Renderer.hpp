#pragma once

#include <functional>

#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{
class IRenderTarget;
struct ColorRGBA;

class Renderer
{
  public:
    using RenderCommand = std::function<bool()>;

    Renderer() = default;

    void Begin(IRenderTarget* target, const ColorRGBA& color);
    void End();

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

    inline auto CommandQueue() const -> const Vector<RenderCommand>& { return m_CommandQueue; }

  private:
    inline void SubmitRenderCommand(const RenderCommand& command) { m_CommandQueue.push_back(command); }

    IRenderTarget* m_CurrentRenderTarget = nullptr;
    Vector<RenderCommand> m_CommandQueue;
};

}  // namespace JE