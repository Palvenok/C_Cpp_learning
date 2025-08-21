#include "logger/Logger.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>

#if defined(_WIN32)
  #include <process.h>
  static inline int get_pid_() { return _getpid(); }
#else
  #include <unistd.h>
  static inline int get_pid_() { return static_cast<int>(::getpid()); }
#endif

namespace logger {

// Локальная потокобезопасная версия localtime
static std::tm safe_localtime_(std::time_t t) noexcept {
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    return tm;
}

// Отформатированная дата/время запуска (инициализируется один раз)
static const std::string& startup_timestamp_() {
    static const std::string ts = []{
        using namespace std::chrono;
        const auto now = system_clock::now();
        const std::time_t t = system_clock::to_time_t(now);
        const std::tm tm = safe_localtime_(t);
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%04d_%02d_%02d-%02d_%02d_%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
        return std::string(buf);
    }();
    return ts;
}

// Базовый каталог + имя файла по умолчанию
static const std::string& default_log_path_() {
    static const std::string path = []{
        std::error_code ec;
        std::filesystem::create_directories("logs", ec); // тихо пытаемся создать каталог
        // Пример: logs/app-2025_08_21-12_58_30-12345.log
        std::string name = "logs/app-";
        name += startup_timestamp_();
        name += "-";
        name += std::to_string(get_pid_());
        name += ".log";
        return name;
    }();
    return path;
}

// === Реализация методов Logger ===

void Logger::init(const std::string& file_path,
                  bool append,
                  Level level,
                  bool auto_flush,
                  bool allow_reinit)
{
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();

    if (ptr && !allow_reinit) {
        return;
    }
    ptr = std::make_shared<FileLogger>(file_path, append, level, auto_flush);
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();
    if (ptr) {
        ptr->flush();
        ptr.reset();
    }
}

bool Logger::initialized() {
    std::lock_guard<std::mutex> lock(mutex_());
    return static_cast<bool>(storage_());
}

void Logger::set_level(Level lvl) {
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();
    if (!ptr) {
        ptr = std::make_shared<FileLogger>(default_log_path_(), true, lvl, false);
    } else {
        ptr->set_level(lvl);
    }
}

Level Logger::level() {
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();
    if (!ptr) {
        ptr = std::make_shared<FileLogger>(default_log_path_(), true, Level::info, false);
    }
    return ptr->level();
}

void Logger::log(Level lvl, std::string_view msg) {
    instance().log(lvl, msg);
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();
    if (ptr) ptr->flush();
}

FileLogger& Logger::instance() {
    // Если не инициализирован — создадим с именем по умолчанию (дата/время + pid)
    std::lock_guard<std::mutex> lock(mutex_());
    auto& ptr = storage_();
    if (!ptr) {
        ptr = std::make_shared<FileLogger>(default_log_path_(), /*append*/true,
                                           /*level*/Level::info, /*auto_flush*/false);
    }
    return *ptr;
}

std::shared_ptr<FileLogger>& Logger::storage_() {
    static std::shared_ptr<FileLogger> s_logger;
    return s_logger;
}

std::mutex& Logger::mutex_() {
    static std::mutex s_mutex;
    return s_mutex;
}

// --- Автоинициализация на этапе статической инициализации TU ---

struct LoggerAutoInit {
    LoggerAutoInit() {
        // Можно задать свои дефолты здесь (уровень/auto_flush)
        Logger::init(default_log_path_(),
                     /*append*/true,
                     /*level*/Level::info,
                     /*auto_flush*/true,
                     /*allow_reinit*/true); // оставим возможность переинициализировать в main()
    }
};

// На GCC/Clang можно подсказать приоритет инициализации (чем меньше, тем раньше).
// Это НЕ стандарт C++, но иногда полезно.
//
// #if defined(__GNUC__) && !defined(_MSC_VER)
// __attribute__((init_priority(101)))
// #endif
static LoggerAutoInit s_logger_auto_init;

} // namespace logger
