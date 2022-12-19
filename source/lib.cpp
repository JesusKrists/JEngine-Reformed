#include "lib.hpp"

#include <Tracy.hpp>
#include <fmt/core.h>

library::library()
    : name {fmt::format("{}", "JEngine3D")}
{
    ZoneScoped;
}
