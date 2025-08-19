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

class ServerLogger {
public:

    // Уровни логирования
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERR
    };

    // Получение экземпляра логгера (синглтон)
    static ServerLogger& getInstance();

    // Конфигурация логгера
    void setLogFile(const std::string& filename);
    void setMinLogLevel(Level level);
    void enableConsoleOutput(bool enable);
    void enableFileOutput(bool enable);

    // Основные методы логирования
    void log(Level level, const std::string& message);
    
    // Специализированные методы для нашего сервера
    void logClientConnect(const std::string& nickname, const std::string& clientId, const std::string& color);
    void logClientDisconnect(const std::string& nickname, const std::string& clientId, const std::string& color);
    void logMessage(const std::string& nickname, const std::string& message, const std::string& color);
    void logPrivateMessage(const std::string& sender, const std::string& receiver, 
                           const std::string& message, const std::string& senderColor, 
                           const std::string& receiverColor);
    void logPrivateMessageError(const std::string& sender, const std::string& target, 
                                const std::string& error, const std::string& senderColor);
    void logServerStart(int port);
    void logServerStop();
    void logError(const std::string& error);

private:
    ServerLogger(); // Приватный конструктор
    ~ServerLogger();

    // Форматирование сообщения
    std::string formatMessage(Level level, const std::string& message);
    
    // Получение текущего времени
    std::string getCurrentTime();
    
    // Получение строки уровня логирования
    std::string levelToString(Level level);
    
    // Получение цвета для уровня логирования
    std::string getLevelColor(Level level);

    // Члены класса
    std::ofstream logFile_;
    Level minLevel_ = Level::INFO;
    std::mutex mutex_;
    std::string logFilename_;
    std::atomic<bool> consoleOutputEnabled_{true};
    std::atomic<bool> fileOutputEnabled_{true};
};