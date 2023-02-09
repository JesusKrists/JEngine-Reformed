#include "lib.hpp"

#include <fmt/core.h>
#include <tracy/Tracy.hpp>

Library::Library()
    : mName{fmt::format("{}", "JEngine-Reformed")}
{
    ZoneScoped;
}
