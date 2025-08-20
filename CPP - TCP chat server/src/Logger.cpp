#include "Logger.h"

// Инициализация статических переменных
std::ofstream Logger::logFile_;
Logger::Level Logger::minLevel_ = Logger::Level::DEBUG;
std::mutex Logger::mutex_;
std::string Logger::logFilename_;
std::atomic<bool> Logger::consoleOutputEnabled_{true};
std::atomic<bool> Logger::fileOutputEnabled_{false};
std::atomic<bool> Logger::fileInitialized_{false};

void Logger::initializeFileOutput() {
    if (!fileInitialized_ && !logFilename_.empty()) {
        logFile_.open(logFilename_, std::ios::out | std::ios::app);
        if (!logFile_.is_open()) {
            std::cerr << "ERROR: Failed to open log file: " << logFilename_ << std::endl;
            fileOutputEnabled_ = false;
        } else {
            fileInitialized_ = true;
        }
    }
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Закрываем предыдущий файл, если он был открыт
    if (logFile_.is_open()) {
        logFile_.close();
        fileInitialized_ = false;
    }
    
    logFilename_ = filename;
    fileInitialized_ = false;
    
    // Если файловый вывод включен, инициализируем файл
    if (fileOutputEnabled_) {
        initializeFileOutput();
    }
}

void Logger::setMinLogLevel(Level level) {
    minLevel_ = level;
}

void Logger::enableConsoleOutput(bool enable) {
    consoleOutputEnabled_ = enable;
}

void Logger::enableFileOutput(bool enable) {
    fileOutputEnabled_ = enable;
    
    if (enable) {
        // Включаем файловый вывод
        initializeFileOutput();
    } else {
        // Выключаем файловый вывод, но не закрываем файл
        std::lock_guard<std::mutex> lock(mutex_);
        fileInitialized_ = false;
    }
}

void Logger::log(Level level, const char* format, ...) {
    if (level < minLevel_) return;
    
    va_list args;
    va_start(args, format);
    std::string formattedMessage = formatString(format, args);
    va_end(args);
    
    logInternal(level, formattedMessage);
}

void Logger::debug(const char* format, ...) {
    if (Level::DEBUG < minLevel_) return;
    
    va_list args;
    va_start(args, format);
    std::string formattedMessage = formatString(format, args);
    va_end(args);
    
    logInternal(Level::DEBUG, formattedMessage);
}

void Logger::info(const char* format, ...) {
    if (Level::INFO < minLevel_) return;
    
    va_list args;
    va_start(args, format);
    std::string formattedMessage = formatString(format, args);
    va_end(args);
    
    logInternal(Level::INFO, formattedMessage);
}

void Logger::warning(const char* format, ...) {
    if (Level::WARNING < minLevel_) return;
    
    va_list args;
    va_start(args, format);
    std::string formattedMessage = formatString(format, args);
    va_end(args);
    
    logInternal(Level::WARNING, formattedMessage);
}

void Logger::error(const char* format, ...) {
    if (Level::ERR < minLevel_) return;
    
    va_list args;
    va_start(args, format);
    std::string formattedMessage = formatString(format, args);
    va_end(args);
    
    logInternal(Level::ERR, formattedMessage);
}

void Logger::logInternal(Level level, const std::string& message) {
    std::string formattedMessage = formatMessage(level, message);
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Вывод в консоль
    if (consoleOutputEnabled_) {
        std::cerr << getLevelColor(level) << formattedMessage << "\033[0m" << std::endl;
    }
    
    // Вывод в файл
    if (fileOutputEnabled_) {
        // Если файл не инициализирован, пытаемся инициализировать
        if (!fileInitialized_) {
            initializeFileOutput();
        }
        
        // Если файл успешно инициализирован, пишем в него
        if (fileInitialized_ && logFile_.is_open()) {
            logFile_ << formattedMessage << std::endl;
            
            // Сбрасываем буфер для немедленной записи
            logFile_.flush();
        }
    }
}

std::string Logger::formatString(const char* format, va_list args) {
    // Определяем необходимый размер буфера
    va_list argsCopy;
    va_copy(argsCopy, args);
    int size = vsnprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);
    
    if (size < 0) {
        return "Formatting error";
    }
    
    // Создаем буфер нужного размера
    std::string result(size + 1, '\0');
    vsnprintf(&result[0], size + 1, format, args);
    
    // Убираем завершающий нулевой символ
    result.resize(size);
    
    return result;
}

std::string Logger::formatMessage(Level level, const std::string& message) {
    return "[" + getCurrentTime() + "] [" + levelToString(level) + "] " + message;
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    
    std::string result(buffer);
    result += "." + std::to_string(ms.count());
    return result;
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::getLevelColor(Level level) {
    switch (level) {
        case Level::DEBUG: return "\033[37m"; // Белый
        case Level::INFO: return "\033[32m"; // Зеленый
        case Level::WARNING: return "\033[33m"; // Желтый
        case Level::ERR: return "\033[31m"; // Красный
        default: return "\033[0m";
    }
}