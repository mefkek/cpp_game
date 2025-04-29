#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <mutex>
#include <ctime>

#ifdef DEBUG
#include <filesystem>
#endif

class Logger
{
    private:

    static bool can_open;
    static std::mutex log_mutex;

    enum ConsoleColor
    {
        white = 0xF,
        yellow = 0xE,
        red = 0x4
    };

    static void set_console_output_color(ConsoleColor color);
    
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
            std::stringstream log_ss(Logger::format(type, std::forward<Args>(args)...));
            std::lock_guard<std::mutex> lock(log_mutex);
            std::string dump, time_str;;

            std::getline(log_ss, dump, '[');
            time_str = dump;
            std::cout << dump;
            std::getline(log_ss, dump, ']');

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
            }
            set_console_output_color(ConsoleColor::white);

            std::getline(log_ss, dump);
            std::cout << dump << std::endl;

            #ifdef DEBUG
            std::filesystem::create_directories("build/runtime_files");
            const std::string path = "build/runtime_files/Logs.txt";
            #else
            const std::string path = "Logs.txt";
            #endif

            std::fstream file(path, std::ios::app);

            if(!file.is_open() && can_open)
            {
                can_open = false;
                set_console_output_color(ConsoleColor::yellow);
                std::cout << time_str << " [WARNING] ";
                set_console_output_color(ConsoleColor::white);
                std::cout << "Could not open \"" << path << "\" file\n";
            }

            can_open = true;    //in case program suddenly regains th ability to open
            file << log_ss.str() << '\n';
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
            std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string time_str = std::ctime(&current_time);
            if (!time_str.empty() && time_str.back() == '\n')
            {
                time_str.pop_back();    //by default string end with '\n' which I am removing here
            }

            std::stringstream log_ss;
            log_ss << time_str << ' ';

            switch (type)
            {
                case MessageType::Info:
                    log_ss << "[INFO] ";
                    break;
                case MessageType::Warning:
                    log_ss << "[WARNING] ";
                    break;
                case MessageType::Error:
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

    static std::string prep_message(std::string msg);
};