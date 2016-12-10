#pragma once

#include "State.hpp"
#include "ExplosionEntity.hpp"

#include <Thor/Resources.hpp>

#include <string>

class GameState : public State
{
public:
    GameState(SharedContext& context);

private:
    void process_events() final;
    void update(const sf::Time dt) final;
    void render() final;

    void load_resources();

private:
    ExplosionEntity m_ent;
};