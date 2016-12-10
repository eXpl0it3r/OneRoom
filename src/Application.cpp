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
    m_context.textures.acquire("test", thor::Resources::fromFile<sf::Texture>("test.png"));
    m_context.textures.acquire("player", thor::Resources::fromFile<sf::Texture>("entities/player.png"));
}

void Application::map_actions()
{
    m_context.actionmap["close"] = thor::Action(sf::Event::Closed);

    // TODO: Make mapping configurable.
    m_context.actionmap["left"] = thor::Action(sf::Keyboard::Left);
    m_context.actionmap["left_released"] = thor::Action(sf::Keyboard::Left, thor::Action::ReleaseOnce);
    m_context.actionmap["right"] = thor::Action(sf::Keyboard::Right);
    m_context.actionmap["right_released"] = thor::Action(sf::Keyboard::Right, thor::Action::ReleaseOnce);
    m_context.actionmap["up"] = thor::Action(sf::Keyboard::Up);
    m_context.actionmap["up_released"] = thor::Action(sf::Keyboard::Up, thor::Action::ReleaseOnce);
    m_context.actionmap["down"] = thor::Action(sf::Keyboard::Down);
    m_context.actionmap["down_released"] = thor::Action(sf::Keyboard::Down, thor::Action::ReleaseOnce);
}
