#pragma once

#include <source_location>

#include "Logger.hpp"

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

#if !defined(JE_ASSERT_BREAK_ON_FAIL)
#    define JE_ASSERT_BREAK_ON_FAIL true
#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASSERT(check) \
    JE::detail::Assert<JE::ASSERTS_ENABLED, JE_ASSERT_BREAK_ON_FAIL>( \
        check, JE_STRINGIFY_MACRO(check))
