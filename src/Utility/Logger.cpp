#include "Utility/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime>

#ifdef _WIN32
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

std::string Logger::get_current_time()
{
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str = std::ctime(&current_time);
    if (!time_str.empty() && time_str.back() == '\n')
    {
        time_str.pop_back();    //by default string end with '\n' which I am removing here
    }

    return time_str;
}

void Logger::write_to_console(Logger::MessageType type, std::stringstream& log_ss, std::string& time_str)
{
    std::string dump;

    std::getline(log_ss, dump, '[');
    time_str = dump;
    std::cout << dump;
    std::getline(log_ss, dump);

    dump = "[" + dump;

    switch (type)
    {
        case MessageType::Info:
            set_console_output_color(ConsoleColor::white);
            std::cout << dump;
            break;
        case MessageType::Warning:
            set_console_output_color(ConsoleColor::yellow);
            std::cout << dump;
            break;
        case MessageType::Error:
            set_console_output_color(ConsoleColor::red);
            std::cout << dump;
            break;
        case MessageType::Fatal:
            set_console_output_color(ConsoleColor::red);
            std::cout << dump;
            break;
    }
    set_console_output_color(ConsoleColor::white);
    std::cout << '\n';

    while (std::getline(log_ss, dump))
    {
        std::cout << dump << '\n';
    }

    std::cout << std::flush;
}

void Logger::write_to_file(const std::string& path, std::stringstream& log_ss, const std::string& time_str)
{
    std::fstream file(path, std::ios::app);
    if(!file.is_open() && can_open)
    {
        can_open = false;
        set_console_output_color(ConsoleColor::yellow);
        std::cout << time_str << " [WARNING] ";
        set_console_output_color(ConsoleColor::white);
        std::cout << "Could not open \"" << path << "\" file\n";
        return;
    }

    can_open = true;    //in case program suddenly regains th ability to open
    file << log_ss.str();
}

std::string Logger::prep_message(std::string msg)
{
    int pos_to__delete = msg.find("] ");
    if(pos_to__delete != std::string::npos)
    {
        msg.erase(0, pos_to__delete);        
    }

    return msg;
}