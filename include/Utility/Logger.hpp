#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef DEBUG
#include <filesystem>
#endif

class Logger
{
    private:
        enum ConsoleColor
    {
        white = 0xF,
        yellow = 0xE,
        red = 0x4
    };

    static void set_console_output_color(ConsoleColor color)
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
    
    public:
    enum MessageType : unsigned char
    {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    template<typename ... Args>
    static void log(Logger::MessageType type, Args&&... args)
    {
        try
        {
            std::stringstream log_ss;

            std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string time_str = std::ctime(&current_time);
            if (!time_str.empty() && time_str.back() == '\n')
            {
                time_str.pop_back();    //by default string end with '\n' which I am removing here
            }

            std::cout << time_str << ' ';
            log_ss << time_str << ' ';

            switch (type)
            {
                case MessageType::Info:
                    set_console_output_color(ConsoleColor::white);
                    log_ss << "[INFO] ";
                    std::cout << "[INFO] ";
                    break;
                case MessageType::Warning:
                    set_console_output_color(ConsoleColor::yellow);
                    log_ss << "[WARNING] ";
                    std::cout << "[WARNING] ";
                    break;
                case MessageType::Error:
                    set_console_output_color(ConsoleColor::red);
                    log_ss << "[ERROR] ";
                    std::cout << "[ERROR] ";
                    break;
            }
            set_console_output_color(ConsoleColor::white);

            (std::cout << ... << args) << '\n';

            (log_ss << ... << args) << '\n';

            #ifdef DEBUG
            std::filesystem::create_directories("build/runtime_files");
            const std::string path = "build/runtime_files/Logs.txt";
            #else
            const std::string path = "Logs.txt";
            #endif

            std::fstream file(path, std::ios::app);
            static bool can_open = true;

            if(!file.is_open() && can_open)
            {
                can_open = false;
                set_console_output_color(ConsoleColor::yellow);
                std::cout << time_str << " [WARNING] ";
                set_console_output_color(ConsoleColor::white);
                std::cout << "Could not open \"" << path << "\" file\n";
            }

            can_open = true;    //in case program suddenly regains th ability to open
            file << log_ss.str();
        }
        catch(...)
        {
            printf("[FATAL] Log creation failed.\n");
        }
    }

    template<typename ... Args>
    static std::string format(Logger::MessageType type, Args&&... args) noexcept
    {
        try
        {
            std::stringstream log_ss;

            switch (type)
            {
                case MessageType::Info:
                    set_console_output_color(ConsoleColor::white);
                    log_ss << "[INFO] ";
                    break;
                case MessageType::Warning:
                    set_console_output_color(ConsoleColor::yellow);
                    log_ss << "[WARNING] ";
                    break;
                case MessageType::Error:
                    set_console_output_color(ConsoleColor::red);
                    log_ss << "[ERROR] ";
                    break;
            }

            (log_ss << ... << args);
            return log_ss.str();
        }
        catch(...)
        {
            return "[ERROR] Unexpected error, log formating failed.";
        }
    }

    static std::string& prep_message(std::string& msg)
    {
        int pos_to__delete = msg.find("[ERROR] ");
        if(pos_to__delete != std::string::npos)
        {
            msg.erase(pos_to__delete, std::string("[ERROR] ").size());
        }

        return msg;
    }
};