#include "lib.hpp"

#include <fmt/core.h>
#include <tracy/Tracy.hpp>

Library::Library()
    : m_name{fmt::format("{}", "JEngine-Reformed")}
{
    ZoneScoped;
}
