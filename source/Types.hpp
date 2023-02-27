#pragma once

#include <cstdint>

#include <fmt/core.h>
#include <fmt/format.h>

namespace JE
{

struct Size2D
{
    std::int32_t x = 0;  // NOLINT(readability-identifier-naming)
    std::int32_t y = 0;  // NOLINT(readability-identifier-naming)
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
        return format_to(ctx.out(), "Size2D{{ X:{} Y:{} }}", size.x, size.y);
    }
};