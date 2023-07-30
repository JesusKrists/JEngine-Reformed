#pragma once

// IWYU pragma: no_include <glm/detail/type_vec3.inl>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_vec3.hpp>

#include "Assert.hpp"
#include "IRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{

    struct RGBA;

    class IRenderTarget
    {
      public:
        IRenderTarget(const IRenderTarget& other) = delete;
        IRenderTarget(IRenderTarget&& other) = delete;
        auto operator=(const IRenderTarget& other) -> IRenderTarget& = delete;
        auto operator=(IRenderTarget&& other) -> IRenderTarget& = delete;

        IRenderTarget() = default;
        virtual ~IRenderTarget() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    };

    class AttributeLayout
    {
      public:
        struct Attribute
        {
            Attribute(const std::string_view NAME,
                      IRendererAPI::Type type,
                      std::uint32_t component_count,
                      bool normalized = true)
                : Name(NAME)
                , Type(type)
                , ComponentCount(component_count)
                , Normalized(normalized)
            {
            }

            std::string Name;
            IRendererAPI::Type Type = IRendererAPI::Type::FLOAT;
            std::size_t ComponentCount = 0;
            bool Normalized = false;
            std::size_t Offset = 0;
        };

        AttributeLayout() = default;

        AttributeLayout(const std::initializer_list<const Attribute> ATTRIBUTES)
            : m_Attributes(std::begin(ATTRIBUTES), std::end(ATTRIBUTES))
        {
            for (auto& attribute : m_Attributes) {
                if (attribute.Type == IRendererAPI::Type::FLOAT) {
                    attribute.Offset = m_Stride;
                    m_Stride += TypeByteCount(attribute.Type) * attribute.ComponentCount;
                }
            }
        }

        inline auto Count() const -> std::size_t { return m_Attributes.size(); }
        inline auto Stride() const -> std::size_t { return m_Stride; }

        inline auto begin() const { return std::begin(m_Attributes); }  // NOLINT(readability-identifier-naming)
        inline auto end() const { return std::end(m_Attributes); }  // NOLINT(readability-identifier-naming)
        inline auto operator[](std::size_t index) const -> const Attribute& { return m_Attributes[index]; }

      private:
        std::size_t m_Stride = 0;
        Vector<Attribute> m_Attributes;
    };

    class IVertexBuffer
    {
      public:
        IVertexBuffer(const IVertexBuffer& other) = delete;
        IVertexBuffer(IVertexBuffer&& other) = delete;
        auto operator=(const IVertexBuffer& other) -> IVertexBuffer& = delete;
        auto operator=(IVertexBuffer&& other) -> IVertexBuffer& = delete;

        explicit IVertexBuffer(AttributeLayout layout)
            : m_Layout(std::move(layout))
        {
        }
        virtual ~IVertexBuffer() = default;

        inline auto ID() const -> IRendererAPI::BufferID { return m_BufferID; }

        virtual auto Bind() -> bool = 0;

        virtual auto Unbind() -> bool = 0;

        virtual auto SetData(std::span<const std::byte> data) -> bool = 0;

        inline auto Layout() const -> const AttributeLayout& { return m_Layout; }

        virtual auto UploadLayout() -> bool = 0;

      protected:
        IRendererAPI::BufferID m_BufferID = 0;
        AttributeLayout m_Layout;
    };

    auto CreateVertexBuffer(const AttributeLayout& layout) -> Scope<IVertexBuffer>;

    class IElementBuffer
    {
      public:
        IElementBuffer(const IElementBuffer& other) = delete;
        IElementBuffer(IElementBuffer&& other) = delete;
        auto operator=(const IElementBuffer& other) -> IElementBuffer& = delete;
        auto operator=(IElementBuffer&& other) -> IElementBuffer& = delete;

        IElementBuffer() = default;
        virtual ~IElementBuffer() = default;

        inline auto ID() const -> IRendererAPI::BufferID { return m_BufferID; }

        virtual auto Bind() -> bool = 0;

        virtual auto Unbind() -> bool = 0;

        virtual auto SetData(std::span<const std::byte> data) -> bool = 0;

      protected:
        IRendererAPI::BufferID m_BufferID = 0;
    };

    auto CreateElementBuffer() -> Scope<IElementBuffer>;

    class IVertexArray
    {
      public:
        IVertexArray(const IVertexArray& other) = delete;
        IVertexArray(IVertexArray&& other) = delete;
        auto operator=(const IVertexArray& other) -> IVertexArray& = delete;
        auto operator=(IVertexArray&& other) -> IVertexArray& = delete;

        IVertexArray() = default;
        virtual ~IVertexArray() = default;

        inline auto ID() const -> IRendererAPI::BufferID { return m_VAOId; }

        inline auto Buffers() const -> const Vector<Scope<IVertexBuffer>>& { return m_VertexBuffers; }
        inline void AddBuffer(Scope<IVertexBuffer> buffer) { m_VertexBuffers.emplace_back(std::move(buffer)); }

        inline void SetIndexBuffer(Scope<IElementBuffer> buffer) { m_IndexBuffer = std::move(buffer); }

        virtual auto Build() -> bool = 0;

        virtual auto Bind() -> bool = 0;

        virtual auto Unbind() -> bool = 0;

      protected:
        IRendererAPI::BufferID m_VAOId = 0;
        Vector<Scope<IVertexBuffer>> m_VertexBuffers;
        Scope<IElementBuffer> m_IndexBuffer;
    };

    auto CreateVertexArray() -> Scope<IVertexArray>;

    using VertexType = glm::tvec3<float>;
    using IndexType = std::uint32_t;

    class Mesh
    {
      public:
        Mesh() = default;

        Mesh(const std::span<const VertexType> VERTICES, const std::span<const IndexType> INDICES)
            : m_Vertices(std::begin(VERTICES), std::end(VERTICES))
            , m_Indices(std::begin(INDICES), std::end(INDICES))
        {
            UploadMesh();
        }

        Mesh(const std::initializer_list<VertexType> VERTICES, const std::initializer_list<IndexType> INDICES)
            : m_Vertices(std::begin(VERTICES), std::end(VERTICES))
            , m_Indices(std::begin(INDICES), std::end(INDICES))
        {
            UploadMesh();
        }

        inline auto Vertices() const -> const Vector<VertexType>& { return m_Vertices; }
        inline auto Indices() const -> const Vector<IndexType>& { return m_Indices; }
        inline auto VAO() -> IVertexArray& { return *m_VAO; }

      private:
        inline void UploadMesh()
        {
            auto vertex_buffer = CreateVertexBuffer(
                AttributeLayout{{AttributeLayout::Attribute{"a_VertexPos", IRendererAPI::Type::FLOAT, 3}}});
            auto index_buffer = CreateElementBuffer();

            vertex_buffer->Bind();
            vertex_buffer->SetData(
                {reinterpret_cast<std::byte*>(m_Vertices.data()), m_Vertices.size() * sizeof(VertexType)});
            vertex_buffer->Unbind();

            index_buffer->Bind();
            index_buffer->SetData(
                {reinterpret_cast<std::byte*>(m_Indices.data()), m_Indices.size() * sizeof(IndexType)});
            index_buffer->Unbind();

            m_VAO = CreateVertexArray();
            m_VAO->AddBuffer(std::move(vertex_buffer));
            m_VAO->SetIndexBuffer(std::move(index_buffer));
            m_VAO->Build();
        }

        Vector<VertexType> m_Vertices;
        Vector<IndexType> m_Indices;
        Scope<IVertexArray> m_VAO;
    };

    inline auto CreateTriangleMesh()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        return Mesh{{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}}, {0, 1, 2}};
    }

    inline auto CreateQuadMesh()
    {
        return Mesh{{{-0.5f, 0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                     {-0.5f, -0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                     {0.5f, -0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                     {0.5f, 0.5f, 0.0f}},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                    {0, 1, 2, 2, 3, 0}};
    }

    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    class IShaderProgram
    {
      public:
        IShaderProgram(const IShaderProgram& other) = delete;
        IShaderProgram(IShaderProgram&& other) = delete;
        auto operator=(const IShaderProgram& other) -> IShaderProgram& = delete;
        auto operator=(IShaderProgram&& other) -> IShaderProgram& = delete;

        explicit IShaderProgram(std::string_view debug_name)
            : m_DebugName(debug_name)
        {
        }
        virtual ~IShaderProgram() = default;

        inline auto ID() const -> IRendererAPI::ProgramID { return m_ProgramID; }

        virtual auto Bind() -> bool = 0;

        virtual auto Unbind() -> bool = 0;

        inline auto Valid() const -> bool { return m_Valid; }

      protected:
        std::string m_DebugName;
        IRendererAPI::ProgramID m_ProgramID = 0;
        bool m_Valid = false;
    };

    auto CreateShader(std::string_view debug_name, std::string_view vertex_source, std::string_view fragment_source)
        -> Scope<IShaderProgram>;

    class Renderer
    {
        friend class App;

      public:
        using RenderCommand = std::function<bool()>;

        static constexpr IRendererAPI::AttachmentFlags DEFAULT_ATTACHMENT_FLAGS = IRendererAPI::AttachmentFlag::COLOR
            | IRendererAPI::AttachmentFlag::DEPTH | IRendererAPI::AttachmentFlag::STENCIL;
        Renderer() = default;

        void Begin(IRenderTarget* target, const RGBA& color);
        void End();

        void DrawMesh(Mesh& mesh);
        void DrawMesh(Mesh& mesh, IShaderProgram& shader_program);

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