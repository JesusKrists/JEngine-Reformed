#pragma once

#include "Assert.hpp"
#include "Base.hpp"
#include "Events.hpp"
#include "Graphics/Renderer.hpp"
#include "Platform.hpp"
#include "Types.hpp"

namespace JE
{

class App final : public IEventProcessor
{
    friend auto Application() -> App&;

  public:
    static constexpr auto MAINWINDOW_DEFAULT_TITLE = "JEngine-Reformed Application";
    static constexpr auto DEFAULT_CLEAR_COLOR = ColorRGBA{255u, 0u, 255u, 255u};

    // cppcheck-suppress unusedFunction
    inline void ProcessEvent(IEvent& event) override
    {
        EngineLogger()->trace(
            "Processing event of class - {} | type - {}", ToString(event.Category()), ToString(event.Type()));

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

        while (EnginePlatform().PollEvents(*this)) {
            ++m_EventsProcessed;
        }
    }

    inline void Loop(std::int64_t loopCount = -1)
    {
        ASSERT(m_Initialized);

        m_TestMesh = CreateQuadMesh();

        m_Running = true;
        while (m_LoopCount != loopCount && m_Running) {
            ProcessEvents();

            m_Renderer.Begin(m_MainWindow, DEFAULT_CLEAR_COLOR);
            m_Renderer.DrawMesh(m_TestMesh);
            m_Renderer.End();

            m_Renderer.ProcessCommandQueue();

            m_MainWindow->GraphicsContext().SwapBuffers();

            ++m_LoopCount;
        }
    }

    inline auto MainWindow() -> IWindow& { return *m_MainWindow; }
    inline auto Renderer() -> JE::Renderer& { return m_Renderer; }

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

        m_Initialized = true;
    }

    IWindow* m_MainWindow = nullptr;
    JE::Renderer m_Renderer;

    Mesh m_TestMesh;

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

}  // namespace JE