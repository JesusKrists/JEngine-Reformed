#include "lib.hpp"

#include <Tracy.hpp>
#include <fmt/core.h>

Library::Library()
    : m_name{fmt::format("{}", "JEngine-Reformed")}
{
    ZoneScoped;
}
