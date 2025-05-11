#include "Nodes.hpp"

/*
    NOTES

    A logging function could be useful
*/

int main()
{
    //any parameter magic goes here

    /******************************/
    try
    {
        Application::instance().run();  //setup and run
    }
    catch(std::exception& e)
    {
        /*
            This may or may not be temporary, as ideally
            application would at this moment try to display
            an error popup
        */
        std::string msg(e.what());
        Logger::log(Logger::MessageType::Error, Logger::prep_message(msg));
    }
    
    return 0;
}
