#pragma once

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include <sstream>

namespace logger {

enum class Level : int { trace = 0, debug, info, warn, error, critical, off };

class FileLogger {
public:
    explicit FileLogger(const std::string& file_path,
                        bool append = true,
                        Level level = Level::info,
                        bool auto_flush = false);

    void set_level(Level lvl) noexcept;
    Level level() const noexcept;

    // Публичная явная строковая версия (если хотите писать готовую строку)
    void log(Level lvl, std::string_view message);

    // Вариадик — строит строку и вызывает приватный writer без перегрузок
    template <typename... Args>
    void log(Level lvl, Args&&... args) {
        if (lvl < level()) return;
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        write_line_(lvl, oss.str());          // <— заметно ДРУГОЕ имя
    }

    // Шорткаты
    template <typename... Args> void trace(Args&&... a)    { log(Level::trace,    std::forward<Args>(a)...); }
    template <typename... Args> void debug(Args&&... a)    { log(Level::debug,    std::forward<Args>(a)...); }
    template <typename... Args> void info(Args&&... a)     { log(Level::info,     std::forward<Args>(a)...); }
    template <typename... Args> void warn(Args&&... a)     { log(Level::warn,     std::forward<Args>(a)...); }
    template <typename... Args> void error(Args&&... a)    { log(Level::error,    std::forward<Args>(a)...); }
    template <typename... Args> void critical(Args&&... a) { log(Level::critical, std::forward<Args>(a)...); }

    void flush();
    bool is_open() const noexcept;

private:
    std::string format_prefix_(Level lvl) const;
    static const char* to_string(Level lvl) noexcept;
    static std::tm safe_localtime_(std::time_t t) noexcept;

    // Единственная точка фактической записи
    void write_line_(Level lvl, std::string_view message);

private:
    std::atomic<Level> level_;
    bool auto_flush_;
    std::ofstream stream_;
    mutable std::mutex mutex_;
};

} // namespace logger
