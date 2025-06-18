#include "Nodes.hpp"

int main()
{
    try
    {
        Application::instance().run();  //setup and run
    }
    catch(std::exception& e)
    {
        std::string msg(e.what());
        Logger::log(Logger::MessageType::Error, Logger::prep_message(msg));
    }

    return 0;
}