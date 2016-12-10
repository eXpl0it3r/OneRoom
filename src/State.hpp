#pragma once

#include "SharedContext.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class State
{
public:
    State(SharedContext& context);
    virtual ~State() = default;

    // Non copyable
    State(State const&) = delete;
    State& operator=(State const&) = delete;

    std::unique_ptr<State> run();

protected:
    virtual void process_events() = 0;
    virtual void update(const sf::Time dt) = 0;
    virtual void render() = 0;

protected:
    SharedContext& m_context;

    std::unique_ptr<State> m_next;
};