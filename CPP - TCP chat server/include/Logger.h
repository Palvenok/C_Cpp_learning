#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <vector>
#include <atomic>
#include <iostream>
#include <cstdarg>

class Logger {
public:
    // Уровни логирования
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERR
    };
    
    // Конфигурация логгера
    static void setLogFile(const std::string& filename);
    static void setMinLogLevel(Level level);
    static void enableConsoleOutput(bool enable);
    static void enableFileOutput(bool enable);

    // Основные методы логирования с поддержкой форматирования
    static void log(Level level, const char* format, ...);
    
    // Вспомогательные методы для удобства
    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warning(const char* format, ...);
    static void error(const char* format, ...);
    
private:
    // Приватный конструктор и деструктор
    Logger() = delete;
    ~Logger() = delete;

    // Внутренний метод для форматирования и записи
    static void logInternal(Level level, const std::string& message);
    
    // Форматирование сообщения
    static std::string formatMessage(Level level, const std::string& message);
    
    // Получение текущего времени
    static std::string getCurrentTime();
    
    // Получение строки уровня логирования
    static std::string levelToString(Level level);
    
    // Получение цвета для уровня логирования (для консоли)
    static std::string getLevelColor(Level level);
    
    // Инициализация файлового вывода
    static void initializeFileOutput();
    
    // Форматирование строки в стиле printf
    static std::string formatString(const char* format, va_list args);

    // Статические члены класса
    static std::ofstream logFile_;
    static Level minLevel_;
    static std::mutex mutex_;
    static std::string logFilename_;
    static std::atomic<bool> consoleOutputEnabled_;
    static std::atomic<bool> fileOutputEnabled_;
    static std::atomic<bool> fileInitialized_;
};