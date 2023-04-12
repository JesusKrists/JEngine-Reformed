#pragma once

#include <span>

#include <glm/glm.hpp>

#include "Memory.hpp"

namespace JE
{

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

}  // namespace JE