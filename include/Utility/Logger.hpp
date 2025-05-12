#pragma once
#include <sstream>
#include <mutex>

#ifdef DEBUG
#include <filesystem>
#endif

class Logger
{
    public:
    enum MessageType : unsigned char
    {
        Info = 0,
        Warning = 1,
        Error = 2,
        Fatal = 3
    };

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
    static std::string get_current_time();
    static void write_to_file(const std::string& path, std::stringstream& log_ss, const std::string& time_str);
    static void write_to_console(Logger::MessageType type, std::stringstream& log_ss, std::string& time_str);

    public:
    template<typename ... Args>
    static void log(Logger::MessageType type, Args&&... args);

    template<typename ... Args>
    static std::string format(Logger::MessageType type, Args&&... args) noexcept;

    static std::string prep_message(std::string msg);
};

#include "Logger.ipp"