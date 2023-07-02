#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <cstdint>
#include <string>
#include <string_view>

#include <spdlog/fmt/bundled/core.h>

#include "Graphics/IRendererAPI.hpp"
#include "Types.hpp"

#define JE_ASSERT_BREAK_ON_FAIL false
#include "Application.hpp"
#include "Assert.hpp"
#include "Base.hpp"
#include "Events.hpp"
#include "Graphics/Renderer.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Platform.hpp"

struct TestGraphicsContext : JE::IGraphicsContext
{
    inline auto Created() const -> bool override { return true; }
    inline auto SwapBuffers() -> bool override { return true; }
};

struct TestWindow : JE::IWindow
{
    inline auto Created() const -> bool override { return true; }
    inline auto GraphicsContext() -> JE::IGraphicsContext& override { return m_GraphicsContext; }

    // cppcheck-suppress unusedFunction
    inline void Bind() override {}
    // cppcheck-suppress unusedFunction
    inline void Unbind() override {}

    TestGraphicsContext m_GraphicsContext;
};

struct TestPlatform : JE::IPlatform
{
    inline auto Name() const -> std::string_view override { return "TestPlatform"; }

    inline auto Initialize() -> bool override { return true; }
    inline auto Initialized() const -> bool override { return true; }
    inline auto GetLastError() const -> std::string_view override { return ""; }

    inline auto PollEvents([[maybe_unused]] JE::IEventProcessor& eventProcessor) -> bool override { return false; }

    inline auto CreateWindow([[maybe_unused]] std::string_view title, [[maybe_unused]] const JE::Size2D& size)
        -> JE::IWindow* override
    {
        return m_Windows.emplace_back(JE::CreateScope<TestWindow>()).get();
    }

    JE::Vector<JE::Scope<TestWindow>> m_Windows;  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,
                                                  // readability-identifier-naming)
};

struct TestRendererAPI : JE::IRendererAPI
{
    inline auto Name() const -> std::string_view override { return "TestRendererAPI"; }

    inline auto SetClearColor([[maybe_unused]] const JE::RGBA& color) -> bool override { return true; }
    inline auto ClearFramebuffer([[maybe_unused]] AttachmentFlags flags) -> bool override { return true; }
    inline auto BindFramebuffer([[maybe_unused]] FramebufferID bufferID) -> bool override { return true; }
    inline auto DrawIndexed([[maybe_unused]] Primitive primitiveType,
                            [[maybe_unused]] std::uint32_t indexCount,
                            [[maybe_unused]] Type indexType) -> bool override
    {
        return true;
    }
};

TEST_CASE(  // NOLINT(cert-err58-cpp,
            // cppcoreguidelines-avoid-non-const-global-variables)
    "Test Base macros",
    "[Base]")
{
    enum class TestEnum
    {
        ZERO,
        ONE,
        TWO,
        COUNT
    };

    REQUIRE(JE_STRINGIFY_MACRO(test == notest) == std::string("test == notest"));

    REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}

TEST_CASE("JE::BIT returns shifted bit from bit index", "[JE::BIT]")
{
    const auto FIRST_BIT = JE::Bit(0);
    const auto SECOND_BIT = JE::Bit(1);
    const auto THIRD_BIT = JE::Bit(2);

    REQUIRE(FIRST_BIT == 0x01);
    REQUIRE(SECOND_BIT == 0x02);
    REQUIRE(THIRD_BIT == 0x04);
}

