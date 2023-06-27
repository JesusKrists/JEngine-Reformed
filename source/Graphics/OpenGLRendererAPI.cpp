#include <cstdint>

#include "OpenGLRendererAPI.hpp"

#include <glad/gl.h>
#include <spdlog/fmt/fmt.h>

#include "Graphics/IRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Types.hpp"

namespace JE::detail
{

constexpr auto ToString(GLenum value) -> std::string_view
{
    switch (value) {
        case GL_INVALID_ENUM:
            return "INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "INVALID_FRAMEBUFFER_OPERATION";
        default:
            return "UKNOWN_OPENGL_ERROR";
    }
}

struct OpenGLErrorWrapper
{
    template<typename Func>
    static inline auto Call(Func func) -> bool
    {
        func();

        bool errored = false;
        GLenum errorCode = 0;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            auto errorMessage = ToString(errorCode);
            JE::EngineLogger()->error("OpenGL API Error: {}", errorMessage);
            errored = true;
        }

        return !errored;
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

namespace
{

constexpr auto PrimitiveToOpenGLPrimitive(IRendererAPI::Primitive primitive) -> GLenum
{
    if (primitive == IRendererAPI::Primitive::TRIANGLES) {
        return GL_TRIANGLES;
    }

    return 0;
}

constexpr auto TypeToOpenGLType(IRendererAPI::Type type) -> GLenum
{
    if (type == IRendererAPI::Type::UNSIGNED_INT) {
        return GL_UNSIGNED_INT;
    }
    if (type == IRendererAPI::Type::UNSIGNED_SHORT) {
        return GL_UNSIGNED_SHORT;
    }

    return 0;
}

}  // namespace

auto OpenGLRendererAPI::DrawIndexed(Primitive primitiveType, std::uint32_t indexCount, Type indexType) -> bool
{
    return OpenGLErrorWrapper::Call(
        [primitiveType, indexCount, indexType]()
        {
            glDrawElements(PrimitiveToOpenGLPrimitive(primitiveType),
                           static_cast<GLsizei>(indexCount),
                           TypeToOpenGLType(indexType),
                           nullptr);
        });
}

}  // namespace JE::detail