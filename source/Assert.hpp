#pragma once

#include "Logger.hpp"

#if !defined(JE_ASSERT_BREAK_ON_FAIL)
#    define JE_ASSERT_BREAK_ON_FAIL true
#endif

#if !JE_PLATFORM_APPLE_VALUE && !JE_PLATFORM_CLANG_ENV_VALUE
#    include <source_location>

namespace JE::detail
{

template<bool ENABLED = false, bool BREAK = true>
inline auto Assert(const bool CHECK,
                   const std::string_view ASSERTION,
                   const std::source_location LOCATION =
                       std::source_location::current()) -> bool
{
    if constexpr (ENABLED) {
        if (!CHECK) {
            EngineLogger()->error("Assertion `{}` in `{}` at {}:{}:{} failed!",
                                  ASSERTION,
                                  LOCATION.function_name(),
                                  LOCATION.file_name(),
                                  LOCATION.line(),
                                  LOCATION.column());

            if constexpr (BREAK) {
                DEBUGBREAK();
            }
        }
    }

    return CHECK;
}

}  // namespace JE::detail

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#    define ASSERT(check) \
        JE::detail::Assert<JE::ASSERTS_ENABLED, JE_ASSERT_BREAK_ON_FAIL>( \
            check, JE_STRINGIFY_MACRO(check))

#else

namespace JE::detail
{

template<bool ENABLED = false, bool BREAK = true>
inline auto Assert(const bool CHECK,
                   const std::string_view ASSERTION,
                   const std::string_view FILE,
                   const int LINE) -> bool
{
    if constexpr (ENABLED) {
        if (!CHECK) {
            EngineLogger()->error(
                "Assertion `{}` at {}:{} failed!", ASSERTION, FILE, LINE);

            if constexpr (BREAK) {
                DEBUGBREAK();
            }
        }
    }

    return CHECK;
}

}  // namespace JE::detail

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#    define ASSERT(check) \
        JE::detail::Assert<JE::ASSERTS_ENABLED, JE_ASSERT_BREAK_ON_FAIL>( \
            check, JE_STRINGIFY_MACRO(check), __FILE__, __LINE__)

#endif