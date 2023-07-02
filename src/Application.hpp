#pragma once

#include "Assert.hpp"
#include "Base.hpp"
#include "Events.hpp"
#include "Graphics/Renderer.hpp"
#include "Platform.hpp"
#include "Sound/ImpulseAudio.hpp"
#include "Types.hpp"

namespace JE
{

inline constexpr auto VERTEX_SOURCE = R"(
                                                            #version 330 core
                                                            layout (location = 0) in vec3 a_VertexPos;
                                                            void main()
                                                            {
                                                                gl_Position = vec4(a_VertexPos.xyz, 1.0);
                                                            }
                                                            )";

inline constexpr auto FRAGMENT_SOURCE = R"(
                                                            #version 330 core
                                                            out vec4 out_FragColor;
                                                            void main()
                                                            {
                                                                out_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                                                            } 
                                                            )";

class App final : public IEventProcessor
{
    friend auto Application() -> App&;

  public:
    static constexpr auto MAINWINDOW_DEFAULT_TITLE = "JEngine-Reformed Application";
    static constexpr auto DEFAULT_CLEAR_COLOR = RGBA{255u, 0u, 255u, 255u};

    // cppcheck-suppress unusedFunction
    inline void ProcessEvent(IEvent& event) override
    {
        LogEvent(event);

        if (event.Category() == IEvent::EventCategory::KEYBOARD || event.Category() == IEvent::EventCategory::MOUSE) {
            m_InputController.InputController::ProcessEvent(event);
            return;
        }

        EventDispatcher dispatcher{event};
        dispatcher.Dispatch<QuitEvent>(
            [this]([[maybe_unused]] const QuitEvent& evnt)
            {
                m_Running = false;
                return true;
            });
    }

    inline void ProcessEvents()
    {
        ASSERT(m_Initialized);

        m_InputController.NewFrame();
        while (EnginePlatform().PollEvents(*this)) {
            ++m_EventsProcessed;
        }
    }

    inline void Loop(std::int64_t loopCount = -1)
    {
        ASSERT(m_Initialized);

        m_Running = true;
        while (m_LoopCount != loopCount && m_Running) {
            ProcessEvents();

            m_Renderer.ProcessCommandQueue();

            m_MainWindow->GraphicsContext().SwapBuffers();

            ++m_LoopCount;
        }

        // Flush the last processed event
        const UnknownEvent DUMMY;
        LogEvent(DUMMY);
    }

    inline auto MainWindow() -> IWindow& { return *m_MainWindow; }
    inline auto Renderer() -> JE::Renderer& { return m_Renderer; }
    inline auto InputController() -> JE::InputController& { return m_InputController; }

    inline auto LoopCount() const -> std::int64_t { return m_LoopCount; }
    inline auto Running() const -> bool { return m_Running; }
    inline auto EventsProcessed() const -> std::uint64_t { return m_EventsProcessed; }

    inline auto Initialized() const -> bool { return m_Initialized; }

  private:
    App()
    {
        if (!EnginePlatform().Initialize()) {
            EngineLogger()->error("Failed to create application - EnginePlatform failed to initialize");
            return;
        }

        m_MainWindow = CreateWindow(MAINWINDOW_DEFAULT_TITLE);
        if (!m_MainWindow->Created()) {
            EngineLogger()->error("Failed to create application - MainWindow could not be created");
            return;
        }

        ImpulseAudio::TestStuff();

        m_Initialized = true;
    }

    static inline void LogEvent(const IEvent& event)
    {
        static IEvent::EventType sLastEventType = IEvent::EventType::UNKNOWN;
        static std::uint32_t sEventCounter = 0;

        if (event.Type() == sLastEventType) {
            sEventCounter++;
            sLastEventType = event.Type();
            return;
        }

        if (sEventCounter > 1) {
            EngineLogger()->trace("Processing {} events of class - {} | type - {}",
                                  sEventCounter,
                                  ToString(EventTypeToCategory(sLastEventType)),
                                  ToString(sLastEventType));
        } else {
            EngineLogger()->trace("Processing event of class - {} | type - {}",
                                  ToString(EventTypeToCategory(sLastEventType)),
                                  ToString(sLastEventType));
        }

        sEventCounter = 1;
        sLastEventType = event.Type();
    }

    IWindow* m_MainWindow = nullptr;
    JE::Renderer m_Renderer;
    JE::InputController m_InputController;

    std::int64_t m_LoopCount = 0;
    bool m_Running = false;
    std::uint64_t m_EventsProcessed = 0;

    bool m_Initialized = false;
};

inline auto Application() -> App&
{
    static App sApplication;
    return sApplication;
}

inline auto Input() -> InputController& { return Application().InputController(); }

}  // namespace JE