TEST_CASE("Test Assert", "[Assert]")
{
    struct Library
    {
        std::string m_Name = fmt::format("{}", "JEngine-Reformed");
    };

    const Library LIB{};

    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Old") == false);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("Test ColorRGBA", "[Types]")
{
    static constexpr auto COLOR8_MAX_VALUE = 255u;
    static constexpr auto COLOR8_HALF_VALUE = 128u;
    static constexpr auto COLOR_HALF_VALUE = 0.5f;

    auto white = JE::RGBA{1.0f, 1.0f, 1.0f, 1.0f};
    REQUIRE(white.ToUint32() == 0xFFFFFFFF);
    auto green = JE::RGBA{0.0f, 1.0f, 0.0f, 1.0f};
    REQUIRE(green.ToUint32() == 0x00FF00FF);

    auto blue = JE::RGBA{0.0f, 0.0f, 1.0f, 0.0f};
    REQUIRE(JE::CompareFloat(blue.R(), 0.0f));
    REQUIRE(JE::CompareFloat(blue.G(), 0.0f));
    REQUIRE(JE::CompareFloat(blue.B(), 1.0f));
    REQUIRE(JE::CompareFloat(blue.A(), 0.0f));
    REQUIRE(blue.RToUint32() == 0);
    REQUIRE(blue.GToUint32() == 0);
    REQUIRE(blue.BToUint32() == 255);
    REQUIRE(blue.AToUint32() == 0);
    REQUIRE(blue.ToUint32() == 0x0000FF00);

    blue.SetR(COLOR8_MAX_VALUE);
    REQUIRE(JE::CompareFloat(blue.R(), 1.0f));
    blue.SetR(COLOR_HALF_VALUE);
    REQUIRE(JE::CompareFloat(blue.R(), COLOR_HALF_VALUE));
    REQUIRE(blue.RToUint32() == COLOR8_HALF_VALUE);

    blue.SetG(COLOR8_MAX_VALUE);
    REQUIRE(JE::CompareFloat(blue.G(), 1.0f));
    blue.SetG(COLOR_HALF_VALUE);
    REQUIRE(JE::CompareFloat(blue.G(), COLOR_HALF_VALUE));
    REQUIRE(blue.GToUint32() == COLOR8_HALF_VALUE);

    blue.SetB(COLOR8_MAX_VALUE);
    REQUIRE(JE::CompareFloat(blue.B(), 1.0f));
    blue.SetB(COLOR_HALF_VALUE);
    REQUIRE(JE::CompareFloat(blue.B(), COLOR_HALF_VALUE));
    REQUIRE(blue.BToUint32() == COLOR8_HALF_VALUE);

    blue.SetA(COLOR8_MAX_VALUE);
    REQUIRE(JE::CompareFloat(blue.A(), 1.0f));
    blue.SetA(COLOR_HALF_VALUE);
    REQUIRE(JE::CompareFloat(blue.A(), COLOR_HALF_VALUE));
    REQUIRE(blue.AToUint32() == COLOR8_HALF_VALUE);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}

TEST_CASE("Test Platform initialization, window creation and OpenGL context creation",
          "[Application][Platform][OpenGL]")
{
    REQUIRE(JE::Application().Initialized());

    REQUIRE(JE::Application().MainWindow().Created());
    REQUIRE(JE::Application().MainWindow().GraphicsContext().Created());
}

TEST_CASE("Test Application creation and main loop", "[Application]")
{
    REQUIRE(JE::Application().Initialized());
    REQUIRE(JE::Application().LoopCount() == 0);

    JE::Application().Loop(1);

    REQUIRE(JE::Application().LoopCount() == 1);
    REQUIRE(JE::Application().EventsProcessed() != 0);
}

TEST_CASE("Test StaticType and Category/Type to string", "[Events]")
{
    REQUIRE(JE::UnknownEvent::StaticType() == JE::IEvent::EventType::UNKNOWN);
    REQUIRE(JE::QuitEvent::StaticType() == JE::IEvent::EventType::QUIT);

    REQUIRE(JE::ToString(JE::IEvent::EventCategory::UNKNOWN) == "UNKNOWN");
    REQUIRE(JE::ToString(JE::IEvent::EventCategory::APP) == "APP");

    REQUIRE(JE::ToString(JE::IEvent::EventType::UNKNOWN) == "UNKNOWN");
    REQUIRE(JE::ToString(JE::IEvent::EventType::QUIT) == "QUIT");
}

TEST_CASE("Test EventDispatcher and Event handling", "[Events]")
{
    JE::UnknownEvent event;

    JE::EventDispatcher dispatcher{event};
    REQUIRE(!event.Handled());

    bool dispatched = dispatcher.Dispatch<JE::UnknownEvent>([]([[maybe_unused]] auto& evnt) { return true; });

    REQUIRE(dispatched);
    REQUIRE(event.Handled());

    dispatched = dispatcher.Dispatch<JE::UnknownEvent>([]([[maybe_unused]] auto& evnt) { return true; });

    REQUIRE(!dispatched);
}

TEST_CASE("Test Application initialization failure (Initialization failure)", "[Application]")
{
    struct InitFailPlatform : TestPlatform
    {
        inline auto Initialize() -> bool override { return false; }
    };

    JE::detail::InjectCustomEnginePlatform<InitFailPlatform>();

    REQUIRE(!JE::Application().Initialized());
}

TEST_CASE("Test Application initialization failure (Window creation failure)", "[Application]")
{
    struct FailWindow : TestWindow
    {
        inline auto Created() const -> bool override { return false; }
    };

    struct WindowFailPlatform : TestPlatform
    {
        inline auto CreateWindow([[maybe_unused]] std::string_view title, [[maybe_unused]] const JE::Size2D& size)
            -> JE::IWindow* override
        {
            return m_Windows.emplace_back(JE::CreateScope<FailWindow>()).get();
        }
    };

    JE::detail::InjectCustomEnginePlatform<WindowFailPlatform>();

    REQUIRE(!JE::Application().Initialized());
}

TEST_CASE("Test Application QuitEvent handling", "[Application][Events]")
{
    static constexpr auto MAX_LOOP_COUNT = 100;

    struct QuitEventPlatform : TestPlatform
    {
        inline auto PollEvents(JE::IEventProcessor& eventProcessor) -> bool override
        {
            if (!m_EventProcessed) {
                JE::QuitEvent event;
                eventProcessor.ProcessEvent(event);

                m_EventProcessed = true;
                return true;
            }

            return false;
        }

        bool m_EventProcessed = false;
    };

    JE::detail::InjectCustomEnginePlatform<QuitEventPlatform>();
    JE::detail::InjectCustomRendererAPI<TestRendererAPI>();

    JE::Application().Loop(MAX_LOOP_COUNT);

    REQUIRE(!JE::Application().Running());
    REQUIRE(JE::Application().LoopCount() == 1);
    REQUIRE(JE::Application().EventsProcessed() == 1);
}

TEST_CASE("Test Application Renderer command queue processing", "[Application][Renderer]")
{
    static constexpr auto CLEAR_COLOR = JE::RGBA{1.f, 1.f, 1.f, 1.f};

    JE::Application().Renderer().Begin(&JE::Application().MainWindow(), CLEAR_COLOR);
    JE::Application().Renderer().End();

    REQUIRE(!JE::Application().Renderer().CommandQueue().empty());

    JE::Application().Loop(1);

    REQUIRE(JE::Application().Renderer().CommandQueue().empty());
}