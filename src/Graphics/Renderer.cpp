#include <cstdint>

#include "Renderer.hpp"

#include "Assert.hpp"
#include "IRendererAPI.hpp"
#include "OpenGLRenderer.hpp"

namespace JE
{

    auto CreateVertexBuffer(const AttributeLayout& layout) -> Scope<IVertexBuffer>
    {
        return CreateScope<OpenGLVertexBuffer>(layout);
    }

    auto CreateElementBuffer() -> Scope<IElementBuffer> { return CreateScope<OpenGLElementBuffer>(); }

    auto CreateVertexArray() -> Scope<IVertexArray> { return CreateScope<OpenGLVertexArray>(); }

    // cppcheck-suppress unusedFunction
    auto CreateShader(std::string_view debugName, std::string_view vertexSource, std::string_view fragmentSource)
        -> Scope<IShaderProgram>
    {
        return CreateScope<OpenGLShaderProgram>(debugName, vertexSource, fragmentSource);
    }

    // class RendererMesh
    // {
    //   public:
    //     struct Vertex
    //     {
    //         VertexType vertexPosition;  // NOLINT(readability-identifier-naming)
    //     };

    //     explicit RendererMesh(const Mesh* mesh)
    //         : m_OriginalMesh(*mesh)
    //     {
    //         ASSERT(mesh != nullptr);

    //         m_Vertices.reserve(mesh->Vertices().size());

    //         m_VAO->AddBuffer(AttributeLayout{}, const std::span<std::byte>& data)
    //     }

    //     inline auto Used() const -> bool { return m_Used; }
    //     inline auto OriginalMesh() const -> const Mesh& { return m_OriginalMesh; }
    //     inline auto Vertices() const -> const Vector<Vertex>& { return m_Vertices; }
    //     inline auto Indices() const -> const Vector<IndexType>& { return m_OriginalMesh.Indices(); }

    //   private:
    //     bool m_Used = false;
    //     Mesh m_OriginalMesh;
    //     Vector<Vertex> m_Vertices;
    //     Scope<IVertexArray> m_VAO;
    // };

    // class VertexArrayRegister
    // {
    //   public:
    //     static auto CreateFromMesh(const Mesh& mesh) -> IVertexArray&
    //     {
    //         for (const auto& vao : sVertexArrays) {
    //             if (vao->OriginalMesh() == mesh) {
    //                 vao->Use();
    //                 return *vao;
    //             }
    //         }

    //         return *sVertexArrays.emplace_back(CreateVertexArray(&mesh));
    //     }

    //     static void Reset()
    //     {
    //         for (auto iter = std::begin(sVertexArrays); iter != std::end(sVertexArrays);) {
    //             if (iter->Used()) {
    //                 ++iter;
    //                 continue;
    //             }

    //             iter = sVertexArrays.erase(iter);
    //         }
    //     }

    //   private:
    //     // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    //     static inline std::vector<Scope<RendererMesh>> sVertexArrays;
    // };

    // static const std::string_view VERTEX_SHADER_SOURCE =
    //     "#version 330 core\n"
    //     "layout (location = 0) in vec3 aPos;\n"
    //     "void main()\n"
    //     "{\n"
    //     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //     "}\0";
    // static const std::string_view FRAGMENT_SHADER_SOURCE =
    //     "#version 330 core\n"
    //     "out vec4 FragColor;\n"
    //     "void main()\n"
    //     "{\n"
    //     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    //     "}\n\0";

    // cppcheck-suppress unusedFunction
    void Renderer::Begin(IRenderTarget* target, [[maybe_unused]] const RGBA& color)
    {
        ASSERT(target != nullptr);
        ASSERT(m_CurrentRenderTarget == nullptr);

        m_CurrentRenderTarget = target;

        SubmitRenderCommand(
            [target, &color]() -> bool
            {
                target->Bind();
                RendererAPI().SetClearColor(color);
                RendererAPI().ClearFramebuffer(Renderer::DEFAULT_ATTACHMENT_FLAGS);
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

    // cppcheck-suppress unusedFunction
    void Renderer::DrawMesh(Mesh& mesh)
    {
        ASSERT(!mesh.Vertices().empty());
        ASSERT(!mesh.Indices().empty());

        SubmitRenderCommand(
            [&mesh]()
            {
                mesh.VAO().Bind();
                const bool SUCCESS = RendererAPI().DrawIndexed(IRendererAPI::Primitive::TRIANGLES,
                                                               static_cast<std::uint32_t>(mesh.Indices().size()),
                                                               IRendererAPI::Type::UNSIGNED_INT);
                mesh.VAO().Unbind();
                return SUCCESS;
            });
    }

    // cppcheck-suppress unusedFunction
    void Renderer::DrawMesh(Mesh& mesh, IShaderProgram& shaderProgram)
    {
        ASSERT(!mesh.Vertices().empty());
        ASSERT(!mesh.Indices().empty());
        ASSERT(shaderProgram.Valid());

        SubmitRenderCommand(
            [&mesh, &shaderProgram]()
            {
                shaderProgram.Bind();
                mesh.VAO().Bind();
                const bool SUCCESS = RendererAPI().DrawIndexed(IRendererAPI::Primitive::TRIANGLES,
                                                               static_cast<std::uint32_t>(mesh.Indices().size()),
                                                               IRendererAPI::Type::UNSIGNED_INT);
                mesh.VAO().Unbind();
                shaderProgram.Unbind();
                return SUCCESS;
            });
    }

}  // namespace JE