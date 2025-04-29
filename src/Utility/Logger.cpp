#include "Utility/Logger.hpp"

#ifdef WIN32
#include <windows.h>
#endif

bool Logger::can_open = true;
std::mutex Logger::log_mutex;

void Logger::set_console_output_color(ConsoleColor color)
{
    #if defined(_WIN32)

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

    #elif defined(__linux__) || defined(__APPLE__)

    if(color == ConsoleColor::white)
    {
        std::cout << "\033[0m";     //technically default, not necessarily white
    }
    else if(color == ConsoleColor::yellow)
    {
        std::cout << "\033[33m";
    }
    else if(color == ConsoleColor::red)
    {
        std::cout << "\033[31m";
    }

    #endif
}

std::string Logger::prep_message(std::string msg)
{
    int pos_to__delete = msg.find("[ERROR] ");
    if(pos_to__delete != std::string::npos)
    {
        msg.erase(pos_to__delete, std::string("[ERROR] ").size());        
    }

    return msg;
}