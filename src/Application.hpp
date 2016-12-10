#pragma once

#include "SharedContext.hpp"

class Application
{
public:
    Application();

    void run();

private:
     SharedContext m_context;
};
