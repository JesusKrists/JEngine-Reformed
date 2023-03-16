#include <cstdint>

#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

#include "Types.hpp"

namespace JE::detail
{

struct OpenGLErrorWrapper
{
    template<typename Func>
    static inline auto Call(Func func) -> bool
    {
        func();

        return true;
    }
};

auto OpenGLRendererAPI::Name() const -> std::string_view { return "OpenGL"; }

auto OpenGLRendererAPI::SetClearColor(const ColorRGBA& color) -> bool
{
    return OpenGLErrorWrapper::Call([&color]() { glClearColor(color.R(), color.G(), color.B(), color.A()); });
}

auto OpenGLRendererAPI::ClearFramebuffer(AttachmentFlags flags) -> bool
{
    return OpenGLErrorWrapper::Call(
        [flags]()
        {
            AttachmentFlags glFlags = 0;
            if ((flags & AttachmentFlag::COLOR) != 0u) {
                glFlags |= static_cast<std::uint32_t>(GL_COLOR_BUFFER_BIT);
            }

            if ((flags & AttachmentFlag::DEPTH) != 0u) {
                glFlags |= static_cast<std::uint32_t>(GL_DEPTH_BUFFER_BIT);
            }

            if ((flags & AttachmentFlag::STENCIL) != 0u) {
                glFlags |= static_cast<std::uint32_t>(GL_STENCIL_BUFFER_BIT);
            }

            glClear(glFlags);
        });
}

auto OpenGLRendererAPI::BindFramebuffer(FramebufferID bufferID) -> bool
{
    return OpenGLErrorWrapper::Call([bufferID]() { glBindFramebuffer(GL_FRAMEBUFFER, bufferID); });
}

}  // namespace JE::detail