#include "ServerLogger.h"
#include <iostream>
#include <sstream>

// ANSI коды цветов
const std::string COLOR_RESET = "\033[0m";
const std::string COLOR_DEBUG = "\033[0;37m";   // Серый
const std::string COLOR_INFO = "\033[0m";    // Зеленый
const std::string COLOR_WARNING = "\033[0;33m"; // Желтый
const std::string COLOR_ERROR = "\033[0;31m";   // Красный

ServerLogger::ServerLogger() {
    // Минимальный уровень логирования по умолчанию - INFO
}

ServerLogger::~ServerLogger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

ServerLogger& ServerLogger::getInstance() {
    static ServerLogger instance;
    return instance;
}

void ServerLogger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFilename_ = filename;
    
    if (logFile_.is_open()) {
        logFile_.close();
    }
    
    logFile_.open(filename, std::ios::out | std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void ServerLogger::setMinLogLevel(Level level) {
    minLevel_ = level;
}

void ServerLogger::enableConsoleOutput(bool enable) {
    consoleOutputEnabled_ = enable;
}

void ServerLogger::enableFileOutput(bool enable) {
    fileOutputEnabled_ = enable;
}

std::string ServerLogger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string ServerLogger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:   return "DEBUG";
        case Level::INFO:    return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERR:   return "ERROR";
        default:             return "UNKNOWN";
    }
}

std::string ServerLogger::getLevelColor(Level level) {
    switch (level) {
        case Level::DEBUG:   return COLOR_DEBUG;
        case Level::INFO:    return COLOR_INFO;
        case Level::WARNING: return COLOR_WARNING;
        case Level::ERR:   return COLOR_ERROR;
        default:             return COLOR_RESET;
    }
}

std::string ServerLogger::formatMessage(Level level, const std::string& message) {
    std::string levelStr = levelToString(level);
    return "[" + getCurrentTime() + "] [" + levelStr + "] " + message;
}

void ServerLogger::log(Level level, const std::string& message) {
    if (level < minLevel_) {
        return;
    }

    std::string formatted = formatMessage(level, message);
    std::string color = getLevelColor(level);
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (consoleOutputEnabled_) {
        std::cout << color << formatted << COLOR_RESET << std::endl;
    }
    
    if (fileOutputEnabled_ && logFile_.is_open()) {
        logFile_ << formatted << std::endl;
    }
}

// Специализированные методы для сервера

void ServerLogger::logClientConnect(const std::string& nickname, const std::string& clientId, 
                                    const std::string& color) {
    std::string message = "Client connected: " + color + nickname + COLOR_RESET + " (" + clientId + ")";
    log(Level::INFO, message);
}

void ServerLogger::logClientDisconnect(const std::string& nickname, const std::string& clientId, 
                                       const std::string& color) {
    std::string message = "Client disconnected: " + color + nickname + COLOR_RESET + " (" + clientId + ")";
    log(Level::INFO, message);
}

void ServerLogger::logMessage(const std::string& nickname, const std::string& message, 
                              const std::string& color) {
    std::string logMsg = "[" + color + nickname + COLOR_RESET + "]: " + message;
    log(Level::INFO, logMsg);
}

void ServerLogger::logPrivateMessage(const std::string& sender, const std::string& receiver, 
                                     const std::string& message, const std::string& senderColor, 
                                     const std::string& receiverColor) {
    std::string logMsg = "[PM] " + senderColor + sender + COLOR_RESET + 
                         " -> " + receiverColor + receiver + COLOR_RESET + ": " + message;
    
    log(Level::INFO, logMsg);
}

void ServerLogger::logPrivateMessageError(const std::string& sender, const std::string& target, 
                                         const std::string& error, const std::string& senderColor) {
    std::string logMsg = "[PM Error] " + senderColor + sender + COLOR_RESET + " -> " + target + ": " + error;
    log(Level::WARNING, logMsg);
}

void ServerLogger::logServerStart(int port) {
    std::string message = "Server started on port " + std::to_string(port);
    log(Level::INFO, message);
}

void ServerLogger::logServerStop() {
    log(Level::INFO, "Server stopped");
}

void ServerLogger::logError(const std::string& error) {
    log(Level::ERR, error);
}