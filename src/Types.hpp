#pragma once

#include <cmath>
#include <cstdint>

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4365)
#endif

#include <spdlog/fmt/bundled/core.h>
#include <spdlog/fmt/bundled/format.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include <glm/glm.hpp>

namespace JE
{

    struct Size2D
    {
        std::int32_t X = 0;
        std::int32_t Y = 0;
    };

    struct RGBA
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

        constexpr RGBA() = default;
        constexpr RGBA(float red, float green, float blue, float alpha)
            : Color(red, green, blue, alpha)
        {
        }
        constexpr RGBA(std::uint32_t red, std::uint32_t green, std::uint32_t blue, std::uint32_t alpha)
            : Color(static_cast<float>(red) / COLOR8_MAX_VALUE,
                    static_cast<float>(green) / COLOR8_MAX_VALUE,
                    static_cast<float>(blue) / COLOR8_MAX_VALUE,
                    static_cast<float>(alpha) / COLOR8_MAX_VALUE)
        {
        }

        inline auto ToUint32() const -> std::uint32_t
        {
            auto col_r = RToUint32();
            auto col_g = GToUint32();
            auto col_b = BToUint32();
            auto col_a = AToUint32();

            return ((col_r << R_BIT_OFFSET) & R_BYTE_MASK) | ((col_g << G_BIT_OFFSET) & G_BYTE_MASK)
                | ((col_b << B_BIT_OFFSET) & B_BYTE_MASK) | (col_a & A_BYTE_MASK);
        }

        constexpr auto R() const -> float { return Color.r; }
        constexpr auto G() const -> float { return Color.g; }
        constexpr auto B() const -> float { return Color.b; }
        constexpr auto A() const -> float { return Color.a; }

        inline auto RToUint32() const -> std::uint32_t
        {
            return static_cast<std::uint32_t>(std::roundf(Color.r * COLOR8_MAX_VALUE));
        }
        inline auto GToUint32() const -> std::uint32_t
        {
            return static_cast<std::uint32_t>(std::roundf(Color.g * COLOR8_MAX_VALUE));
        }
        inline auto BToUint32() const -> std::uint32_t
        {
            return static_cast<std::uint32_t>(std::roundf(Color.b * COLOR8_MAX_VALUE));
        }
        inline auto AToUint32() const -> std::uint32_t
        {
            return static_cast<std::uint32_t>(std::roundf(Color.a * COLOR8_MAX_VALUE));
        }

        constexpr void SetR(std::uint32_t red) { Color.r = static_cast<float>(red) / COLOR8_MAX_VALUE; }
        constexpr void SetG(std::uint32_t green) { Color.g = static_cast<float>(green) / COLOR8_MAX_VALUE; }
        constexpr void SetB(std::uint32_t blue) { Color.b = static_cast<float>(blue) / COLOR8_MAX_VALUE; }
        constexpr void SetA(std::uint32_t alpha) { Color.a = static_cast<float>(alpha) / COLOR8_MAX_VALUE; }

        constexpr void SetR(float red) { Color.r = red; }
        constexpr void SetG(float green) { Color.g = green; }
        constexpr void SetB(float blue) { Color.b = blue; }
        constexpr void SetA(float alpha) { Color.a = alpha; }

        glm::vec4 Color = {0, 0, 0, 0};
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
        return fmt::format_to(ctx.out(), "Size2D{{ X:{} Y:{} }}", size.X, size.Y);
    }
};

template<>
struct fmt::formatter<JE::RGBA>
{
    // cppcheck-suppress functionStatic
    constexpr auto parse(format_parse_context& ctx)  // NOLINT
        -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    // cppcheck-suppress functionStatic
    auto format(const JE::RGBA& color, FormatContext& ctx)  // NOLINT
        -> decltype(ctx.out())
    {
        return fmt::format_to(
            ctx.out(), "ColorRGBA{{ R:{} G:{} B:{} A:{} }}", color.R(), color.G(), color.B(), color.A());
    }
};