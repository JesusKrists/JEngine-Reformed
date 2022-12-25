#include "lib.hpp"

#include <Tracy.hpp>
#include <fmt/core.h>

Library::Library()
    : name{fmt::format("{}", "JEngine-Reformed")}
{
    ZoneScoped;
}
