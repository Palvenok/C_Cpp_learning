#include "ServerLogger.h"
#include "TCPServer.h"
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int signum) {
    running = false;
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    #ifdef _WIN32
    signal(SIGBREAK, signalHandler);
    #endif

    // Настройка логгера
    ServerLogger& logger = ServerLogger::getInstance();
    logger.setLogFile("server.log");
    logger.setMinLogLevel(ServerLogger::Level::INFO);

    try {
        TCPServer server(8080);
        if (!server.start()) {
            return 1;
        }

        logger.logServerStart(8080);
        logger.log(ServerLogger::Level::INFO, "Chat server is running. Press Ctrl+C to stop...");

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        server.stop();
    } catch (const std::exception& e) {
        logger.logError("Error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}