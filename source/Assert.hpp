#pragma once

#include <iostream>
#include <source_location>
#include <string>

#include "Base.hpp"

namespace JE
{

template<bool ENABLED = false, bool BREAK = true>
inline auto Assert(const bool CHECK,
                   const std::string_view ASSERTION,
                   const std::source_location LOCATION =
                       std::source_location::current()) -> bool
{
    if constexpr (ENABLED) {
        if (!CHECK) {
            std::cout << "Assertion `" << ASSERTION << "` in `"
                      << LOCATION.function_name() << "` at "
                      << LOCATION.file_name() << ":" << LOCATION.line() << ":"
                      << LOCATION.column() << " failed!\n";

            if constexpr (BREAK) {
                DEBUGBREAK();
            }
        }
    }

    return CHECK;
}

}  // namespace JE

#if !defined(JE_ASSERT_BREAK_ON_FAIL)
#    define JE_ASSERT_BREAK_ON_FAIL true
#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASSERT(check) \
    JE::Assert<JE_ASSERTS_ENABLED, JE_ASSERT_BREAK_ON_FAIL>( \
        check, JE_STRINGIFY_MACRO(check))
