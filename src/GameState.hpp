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

private:
    std::map<std::string, std::shared_ptr<Screen>> m_screens;
    thor::ActionMap<std::string>::CallbackSystem m_callbacks;

    sf::Text m_action;
};
