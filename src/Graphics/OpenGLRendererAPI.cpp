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
            GLenum error_code = 0;
            while ((error_code = glGetError()) != GL_NO_ERROR) {
                auto error_message = ToString(error_code);
                JE::EngineLogger()->error("OpenGL API Error: {}", error_message);
                errored = true;
            }

            return !errored;
        }
    };

    auto OpenGLRendererAPI::Name() const -> std::string_view { return "OpenGL"; }

    auto OpenGLRendererAPI::SetClearColor(const RGBA& color) -> bool
    {
        return OpenGLErrorWrapper::Call([&color]() { glClearColor(color.R(), color.G(), color.B(), color.A()); });
    }

    auto OpenGLRendererAPI::ClearFramebuffer(AttachmentFlags flags) -> bool
    {
        return OpenGLErrorWrapper::Call(
            [flags]()
            {
                AttachmentFlags gl_flags = 0;
                if ((flags & AttachmentFlag::COLOR) != 0u) {
                    gl_flags |= static_cast<std::uint32_t>(GL_COLOR_BUFFER_BIT);
                }

                if ((flags & AttachmentFlag::DEPTH) != 0u) {
                    gl_flags |= static_cast<std::uint32_t>(GL_DEPTH_BUFFER_BIT);
                }

                if ((flags & AttachmentFlag::STENCIL) != 0u) {
                    gl_flags |= static_cast<std::uint32_t>(GL_STENCIL_BUFFER_BIT);
                }

                glClear(gl_flags);
            });
    }

    auto OpenGLRendererAPI::BindFramebuffer(FramebufferID buffer_id) -> bool
    {
        return OpenGLErrorWrapper::Call([buffer_id]() { glBindFramebuffer(GL_FRAMEBUFFER, buffer_id); });
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

    auto OpenGLRendererAPI::DrawIndexed(Primitive primitive_type, std::uint32_t index_count, Type index_type) -> bool
    {
        return OpenGLErrorWrapper::Call(
            [primitive_type, index_count, index_type]()
            {
                glDrawElements(PrimitiveToOpenGLPrimitive(primitive_type),
                               static_cast<GLsizei>(index_count),
                               TypeToOpenGLType(index_type),
                               nullptr);
            });
    }

}  // namespace JE::detail