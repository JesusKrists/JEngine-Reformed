#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <string_view>

#include "Base.hpp"
#include "Events.hpp"

enum class TestEnum
{
    ZERO,
    ONE,
    TWO,
    COUNT
};

TEST_CASE(  // NOLINT(cert-err58-cpp,
            // cppcoreguidelines-avoid-non-const-global-variables)
    "constexpr Test Base macros constexpr",
    "[Base]")
{
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    STATIC_REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    STATIC_REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}

TEST_CASE("JE::BIT returns shifted bit from bit index", "[JE::BIT]")
{
    static constexpr auto FIRST_BIT = JE::Bit(0);
    static constexpr auto SECOND_BIT = JE::Bit(1);
    static constexpr auto THIRD_BIT = JE::Bit(2);

    STATIC_REQUIRE(FIRST_BIT == 0x01);
    STATIC_REQUIRE(SECOND_BIT == 0x02);
    STATIC_REQUIRE(THIRD_BIT == 0x04);
}

TEST_CASE("constexpr Test StaticType and Category/Type to string", "[Events]")
{
    STATIC_REQUIRE(JE::UnknownEvent::StaticType() == JE::IEvent::EventType::UNKNOWN);
    STATIC_REQUIRE(JE::QuitEvent::StaticType() == JE::IEvent::EventType::QUIT);

    STATIC_REQUIRE(JE::EventCategoryToString(JE::IEvent::EventCategory::UNKNOWN) == "UNKNOWN");
    STATIC_REQUIRE(JE::EventCategoryToString(JE::IEvent::EventCategory::APP) == "APP");

    STATIC_REQUIRE(JE::EventTypeToString(JE::IEvent::EventType::UNKNOWN) == "UNKNOWN");
    STATIC_REQUIRE(JE::EventTypeToString(JE::IEvent::EventType::QUIT) == "QUIT");
}