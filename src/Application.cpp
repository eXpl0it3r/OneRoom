#include "Application.hpp"

#include "GameState.hpp"

#include <memory>

Application::Application()
{
    m_context.window.setVerticalSyncEnabled(true);
}

void Application::run()
{
    // Simple state machine
    std::unique_ptr<State> state = std::make_unique<GameState>(m_context);

    while(state != nullptr)
    {
        state = state->run();
    }
}
