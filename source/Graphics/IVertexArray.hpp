#pragma once

#include <span>
#include <utility>

#include "Memory.hpp"

namespace JE
{

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

    inline auto Bufers() const -> const Vector<Scope<IVertexBuffer>>& { return m_Buffers; }

    virtual auto Build() -> bool = 0;

    virtual auto Bind() -> bool = 0;

    virtual auto Unbind() -> bool = 0;

  private:
    Vector<Scope<IVertexBuffer>> m_Buffers;
};

}  // namespace JE