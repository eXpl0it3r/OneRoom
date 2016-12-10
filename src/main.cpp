#include "Application.hpp"

#include <iostream>

int main()
{
    try
    {
        Application app;
        app.run();
    }
    catch(std::exception& e)
    {
        std::cerr << "One Room detected an error: " << e.what() << std::endl;
        std::cerr << "Due to this error One Room unfortunately needs to terminate." << std::endl;
    }
}