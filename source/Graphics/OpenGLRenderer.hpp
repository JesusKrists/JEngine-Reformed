
#include <glad/gl.h>

#include "Assert.hpp"
#include "IRendererAPI.hpp"
#include "Renderer.hpp"

namespace JE
{

constexpr auto TypeToGLType(IRendererAPI::Type type) -> GLenum
{
    switch (type) {
        case IRendererAPI::Type::FLOAT:
            return GL_FLOAT;
            break;
        default:
            return 0;
            break;
    }
}

class OpenGLVertexBuffer : public IVertexBuffer
{
    friend class OpenGLVertexArray;

  public:
    OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
    OpenGLVertexBuffer(OpenGLVertexBuffer&& other) = delete;
    auto operator=(const OpenGLVertexBuffer& other) -> OpenGLVertexBuffer& = delete;
    auto operator=(OpenGLVertexBuffer&& other) -> OpenGLVertexBuffer& = delete;

    explicit OpenGLVertexBuffer(AttributeLayout layout)
        : IVertexBuffer(std::move(layout))
    {
        glGenBuffers(1, &m_BufferID);
        ASSERT(m_BufferID != 0);
    }
    ~OpenGLVertexBuffer() override
    {
        if (m_BufferID == 0) {
            return;
        }
        glDeleteBuffers(1, &m_BufferID);
    }

    inline auto Bind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == 0);

        if (m_BufferID == 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);

        sCurrentBoundBufferID = m_BufferID;

        return true;
    }

    inline auto Unbind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        sCurrentBoundBufferID = 0;

        return true;
    }

    inline auto SetData(const std::span<const std::byte> DATA) -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(DATA.size()), DATA.data(), GL_STATIC_DRAW);

        return true;
    }

  private:
    inline auto UploadLayout() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        for (std::size_t i = 0; i < m_Layout.Count(); ++i) {
            glVertexAttribPointer(
                static_cast<GLuint>(i),
                static_cast<GLint>(m_Layout[i].ComponentCount),
                TypeToGLType(m_Layout[i].Type),
                static_cast<GLboolean>(m_Layout[i].Normalized),
                static_cast<GLsizei>(m_Layout.Stride()),
                reinterpret_cast<void*>(m_Layout[i].Offset));  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,
                                                               // performance-no-int-to-ptr)
            glEnableVertexAttribArray(static_cast<GLuint>(i));
        }

        return true;
    }

    static inline IRendererAPI::BufferID
        sCurrentBoundBufferID =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        0;
};

class OpenGLElementBuffer : public IElementBuffer
{
    friend class OpenGLVertexArray;

  public:
    OpenGLElementBuffer(const OpenGLElementBuffer& other) = delete;
    OpenGLElementBuffer(OpenGLElementBuffer&& other) = delete;
    auto operator=(const OpenGLElementBuffer& other) -> OpenGLElementBuffer& = delete;
    auto operator=(OpenGLElementBuffer&& other) -> OpenGLElementBuffer& = delete;

    explicit OpenGLElementBuffer()
    {
        glGenBuffers(1, &m_BufferID);
        ASSERT(m_BufferID != 0);
    }
    ~OpenGLElementBuffer() override
    {
        if (m_BufferID == 0) {
            return;
        }
        glDeleteBuffers(1, &m_BufferID);
    }

    inline auto Bind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == 0);

        if (m_BufferID == 0) {
            return false;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);

        sCurrentBoundBufferID = m_BufferID;

        return true;
    }

    inline auto Unbind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        sCurrentBoundBufferID = 0;

        return true;
    }

    inline auto SetData(const std::span<const std::byte> DATA) -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(DATA.size()), DATA.data(), GL_STATIC_DRAW);

        return true;
    }

  private:
    static inline IRendererAPI::BufferID
        sCurrentBoundBufferID =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        0;
};

class OpenGLVertexArray : public IVertexArray
{
  public:
    OpenGLVertexArray(const OpenGLVertexArray& other) = delete;
    OpenGLVertexArray(OpenGLVertexArray&& other) = delete;
    auto operator=(const OpenGLVertexArray& other) -> OpenGLVertexArray& = delete;
    auto operator=(OpenGLVertexArray&& other) -> OpenGLVertexArray& = delete;

    OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_BufferID);
        ASSERT(m_BufferID != 0);
    }

    ~OpenGLVertexArray() override
    {
        if (m_BufferID == 0) {
            return;
        }
        glDeleteVertexArrays(1, &m_BufferID);
    }

    inline auto Build() -> bool override
    {
        ASSERT(m_VertexBuffers.size()
               == 1);  // TODO(JesusKrists): Right now we only support one buffer for all vertex attributes

        Bind();
        m_VertexBuffers[0]->Bind();

        auto success = m_VertexBuffers[0]->UploadLayout();

        m_VertexBuffers[0]->Unbind();
        Unbind();

        return success;
    }

    inline auto Bind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == 0);

        if (m_BufferID == 0) {
            return false;
        }

        glBindVertexArray(m_BufferID);
        m_IndexBuffer->Bind();

        sCurrentBoundBufferID = m_BufferID;

        return true;
    }

    inline auto Unbind() -> bool override
    {
        ASSERT(sCurrentBoundBufferID == m_BufferID);

        if (m_BufferID == 0) {
            return false;
        }

        m_IndexBuffer->Unbind();
        glBindVertexArray(0);

        sCurrentBoundBufferID = 0;

        return true;
    }

  private:
    static inline IRendererAPI::BufferID
        sCurrentBoundBufferID =  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        0;
};

}  // namespace JE