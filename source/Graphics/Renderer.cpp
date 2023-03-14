#include "Renderer.hpp"

#include "Assert.hpp"
#include "Graphics/IRenderTarget.hpp"
// #include "Types.hpp"

namespace JE
{

// cppcheck-suppress unusedFunction
void Renderer::Begin(IRenderTarget* target, [[maybe_unused]] const ColorRGBA& color)
{
    ASSERT(target != nullptr);
    ASSERT(m_CurrentRenderTarget == nullptr);

    m_CurrentRenderTarget = target;

    SubmitRenderCommand(
        [target]() -> bool
        {
            target->Bind();
            return true;
        });
}

// cppcheck-suppress unusedFunction
void Renderer::End()
{
    ASSERT(m_CurrentRenderTarget != nullptr);

    SubmitRenderCommand(
        [target = m_CurrentRenderTarget]()
        {
            target->Unbind();
            return true;
        });

    m_CurrentRenderTarget = nullptr;
}

}  // namespace JE