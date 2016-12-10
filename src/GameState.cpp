#include "GameState.hpp"

GameState::GameState(SharedContext& context)
: State{context}
{
    load_resources();

    m_ent.texture(m_context.textures["test"]);
}

void GameState::process_events()
{
    sf::Event event;

    while(m_context.window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            m_context.window.close();
    }
}

void GameState::update(const sf::Time dt)
{
    process_events();

    m_ent.update(dt);
}

void GameState::render()
{
    m_context.window.clear();
    m_context.window.draw(m_ent);
    m_context.window.display();
}

void GameState::load_resources()
{
    m_context.textures.acquire("test", thor::Resources::fromFile<sf::Texture>("test.png"));
}
