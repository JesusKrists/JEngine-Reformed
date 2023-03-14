#pragma once

#include <cmath>
#include <cstdint>

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4365)
#endif

#include <fmt/core.h>
#include <fmt/format.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include <glm/glm.hpp>

namespace JE
{

struct Size2D
{
    std::int32_t x = 0;  // NOLINT(readability-identifier-naming)
    std::int32_t y = 0;  // NOLINT(readability-identifier-naming)
};

struct ColorRGBA
{
    static constexpr auto COLOR8_MAX_VALUE = 255.f;

    static constexpr auto R_BIT_OFFSET = 24u;
    static constexpr auto G_BIT_OFFSET = 16u;
    static constexpr auto B_BIT_OFFSET = 8u;
    static constexpr auto A_BIT_OFFSET = 0u;

    static constexpr auto R_BYTE_MASK = 0xFF000000u;
    static constexpr auto G_BYTE_MASK = 0xFF0000u;
    static constexpr auto B_BYTE_MASK = 0xFF00u;
    static constexpr auto A_BYTE_MASK = 0xFFu;

    constexpr ColorRGBA() = default;
    constexpr ColorRGBA(float red, float green, float blue, float alpha)
        : color(red, green, blue, alpha)
    {
    }
    constexpr ColorRGBA(std::uint32_t red, std::uint32_t green, std::uint32_t blue, std::uint32_t alpha)
        : color(static_cast<float>(red) / COLOR8_MAX_VALUE,
                static_cast<float>(green) / COLOR8_MAX_VALUE,
                static_cast<float>(blue) / COLOR8_MAX_VALUE,
                static_cast<float>(alpha) / COLOR8_MAX_VALUE)
    {
    }

    inline auto ToUint32() const -> std::uint32_t
    {
        auto colR = RToUint32();
        auto colG = GToUint32();
        auto colB = BToUint32();
        auto colA = AToUint32();

        return ((colR << R_BIT_OFFSET) & R_BYTE_MASK) | ((colG << G_BIT_OFFSET) & G_BYTE_MASK)
            | ((colB << B_BIT_OFFSET) & B_BYTE_MASK) | (colA & A_BYTE_MASK);
    }

    constexpr auto R() const -> float { return color.r; }  // NOLINT(cppcoreguidelines-pro-type-union-access)
    constexpr auto G() const -> float { return color.g; }  // NOLINT(cppcoreguidelines-pro-type-union-access)
    constexpr auto B() const -> float { return color.b; }  // NOLINT(cppcoreguidelines-pro-type-union-access)
    constexpr auto A() const -> float { return color.a; }  // NOLINT(cppcoreguidelines-pro-type-union-access)

    inline auto RToUint32() const -> std::uint32_t
    {
        return static_cast<std::uint32_t>(
            std::roundf(color.r * COLOR8_MAX_VALUE));  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    inline auto GToUint32() const -> std::uint32_t
    {
        return static_cast<std::uint32_t>(
            std::roundf(color.g * COLOR8_MAX_VALUE));  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    inline auto BToUint32() const -> std::uint32_t
    {
        return static_cast<std::uint32_t>(
            std::roundf(color.b * COLOR8_MAX_VALUE));  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    inline auto AToUint32() const -> std::uint32_t
    {
        return static_cast<std::uint32_t>(
            std::roundf(color.a * COLOR8_MAX_VALUE));  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }

    constexpr void SetR(std::uint32_t red)
    {
        color.r = static_cast<float>(red) / COLOR8_MAX_VALUE;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetG(std::uint32_t green)
    {
        color.g = static_cast<float>(green) / COLOR8_MAX_VALUE;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetB(std::uint32_t blue)
    {
        color.b = static_cast<float>(blue) / COLOR8_MAX_VALUE;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetA(std::uint32_t alpha)
    {
        color.a = static_cast<float>(alpha) / COLOR8_MAX_VALUE;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }

    constexpr void SetR(float red)
    {
        color.r = red;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetG(float green)
    {
        color.g = green;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetB(float blue)
    {
        color.b = blue;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }
    constexpr void SetA(float alpha)
    {
        color.a = alpha;  // NOLINT(cppcoreguidelines-pro-type-union-access)
    }

    glm::vec4 color = {0, 0, 0, 0};  // NOLINT(readability-identifier-naming)
};

}  // namespace JE

template<>
struct fmt::formatter<JE::Size2D>
{
    // cppcheck-suppress functionStatic
    constexpr auto parse(format_parse_context& ctx)  // NOLINT
        -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    // cppcheck-suppress functionStatic
    auto format(const JE::Size2D& size, FormatContext& ctx)  // NOLINT
        -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "Size2D{{ X:{} Y:{} }}", size.x, size.y);
    }
};

template<>
struct fmt::formatter<JE::ColorRGBA>
{
    // cppcheck-suppress functionStatic
    constexpr auto parse(format_parse_context& ctx)  // NOLINT
        -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    // cppcheck-suppress functionStatic
    auto format(const JE::ColorRGBA& color, FormatContext& ctx)  // NOLINT
        -> decltype(ctx.out())
    {
        return fmt::format_to(
            ctx.out(), "ColorRGBA{{ R:{} G:{} B:{} A:{} }}", color.R(), color.G(), color.B(), color.A());
    }
};