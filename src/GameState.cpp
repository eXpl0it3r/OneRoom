#include "GameState.hpp"

#include "Action.hpp"
#include "json.hpp"
#include <cmath>
#include <fstream>

GameState::GameState(SharedContext& context)
: State{context}
, m_action{"", context.fonts["font"], 16}
, m_info{"", context.fonts["font"], 16}
, m_lock{10, 10, 10}
, m_lock_digit0{" ", context.fonts["font"], 28}
, m_lock_digit1{" ", context.fonts["font"], 28}
, m_lock_digit2{" ", context.fonts["font"], 28}
, m_pin{10, 10, 10, 10}
, m_pin_digit0{" ", context.fonts["font"], 25}
, m_pin_digit1{" ", context.fonts["font"], 25}
, m_pin_digit2{" ", context.fonts["font"], 25}
, m_pin_digit3{" ", context.fonts["font"], 25}
, m_password_text{"", context.fonts["font"], 30}
, m_key{context.textures["key"]}
, m_combination{context.textures["combination"]}
, m_login{context.textures["login"]}
, m_briefcase{context.textures["briefcase-comb"]}
{
    load_rooms("res/screens.json");

    m_callbacks.connect("text", std::bind(&GameState::text_entered, this, std::placeholders::_1));

    m_action.setPosition({std::round((m_context.window.getSize().x / 2.f) + 0.5f), 300.f});
    m_info.setPosition({std::round((m_context.window.getSize().x / 2.f) + 0.5f), 320.f});
    m_info.setFillColor(sf::Color{254, 200, 200});

    m_lock_digit0.setPosition({172.f, 129.f});
    m_lock_digit0.setFillColor(sf::Color::Black);
    m_lock_digit1.setPosition({206.f, 129.f});
    m_lock_digit1.setFillColor(sf::Color::Black);
    m_lock_digit2.setPosition({240.f, 129.f});
    m_lock_digit2.setFillColor(sf::Color::Black);

    m_pin_digit0.setPosition({185.f, 70.f});
    m_pin_digit0.setFillColor(sf::Color::White);
    m_pin_digit1.setPosition({208.f, 70.f});
    m_pin_digit1.setFillColor(sf::Color::White);
    m_pin_digit2.setPosition({231.f, 70.f});
    m_pin_digit2.setFillColor(sf::Color::White);
    m_pin_digit3.setPosition({256.f, 70.f});
    m_pin_digit3.setFillColor(sf::Color::White);

    m_password_text.setPosition({198.f, 180.f});
    m_password_text.setFillColor(sf::Color::White);
    m_password_text.setOutlineThickness(1.f);
    m_password_text.setOutlineColor(sf::Color::Black);

    m_key.setPosition({5.f, 347.f});
    m_combination.setPosition({58.f, 347.f});
    m_login.setPosition({111.f, 347.f});
    m_briefcase.setPosition({164.f, 347.f});
}

