#pragma once

#if JE_PLATFORM_UNIX_VALUE
#    include <csignal>
#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JE_STRINGIFY_MACRO(x) #x

#include <cstdint>
#include <type_traits>

namespace JE
{

inline constexpr bool PLATFORM_WINDOWS = JE_PLATFORM_WINDOWS_VALUE;
inline constexpr bool PLATFORM_UNIX = JE_PLATFORM_UNIX_VALUE;
inline constexpr bool PLATFORM_APPLE = JE_PLATFORM_APPLE_VALUE;

inline constexpr bool RELEASE_BUILD = JE_RELEASE_BUILD_VALUE;
inline constexpr bool ASSERTS_ENABLED = JE_ASSERTS_ENABLED_VALUE;

inline auto DEBUGBREAK() -> std::int32_t
{
#if JE_PLATFORM_WINDOWS_VALUE
    __debugbreak();
    return -1;
#elif JE_PLATFORM_UNIX_VALUE
    return raise(SIGTRAP);
#else
#    error "Unsupported platform"
#endif
}

template<typename T>
constexpr auto EnumToInt(const T ENUM_VALUE) -> std::int32_t
{
    static_assert(std::is_enum_v<T>, "Can only be used with enum types");
    return static_cast<std::int32_t>(ENUM_VALUE);
}

}  // namespace JE