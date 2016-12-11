#include "GameState.hpp"

#include "Action.hpp"
#include "json.hpp"
#include <cmath>
#include <fstream>

GameState::GameState(SharedContext& context)
: State{context}
, m_action{"", context.fonts["font"], 16}
{
    //m_screens.emplace("player", *m_player);
    //->connect(m_callbacks);
    //m_context.current
    load_rooms("screens.json");
    m_action.setPosition({std::round((m_context.window.getSize().x / 2.f) + 0.5f), 305.f});
}

void GameState::process_events()
{
    m_context.actionmap.update(m_context.window);
    m_context.actionmap.invokeCallbacks(m_callbacks, &m_context.window);

    if(m_context.actionmap.isActive("close"))
    {
        m_context.window.close();
    }
    if(m_context.actionmap.isActive("click"))
    {
        m_context.current_room->on_click(sf::Mouse::getPosition(m_context.window));
    }
    if(m_context.actionmap.isActive("hover"))
    {
        m_context.current_room->on_hover(sf::Mouse::getPosition(m_context.window));
    }
}

void GameState::update(const sf::Time dt)
{
    process_events();

    if(m_context.current_room != nullptr)
    {
        m_context.current_room->update(dt);
        change_action(m_context.current_room->hovering());
        change_screen(m_context.current_room->action());
    }
}

void GameState::render()
{
    m_context.window.clear(sf::Color{0x1a1d22ff});

    if(m_context.current_room != nullptr)
        m_context.window.draw(*m_context.current_room);

    m_context.window.draw(m_action);

    m_context.window.display();
}

void GameState::load_rooms(const std::string& filename)
{
    nlohmann::json screens;

    std::fstream file(filename, std::ios::in | std::ios::binary);

    if(!file.is_open())
        throw(std::runtime_error{"Failed to open screens file \"" + filename + "\"."});

    try
    {
        file >> screens;
    }
    catch(std::exception& e)
    {
        throw(std::runtime_error{"The animation file \"" + filename + "\" contains invalid JSON data."});
    }

    // Get general options.
    std::string start_screen = screens["start"];

    for(auto screen = screens["screens"].begin(); screen != screens["screens"].end(); ++screen)
    {
        std::string tag = screen.key();

        m_context.textures.acquire(tag, thor::Resources::fromFile<sf::Texture>(screen->at("texture")));

        std::vector<Action> actions;

        for(auto action = screen->at("actions").begin(); action != screen->at("actions").end(); ++action)
        {
            actions.emplace_back(action.key(),
                              sf::IntRect{action.value()[0], action.value()[1], action.value()[2], action.value()[3]});
        }

        m_screens.emplace(tag, std::make_shared<Screen>(screen->at("name"), m_context.textures[tag], std::move(actions)));
    }

    m_context.current_room = m_screens[start_screen];
}

void GameState::change_action(const std::string& action)
{
    m_action.setString(action);
    // TODO: Proper placement.
    m_action.setOrigin({std::round((m_action.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
}

void GameState::change_screen(const std::string& action)
{
    auto screen = m_screens[action];

    if(screen != nullptr)
        m_context.current_room = screen;
}
