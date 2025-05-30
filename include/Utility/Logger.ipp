template <typename T, typename = void>
struct is_streamable : std::false_type {};

template <typename T>
struct is_streamable<T, std::void_t<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> : std::true_type {};

template <typename... Args>
std::string Logger::format(Logger::MessageType type, Args &&...args) noexcept
{
    static_assert((is_streamable<Args>::value && ...), "Logger::log not all arguments are streamable");

    try
    {
        std::stringstream log_ss, raw;
        raw << get_current_time() << ' ';

        switch (type)
        {
        case MessageType::Info:
            raw << std::left << std::setw(10) << "[INFO]";
            break;
        case MessageType::Warning:
            raw << std::left << std::setw(10) << "[WARNING]";
            break;
        case MessageType::Error:
            raw << std::left << std::setw(10) << "[ERROR]";
            break;
        case MessageType::Fatal:
            raw << std::left << std::setw(10) << "[FATAL]";
            break;
        }
        raw << '\n';
        int preaumbule_size = raw.str().size();

        (raw << ... << args);

        std::string dump;
        while (std::getline(raw, dump))
        {
            log_ss << dump << '\n';
        }

        log_ss << std::string(preaumbule_size, '*') << '\n';
        return log_ss.str();
    }
    catch (...)
    {
        try
        {
            return Logger::format(MessageType::Fatal, "Log creation failed.");
        }
        catch (...)
        {
            return "[FATAL] Unexpected error, log formating failed.";
        }
    }
}

template<typename ... Args>
void Logger::log(Logger::MessageType type, Args&&... args)
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