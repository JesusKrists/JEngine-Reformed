#pragma once

// IWYU pragma: no_include <glm/detail/type_vec3.inl>

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <span>
#include <string>
#include <utility>

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_vec3.hpp>

#include "Assert.hpp"
#include "IRendererAPI.hpp"
#include "Logger.hpp"
#include "Memory.hpp"

namespace JE
{

struct ColorRGBA;

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

enum class AttributeType
{
    FLOAT3
};

inline constexpr auto FLOAT3_STRIDE = 12;

class AttributeLayout
{
  public:
    struct Attribute
    {
        std::string Name;  // NOLINT(readability-identifier-naming)
        AttributeType Type = AttributeType::FLOAT3;  // NOLINT(readability-identifier-naming)
        std::uint32_t Offset = 0;  // NOLINT(readability-identifier-naming)
    };

    AttributeLayout() = default;

    explicit AttributeLayout(const std::span<const Attribute> ATTRIBUTES)
        : m_Attributes(std::begin(ATTRIBUTES), std::end(ATTRIBUTES))
    {
        for (auto& attribute : m_Attributes) {
            if (attribute.Type == AttributeType::FLOAT3) {
                attribute.Offset = m_Stride;
                m_Stride += FLOAT3_STRIDE;
            }
        }
    }

  private:
    std::uint32_t m_Stride = 0;
    Vector<Attribute> m_Attributes;
};

class IVertexBuffer
{
  public:
    IVertexBuffer(const IVertexBuffer& other) = delete;
    IVertexBuffer(IVertexBuffer&& other) = delete;
    auto operator=(const IVertexBuffer& other) -> IVertexBuffer& = delete;
    auto operator=(IVertexBuffer&& other) -> IVertexBuffer& = delete;

    IVertexBuffer() = default;
    virtual ~IVertexBuffer() = default;

    virtual auto Bind() -> bool = 0;

    virtual auto Unbind() -> bool = 0;
};

class IVertexArray
{
  public:
    IVertexArray(const IVertexArray& other) = delete;
    IVertexArray(IVertexArray&& other) = delete;
    auto operator=(const IVertexArray& other) -> IVertexArray& = delete;
    auto operator=(IVertexArray&& other) -> IVertexArray& = delete;

    IVertexArray() = default;
    virtual ~IVertexArray() = default;

    inline auto Buffers() const -> const Vector<Scope<IVertexBuffer>>& { return m_Buffers; }

    virtual auto Build() -> bool = 0;

    virtual auto Bind() -> bool = 0;

    virtual auto Unbind() -> bool = 0;

  private:
    Vector<Scope<IVertexBuffer>> m_Buffers;
};

using VertexType = glm::tvec3<float>;
using IndexType = std::uint32_t;

class Mesh
{
  public:
    Mesh(const Mesh& other) = default;
    Mesh(Mesh&& other) noexcept
        : m_Vertices(std::move(other.m_Vertices))
        , m_Indices(std::move(other.m_Indices))
    {
    }
    auto operator=(const Mesh& other) -> Mesh& = default;
    auto operator=(Mesh&& other) noexcept -> Mesh&
    {
        m_Vertices = std::move(other.m_Vertices);
        m_Indices = std::move(other.m_Indices);
        return *this;
    }

    Mesh() = default;

    Mesh(const std::span<const VertexType> VERTICES, const std::span<const IndexType> INDICES)
        : m_Vertices(std::begin(VERTICES), std::end(VERTICES))
        , m_Indices(std::begin(INDICES), std::end(INDICES))
    {
    }

    Mesh(const std::initializer_list<VertexType> VERTICES, const std::initializer_list<IndexType> INDICES)
        : m_Vertices(std::begin(VERTICES), std::end(VERTICES))
        , m_Indices(std::begin(INDICES), std::end(INDICES))
    {
    }

    ~Mesh() = default;

    inline auto Vertices() const -> const Vector<VertexType>& { return m_Vertices; }
    inline auto Indices() const -> const Vector<IndexType>& { return m_Indices; }

  private:
    Vector<VertexType> m_Vertices;
    Vector<IndexType> m_Indices;
};

inline auto CreateTriangleMesh()
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    return Mesh{{{-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}}, {0, 1, 2}};
}

inline auto CreateQuadMesh()
{
    return Mesh{{{0.5f, 0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                 {0.5f, -0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                 {-0.5f, -0.5f, 0.0f},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                 {-0.5f, 0.5f, 0.0f}},  // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                {0, 1, 3, 1, 2, 3}};
}

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

    void DrawMesh(const Mesh& mesh);

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