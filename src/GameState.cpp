#include "GameState.hpp"

GameState::GameState(SharedContext& context)
: State{context}
{
    m_player = std::make_unique<PlayerEntity>(m_context.textures["player"]);
    m_player->connect(m_callbacks);
    m_entities.emplace("player", *m_player);
}

void GameState::process_events()
{
    m_context.actionmap.update(m_context.window);
    m_context.actionmap.invokeCallbacks(m_callbacks, &m_context.window);
}

void GameState::update(const sf::Time dt)
{
    process_events();

    if(m_context.actionmap.isActive("close"))
    {
        m_context.window.close();
    }

    //m_player->move(20.f, 30.f);

    for(auto& entity : m_entities)
    {
        entity.second.update(dt);
    }
}

void GameState::render()
{
    m_context.window.clear(sf::Color{0x465060FF});

    // TODO: Draw order.
    for(auto& entity : m_entities)
    {
        m_context.window.draw(entity.second);
    }

    m_context.window.display();
}

