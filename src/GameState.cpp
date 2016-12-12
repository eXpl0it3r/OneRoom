#include "GameState.hpp"

#include "Action.hpp"
#include "json.hpp"
#include <cmath>
#include <fstream>

GameState::GameState(SharedContext& context)
: State{context}
, m_action{"", context.fonts["font"], 16}
, m_lock{10, 10, 10}
, m_digit0{" ", context.fonts["font"], 28}
, m_digit1{" ", context.fonts["font"], 28}
, m_digit2{" ", context.fonts["font"], 28}
{
    load_rooms("screens.json");
    m_action.setPosition({std::round((m_context.window.getSize().x / 2.f) + 0.5f), 305.f});

    m_digit0.setPosition({172.f, 129.f});
    m_digit0.setFillColor(sf::Color::Black);
    m_digit1.setPosition({206.f, 129.f});
    m_digit1.setFillColor(sf::Color::Black);
    m_digit2.setPosition({240.f, 129.f});
    m_digit2.setFillColor(sf::Color::Black);
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
    if(m_context.current_room->name() == "Lock")
    {
        auto& map = m_context.actionmap;

        if(map.isActive("0"))
            add_combination(0);
        else if(map.isActive("1"))
            add_combination(1);
        else if(map.isActive("2"))
            add_combination(2);
        else if(map.isActive("3"))
            add_combination(3);
        else if(map.isActive("4"))
            add_combination(4);
        else if(map.isActive("5"))
            add_combination(5);
        else if(map.isActive("6"))
            add_combination(6);
        else if(map.isActive("7"))
            add_combination(7);
        else if(map.isActive("8"))
            add_combination(8);
        else if(map.isActive("9"))
            add_combination(9);
    }
}

void GameState::update(const sf::Time dt)
{
    if(m_next != nullptr)
    {
        m_context.current_room = m_next;
        m_next = nullptr;
    }

    process_events();

    if(check_combination())
    {
        // Change screen if the combination is correct.
        m_next = m_screens["suitcase"];
        reset_combination();
    }
    else if(m_context.current_room != nullptr)
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

    if(m_context.current_room->name() == "Lock")
    {
        m_digit0.setString(m_lock[0] == 10 ? " " : std::to_string(m_lock[0]));
        m_digit0.setOrigin({std::round((m_digit0.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_digit1.setString(m_lock[1] == 10 ? " " : std::to_string(m_lock[1]));
        m_digit1.setOrigin({std::round((m_digit1.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_digit2.setString(m_lock[2] == 10 ? " " : std::to_string(m_lock[2]));
        m_digit2.setOrigin({std::round((m_digit2.getGlobalBounds().width / 2.f) + 0.5f), 0.f});

        m_context.window.draw(m_digit0);
        m_context.window.draw(m_digit1);
        m_context.window.draw(m_digit2);
    }

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
    std::string action_text = action;

    if(m_screens[action] != nullptr)
        action_text = m_screens[action]->name();

    m_action.setString(action_text);
    m_action.setOrigin({std::round((m_action.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
}

void GameState::change_screen(const std::string& action)
{
    auto screen = m_screens[action];

    if(screen != nullptr)
    {
        m_context.current_room = screen;
    }
    else
    {
        action;
    }
}

void GameState::add_combination(int digit)
{
    // Reset combination lock.
    if(m_lock[2] != 10)
    {
        reset_combination();
    }

    for(auto i = 0; i < 3; ++i)
    {
        if(m_lock[i] == 10)
        {
            m_lock[i] = digit;
            break;
        }
    }
}

bool GameState::check_combination()
{
    bool check = true;

    for(auto i = 0; i < 3; ++i)
        check &= (m_context.lock_combination[i] == m_lock[i]);

    return check;
}

void GameState::reset_combination()
{
    m_lock = {10, 10, 10};
}
