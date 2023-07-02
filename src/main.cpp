#include <cstdint>

#include "Application.hpp"

auto main() -> std::int32_t
{
    if (!JE::Application().Initialized()) {
        return -1;
    }

    JE::Application().Loop();
}
