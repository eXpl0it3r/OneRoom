#include "Application.hpp"

#include "GameState.hpp"

#include <memory>

Application::Application()
{
    setup_window();
    load_resources();
    map_actions();
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

void Application::setup_window()
{
    m_context.window.setVerticalSyncEnabled(true);
    m_context.window.clear();
    m_context.window.display();
}

void Application::load_resources()
{
    m_context.fonts.acquire("font", thor::Resources::fromFile<sf::Font>("battlenet.ttf"));
}

void Application::map_actions()
{
    m_context.actionmap["close"] = thor::Action(sf::Event::Closed);
    m_context.actionmap["click"] = thor::Action(sf::Mouse::Left, thor::Action::ReleaseOnce);
    m_context.actionmap["hover"] = thor::Action(sf::Event::MouseMoved);

    m_context.actionmap["0"] = thor::Action(sf::Keyboard::Num0, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad0, thor::Action::ReleaseOnce);
    m_context.actionmap["1"] = thor::Action(sf::Keyboard::Num1, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad1, thor::Action::ReleaseOnce);
    m_context.actionmap["2"] = thor::Action(sf::Keyboard::Num2, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad2, thor::Action::ReleaseOnce);
    m_context.actionmap["3"] = thor::Action(sf::Keyboard::Num3, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad3, thor::Action::ReleaseOnce);
    m_context.actionmap["4"] = thor::Action(sf::Keyboard::Num4, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad4, thor::Action::ReleaseOnce);
    m_context.actionmap["5"] = thor::Action(sf::Keyboard::Num5, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad5, thor::Action::ReleaseOnce);
    m_context.actionmap["6"] = thor::Action(sf::Keyboard::Num6, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad6, thor::Action::ReleaseOnce);
    m_context.actionmap["7"] = thor::Action(sf::Keyboard::Num7, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad7, thor::Action::ReleaseOnce);
    m_context.actionmap["8"] = thor::Action(sf::Keyboard::Num8, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad8, thor::Action::ReleaseOnce);
    m_context.actionmap["9"] = thor::Action(sf::Keyboard::Num9, thor::Action::ReleaseOnce)
                               || thor::Action(sf::Keyboard::Numpad9, thor::Action::ReleaseOnce);
}
