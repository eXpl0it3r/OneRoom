#include "GameState.hpp"

GameState::GameState(SharedContext& context)
: State{context}
, m_tilemap{m_context, "room.json"}
{
    m_player = std::make_unique<PlayerEntity>(m_context.textures["player"]);
    m_entities.emplace("player", *m_player);
    m_player->connect(m_callbacks);
    m_player->setPosition(m_tilemap.spawn());
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

    for(auto& entity : m_entities)
    {
        entity.second.update(dt);
    }

    auto rect = m_tilemap.check_collision(m_player->getGlobalBounds());
    m_player->setPosition(rect.left, rect.top);
}

void GameState::render()
{
    m_context.window.clear(sf::Color{0x465060FF});
    m_context.window.draw(m_tilemap);

    // TODO: Draw order.
    for(auto& entity : m_entities)
    {
        m_context.window.draw(entity.second);
    }

    m_context.window.display();
}

