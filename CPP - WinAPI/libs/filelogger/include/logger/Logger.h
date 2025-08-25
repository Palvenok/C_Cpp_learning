#pragma once
#include "logger/FileLogger.h"

#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>

namespace logger {

// Статический фасад для глобального доступа к единому логгеру.
// Автоинициализация происходит в Logger.cpp (формируется имя файла с датой/временем запуска).
class Logger {
public:
    static void init(const std::string& file_path,
                     bool append = true,
                     Level level = Level::info,
                     bool auto_flush = false,
                     bool allow_reinit = true);

    static void shutdown();
    static bool initialized();

    static void set_level(Level lvl);
    static Level level();

    static void log(Level lvl, std::string_view msg);

    template <typename... Args>
    static void log(Level lvl, Args&&... args) {
        instance().log(lvl, std::forward<Args>(args)...);
    }

    template <typename... Args> static void trace(Args&&... a){ instance().trace(std::forward<Args>(a)...); }
    template <typename... Args> static void debug(Args&&... a){ instance().debug(std::forward<Args>(a)...); }
    template <typename... Args> static void info (Args&&... a){ instance().info (std::forward<Args>(a)...); }
    template <typename... Args> static void warn (Args&&... a){ instance().warn (std::forward<Args>(a)...); }
    template <typename... Args> static void error(Args&&... a){ instance().error(std::forward<Args>(a)...); }
    template <typename... Args> static void critical(Args&&... a){ instance().critical(std::forward<Args>(a)...); }

    static void flush();

private:
    static FileLogger& instance();
    static std::shared_ptr<FileLogger>& storage_();
    static std::mutex& mutex_();
};

#define LOG_TRACE(...)    ::logger::Logger::trace(__VA_ARGS__)
#define LOG_DEBUG(...)    ::logger::Logger::debug(__VA_ARGS__)
#define LOG_INFO(...)     ::logger::Logger::info (__VA_ARGS__)
#define LOG_WARN(...)     ::logger::Logger::warn (__VA_ARGS__)
#define LOG_ERROR(...)    ::logger::Logger::error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::logger::Logger::critical(__VA_ARGS__)

} // namespace logger
