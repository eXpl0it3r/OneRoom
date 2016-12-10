#pragma once

#include "SharedContext.hpp"

class Application
{
public:
    Application();

    void run();

private:
    void setup_window();
    void load_resources();
    void map_actions();

private:
     SharedContext m_context;
};
