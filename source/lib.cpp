#include "lib.hpp"

#include <fmt/core.h>
#include <nanosvg.h>

library::library()
    : name {fmt::format("{}", "JEngine-Reformed")}
{
    ZoneScoped;
}
