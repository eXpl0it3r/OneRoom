#pragma once

#include "State.hpp"
#include <Thor/Input/EventSystem.hpp>
#include <Thor/Resources.hpp>
#include <memory>
#include <string>

class GameState : public State
{
public:
    GameState(SharedContext& context);

private:
    void process_events() final;
    void update(const sf::Time dt) final;
    void render() final;

    void load_rooms(const std::string& filename);

    void change_screen(const std::string& screen);
    void change_action(const std::string& action);
    void change_info(const std::string& info);

    void play_effect(const std::string& effect);

    void add_combination(int i);
    bool check_combination();
    void reset_combination();

    void add_pin(int i);
    bool check_pin();
    void reset_pin();

    void text_entered(thor::ActionContext<std::string> context);
    bool check_password();

private:
    std::map<std::string, std::shared_ptr<Screen>> m_screens;
    thor::ActionMap<std::string>::CallbackSystem m_callbacks;
    std::shared_ptr<Screen> m_next_screen;

    sf::Text m_action;
    sf::Text m_info;
    sf::Clock m_info_clock;

    std::vector<int> m_lock;
    sf::Text m_lock_digit0;
    sf::Text m_lock_digit1;
    sf::Text m_lock_digit2;

    std::vector<int> m_pin;
    sf::Text m_pin_digit0;
    sf::Text m_pin_digit1;
    sf::Text m_pin_digit2;
    sf::Text m_pin_digit3;

    std::string m_password;
    sf::Text m_password_text;

    sf::Sprite m_key;
    sf::Sprite m_combination;
    sf::Sprite m_login;
    sf::Sprite m_briefcase;

    sf::Sound m_effect;
    sf::Clock m_win_timeout;
};