void GameState::process_events()
{
    auto& map = m_context.actionmap;

    map.update(m_context.window);
    map.invokeCallbacks(m_callbacks, &m_context.window);

    if(map.isActive("close"))
    {
        m_context.window.close();
    }
    if(map.isActive("click"))
    {
        m_context.current_room->on_click(sf::Mouse::getPosition(m_context.window));
    }
    if(map.isActive("hover"))
    {
        m_context.current_room->on_hover(sf::Mouse::getPosition(m_context.window));
    }

    if(m_context.current_room->tag() == "lock")
    {
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
    else if(m_context.current_room->tag() == "phone-locked")
    {
        if(map.isActive("0"))
            add_pin(0);
        else if(map.isActive("1"))
            add_pin(1);
        else if(map.isActive("2"))
            add_pin(2);
        else if(map.isActive("3"))
            add_pin(3);
        else if(map.isActive("4"))
            add_pin(4);
        else if(map.isActive("5"))
            add_pin(5);
        else if(map.isActive("6"))
            add_pin(6);
        else if(map.isActive("7"))
            add_pin(7);
        else if(map.isActive("8"))
            add_pin(8);
        else if(map.isActive("9"))
            add_pin(9);
    }
}

void GameState::update(const sf::Time dt)
{
    if(m_next_screen != nullptr)
    {
        m_context.current_room = m_next_screen;
        m_next_screen = nullptr;
    }

    process_events();

    if(m_context.win)
    {
        // Let the user keep playing.
        if(m_win_timeout.getElapsedTime().asSeconds() > 5.f)
        {
            m_context.track_one.play();
            m_context.win = false;
        }
    }

    if(check_combination())
    {
        play_effect("lock");
        m_next_screen = m_screens["briefcase"];
        reset_combination();
    }
    if(check_pin())
    {
        play_effect("phone");
        m_next_screen = m_screens["phone-unlocked"];
        reset_pin();
    }
    else if(check_password())
    {
        play_effect("computer");
        m_next_screen = m_screens["computer-unlocked"];
        m_password = "";
    }

    if(m_context.current_room != nullptr)
    {
        change_action(m_context.current_room->hovering());
        change_screen(m_context.current_room->action());
    }
}

void GameState::render()
{
    m_context.window.clear(sf::Color{0x1a1d22ff});

    if(m_context.current_room != nullptr)
        m_context.window.draw(*m_context.current_room);

    if(m_context.current_room->tag() == "lock")
    {
        m_lock_digit0.setString(m_lock[0] == 10 ? " " : std::to_string(m_lock[0]));
        m_lock_digit0.setOrigin({std::round((m_lock_digit0.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_lock_digit1.setString(m_lock[1] == 10 ? " " : std::to_string(m_lock[1]));
        m_lock_digit1.setOrigin({std::round((m_lock_digit1.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_lock_digit2.setString(m_lock[2] == 10 ? " " : std::to_string(m_lock[2]));
        m_lock_digit2.setOrigin({std::round((m_lock_digit2.getGlobalBounds().width / 2.f) + 0.5f), 0.f});

        m_context.window.draw(m_lock_digit0);
        m_context.window.draw(m_lock_digit1);
        m_context.window.draw(m_lock_digit2);
    }
    else if(m_context.current_room->tag() == "phone-locked")
    {
        m_pin_digit0.setString(m_pin[0] == 10 ? " " : std::to_string(m_pin[0]));
        m_pin_digit0.setOrigin({std::round((m_pin_digit0.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_pin_digit1.setString(m_pin[1] == 10 ? " " : std::to_string(m_pin[1]));
        m_pin_digit1.setOrigin({std::round((m_pin_digit1.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_pin_digit2.setString(m_pin[2] == 10 ? " " : std::to_string(m_pin[2]));
        m_pin_digit2.setOrigin({std::round((m_pin_digit2.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_pin_digit3.setString(m_pin[3] == 10 ? " " : std::to_string(m_pin[3]));
        m_pin_digit3.setOrigin({std::round((m_pin_digit3.getGlobalBounds().width / 2.f) + 0.5f), 0.f});

        m_context.window.draw(m_pin_digit0);
        m_context.window.draw(m_pin_digit1);
        m_context.window.draw(m_pin_digit2);
        m_context.window.draw(m_pin_digit3);
    }
    else if(m_context.current_room->tag() == "computer-locked")
    {
        m_password_text.setString(m_password);
        m_password_text.setOrigin({std::round((m_password_text.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
        m_context.window.draw(m_password_text);
    }

    if(m_context.key)
        m_context.window.draw(m_key);

    if(m_context.combination)
        m_context.window.draw(m_combination);

    if(m_context.login)
        m_context.window.draw(m_login);

    if(m_context.briefcase)
        m_context.window.draw(m_briefcase);

    m_context.window.draw(m_action);

    if(m_info_clock.getElapsedTime() <= sf::seconds(5.f))
        m_context.window.draw(m_info);

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

        m_screens.emplace(tag, std::make_shared<Screen>(tag, screen->at("name"), m_context.textures[tag], std::move(actions)));
    }

    m_context.current_room = m_screens[start_screen];
}

void GameState::change_action(const std::string& action)
{
    std::string action_text = action;

    if(m_screens[action] != nullptr)
    {
        action_text = m_screens[action]->name();
    }

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
        if(action == "Pick up the key")
        {
            m_context.key = true;
            play_effect("key");
            change_info("You found a key!");
        }
        else if(action == "Pick up the note")
        {
            m_context.combination = true;
            play_effect("pin");
            change_info("You found some sort of a PIN!");
        }
        else if(action == "Remember the password")
        {
            m_context.login = true;
            play_effect("login");
            change_info("You found some sort of password!");
        }
        else if(action == "Remember the combination")
        {
            m_context.briefcase = true;
            play_effect("combination");
            change_info("You found some sort of combination!");
        }
        else if(action == "Leave the room")
        {
            if(!m_context.key)
            {
                play_effect("locked");
                change_info("The door is locked!");
            }
            else
            {
                m_context.track_one.stop();
                m_context.win = true;
                m_win_timeout.restart();
                play_effect("win");
                change_info("YOU WIN!");
            }
        }
    }
}

void GameState::change_info(const std::string& info)
{
    m_info.setString(info);
    m_info.setOrigin({std::round((m_info.getGlobalBounds().width / 2.f) + 0.5f), 0.f});
    m_info_clock.restart();
}

void GameState::play_effect(const std::string& effect)
{
    m_effect.setBuffer(m_context.sounds[effect]);
    m_effect.play();
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

void GameState::add_pin(int digit)
{
    // Reset combination lock.
    if(m_pin[3] != 10)
    {
        reset_pin();
    }

    for(auto i = 0; i < 4; ++i)
    {
        if(m_pin[i] == 10)
        {
            m_pin[i] = digit;
            break;
        }
    }
}

bool GameState::check_pin()
{
    bool check = true;

    for(auto i = 0; i < 4; ++i)
        check &= (m_context.pin_combination[i] == m_pin[i]);

    return check;
}

void GameState::reset_pin()
{
    m_pin = {10, 10, 10, 10};
}

void GameState::text_entered(thor::ActionContext<std::string> context)
{
    if(m_context.current_room->tag() == "computer-locked")
    {
        if(context.event->text.unicode == '\b')
        {
            if(m_password.size() > 0)
                m_password.pop_back();
        }
        else if(m_password.size() < 10)
        {
            m_password += context.event->text.unicode;
        }
    }
}

bool GameState::check_password()
{
    return m_password == m_context.password;
}
