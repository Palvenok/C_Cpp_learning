#include "logger/FileLogger.h"

#include <chrono>
#include <iomanip>
#include <filesystem>
#include <system_error>

namespace logger {

using namespace std::chrono;

static std::string two_digits_(int x) {
    char buf[3];
    buf[0] = char('0' + (x / 10) % 10);
    buf[1] = char('0' + (x % 10));
    buf[2] = '\0';
    return std::string(buf);
}

const char* FileLogger::to_string(Level lvl) noexcept {
    switch (lvl) {
        case Level::trace:    return "TRACE";
        case Level::debug:    return "DEBUG";
        case Level::info:     return "INFO";
        case Level::warn:     return "WARN";
        case Level::error:    return "ERROR";
        case Level::critical: return "CRITICAL";
        case Level::off:      return "OFF";
        default:              return "UNKNOWN";
    }
}

std::tm FileLogger::safe_localtime_(std::time_t t) noexcept {
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    return tm;
}

FileLogger::FileLogger(const std::string& file_path,
                       bool append,
                       Level level,
                       bool auto_flush)
    : level_(level),
      auto_flush_(auto_flush) {
    std::error_code ec;
    std::filesystem::path p(file_path);
    auto dir = p.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir, ec)) {
        std::filesystem::create_directories(dir, ec);
    }

    stream_.open(file_path, std::ios::out | (append ? std::ios::app : std::ios::trunc));

    // Гарантируем запись «по строкам» в аварийных сценариях
    if (auto_flush_ && stream_.is_open()) {
        stream_.setf(std::ios_base::unitbuf); // flush после каждой операции вывода
    }
}

void FileLogger::set_level(Level lvl) noexcept {
    level_.store(lvl, std::memory_order_relaxed);
}

Level FileLogger::level() const noexcept {
    return level_.load(std::memory_order_relaxed);
}

std::string FileLogger::format_prefix_(Level lvl) const {
    const auto now = system_clock::now();
    const auto sec = time_point_cast<seconds>(now);
    const auto ms  = duration_cast<milliseconds>(now - sec).count();
    const std::time_t t = system_clock::to_time_t(sec);
    const std::tm tm = safe_localtime_(t);

    std::ostringstream oss;
    oss << '['
        << (tm.tm_year + 1900) << '-'
        << two_digits_(tm.tm_mon + 1) << '-'
        << two_digits_(tm.tm_mday) << ' '
        << two_digits_(tm.tm_hour) << ':'
        << two_digits_(tm.tm_min) << ':'
        << two_digits_(tm.tm_sec) << '.'
        << std::setw(3) << std::setfill('0') << ms
        << ']'
        << '[' << to_string(lvl) << "] ";
    return oss.str();
}

// Публичная "строковая" версия — с проверкой уровня
void FileLogger::log(Level lvl, std::string_view message) {
    if (lvl < level()) return;
    write_line_(lvl, message);
}

// Непосредственная запись в поток (без проверки уровней)
void FileLogger::write_line_(Level lvl, std::string_view message) {
    const std::string prefix = format_prefix_(lvl);
    std::lock_guard<std::mutex> lock(mutex_);
    if (!stream_.is_open()) return;

#if __cpp_lib_string_view >= 201606
    stream_ << prefix;
    stream_.write(message.data(), static_cast<std::streamsize>(message.size()));
    stream_ << '\n';
#else
    stream_ << prefix << std::string(message) << '\n';
#endif

    if (auto_flush_) {
        // при unitbuf это необязательно, но если unitbuf не включён, всё равно хорошо
        stream_.flush();
    }
}

void FileLogger::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stream_.is_open()) stream_.flush();
}

bool FileLogger::is_open() const noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    return stream_.is_open();
}

} // namespace logger
