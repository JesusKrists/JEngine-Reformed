#pragma once

#if JE_PLATFORM_UNIX_VALUE
#    include <csignal>
#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define JE_STRINGIFY_MACRO(x) #x

inline constexpr bool JE_PLATFORM_WINDOWS = JE_PLATFORM_WINDOWS_VALUE;
inline constexpr bool JE_PLATFORM_UNIX = JE_PLATFORM_UNIX_VALUE;
inline constexpr bool JE_PLATFORM_APPLE = JE_PLATFORM_APPLE_VALUE;

inline constexpr bool JE_RELEASE_BUILD = JE_RELEASE_BUILD_VALUE;
inline constexpr bool JE_ASSERTS_ENABLED = JE_ASSERTS_ENABLED_VALUE;

namespace JE
{

inline auto DEBUGBREAK() -> int
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

}  // namespace JE