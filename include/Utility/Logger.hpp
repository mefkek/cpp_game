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
    static void log(Logger::MessageType type, Args&&... args)
    {
        try
        {
            std::string time_str;
            std::stringstream log_ss(Logger::format(type, std::forward<Args>(args)...));
            std::lock_guard<std::mutex> lock(log_mutex);
            
            write_to_console(type, log_ss, time_str);

            #ifdef DEBUG
            std::filesystem::create_directories("build/runtime_files");
            const std::string path = "build/runtime_files/Logs.txt";
            #else
            const std::string path = "Logs.txt";
            #endif

            write_to_file(path, log_ss, time_str);
        }
        catch(...)
        {            
            try
            {
                std::stringstream log_ss(Logger::format(MessageType::Fatal, "Log creation failed."));
                std::string time_dump;
                std::lock_guard<std::mutex> lock(log_mutex);
                write_to_console(MessageType::Fatal, log_ss, time_dump);
            }
            catch(...)
            {
                printf("[FATAL] Log creation failed.\n");
            }
        }
    }

    template<typename ... Args>
    static std::string format(Logger::MessageType type, Args&&... args) noexcept
    {
        try
        {
            std::stringstream log_ss;
            log_ss << get_current_time() << ' ';

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
                case MessageType::Fatal:
                    log_ss << "[FATAL] ";
                    break;
            }

            (log_ss << ... << args);
            return log_ss.str();
        }
        catch(...)
        {
            try
            {
                return Logger::format(MessageType::Fatal, "Log creation failed.");
            }
            catch(...)
            {
                return "[FATAL] Unexpected error, log formating failed.";
            }
        }
    }

    static std::string prep_message(std::string msg);
};