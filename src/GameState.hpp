#pragma once

#include "State.hpp"
#include "PlayerEntity.hpp"
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

private:
    std::unique_ptr<PlayerEntity> m_player;
    std::map<std::string, Entity&> m_entities;
    thor::ActionMap<std::string>::CallbackSystem m_callbacks;
};
