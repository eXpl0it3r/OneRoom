#include "State.hpp"

State::State(SharedContext& context)
: m_context{context}
{

}

std::unique_ptr<State> State::run()
{
    sf::Clock clock;
    sf::Time last_frametime;
    sf::Time dt;

    while(m_next == nullptr && m_context.window.isOpen())
    {
        dt = clock.getElapsedTime() - last_frametime;
        last_frametime = clock.getElapsedTime();

        update(dt);
        render();
    }

    return std::move(m_next);
}