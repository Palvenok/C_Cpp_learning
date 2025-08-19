#pragma once
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <set>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ServerLogger.h"

// ANSI коды цветов для терминала
const std::vector<std::string> ANSI_COLORS = {
    "\033[1;31m", // Красный
    "\033[1;32m", // Зеленый
    "\033[1;34m", // Синий
    "\033[1;35m", // Пурпурный
    "\033[1;36m", // Голубой
    "\033[1;91m", // Ярко-красный
    "\033[1;92m", // Ярко-зеленый
    "\033[1;93m", // Ярко-желтый
    "\033[1;94m", // Ярко-синий
    "\033[1;95m", // Ярко-пурпурный
    "\033[1;96m", // Ярко-голубой
};
const std::string COLOR_RESET = "\033[0m";
const std::string SERVER_COLOR = "\033[3;33m";

const std::string PM_PREFIX = "@";

struct ClientInfo {
    sockaddr_in address;
    std::string nickname;
    std::string color;
    SOCKET socket;
};

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();
    
    bool start();
    void stop();
    bool isRunning() const;

private:
    void handleClient(SOCKET clientSocket, sockaddr_in clientAddr);
    void acceptConnections();
    void broadcastMessage(const std::string& message, SOCKET senderSocket);
    void sendToClient(SOCKET socket, const std::string& message);
    void sendPrivateMessage(SOCKET senderSocket, const std::string& targetNickname, const std::string& message);
    std::string getRandomColor();
    bool registerNickname(SOCKET socket, const std::string& nickname);

    int port;
    SOCKET serverSocket;
    std::atomic<bool> running;
    
    std::vector<std::thread> clientThreads;
    std::thread acceptThread;
    
    // Синхронизация
    std::mutex clientsMutex;
    std::mutex nicknamesMutex;
    
    // Хранение данных
    std::unordered_map<SOCKET, ClientInfo> connectedClients;
    std::set<std::string> usedNicknames;
};