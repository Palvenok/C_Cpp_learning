#include "TCPServer.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

TCPServer::TCPServer(int port) : port(port), serverSocket(INVALID_SOCKET), running(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
}

TCPServer::~TCPServer() {
    stop();
    WSACleanup();
}

bool TCPServer::start() {
    if (running) return true;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        return false;
    }

    running = true;
    acceptThread = std::thread(&TCPServer::acceptConnections, this);

    std::string message = "Server started on port " + std::to_string(port);
    Logger::info(message.c_str());

    return true;
}

void TCPServer::stop() {
    if (!running) return;

    running = false;
    closesocket(serverSocket);
    
    if (acceptThread.joinable()) {
        acceptThread.join();
    }

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& client : connectedClients) {
            closesocket(client.first);
        }
        connectedClients.clear();
    }

    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    clientThreads.clear();

    Logger::info(std::string("Server stopped").c_str());
}

bool TCPServer::isRunning() const {
    return running;
}

void TCPServer::acceptConnections() {
    while (running) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            if (running) {
                Logger::error(("Accept failed: " + std::to_string(WSAGetLastError())).c_str());
            }
            continue;
        }

        clientThreads.emplace_back(&TCPServer::handleClient, this, clientSocket, clientAddr);
    }
}

void TCPServer::sendToClient(SOCKET socket, const std::string& message) {
    
    std::string formattedMessage = "[" + SERVER_COLOR + "SERVER" + COLOR_RESET + "]: " + message;
    send(socket, formattedMessage.c_str(), formattedMessage.size(), 0);
}

std::string TCPServer::getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, ANSI_COLORS.size() - 1);
    return ANSI_COLORS[dis(gen)];
}

bool TCPServer::registerNickname(SOCKET socket, const std::string& nickname) {
    std::lock_guard<std::mutex> lock(nicknamesMutex);
    
    // Проверка уникальности никнейма
    if (usedNicknames.find(nickname) != usedNicknames.end()) {
        return false;
    }
    
    // Регистрация нового никнейма
    usedNicknames.insert(nickname);
    
    // Обновление информации о клиенте
    std::lock_guard<std::mutex> clientLock(clientsMutex);
    if (connectedClients.find(socket) != connectedClients.end()) {
        connectedClients[socket].nickname = nickname;
        connectedClients[socket].color = getRandomColor();
    }
    
    return true;
}

void TCPServer::sendPrivateMessage(SOCKET senderSocket, const std::string& targetNickname, const std::string& message) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    
    // Получаем информацию об отправителе
    std::string senderNickname = "Unknown";
    std::string senderColor = COLOR_RESET;
    SOCKET targetSocket = INVALID_SOCKET;
    
    if (connectedClients.find(senderSocket) != connectedClients.end()) {
        senderNickname = connectedClients[senderSocket].nickname;
        senderColor = connectedClients[senderSocket].color;
    }
    
    // Ищем получателя
    bool recipientFound = false;
    for (auto& client : connectedClients) {
        if (client.second.nickname == targetNickname) {
            targetSocket = client.first;
            recipientFound = true;
            break;
        }
    }
    
    if (recipientFound) {

        if (senderNickname == targetNickname) {
            std::string error = "You cannot send private messages to yourself\n";
            send(senderSocket, error.c_str(), error.size(), 0);
            
            std::string logMsg = "[PM Error] " + senderColor + senderNickname + COLOR_RESET + " -> " + targetNickname + ": " + error;
            Logger::debug(logMsg.c_str());
            return;
        }
        
        // Форматируем сообщение для получателя
        std::string toRecipient = "[PM from " + senderColor + senderNickname + COLOR_RESET + "]: " + message + '\n';
        send(targetSocket, toRecipient.c_str(), toRecipient.size(), 0);
        
        // Логируем на сервере
        std::string logMsg = "[PM] " + senderColor + senderNickname + COLOR_RESET + 
                         " -> " + connectedClients[targetSocket].color + targetNickname + COLOR_RESET + ": " + message;
        Logger::debug(logMsg.c_str());
    } else {
        // Отправляем сообщение об ошибке отправителю
        std::string error = "User '" + targetNickname + "' not found or offline\n";
        send(senderSocket, error.c_str(), error.size(), 0);
        
        // Логируем ошибку
        std::string logMsg = "[PM Error] " + senderColor + senderNickname + COLOR_RESET + " -> " + targetNickname + ": " + error;
        Logger::debug(logMsg.c_str());
    }
}

void TCPServer::broadcastMessage(const std::string& message, SOCKET senderSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    
    // Получение информации об отправителе
    std::string senderNickname = "Unknown";
    std::string senderColor = COLOR_RESET;
    
    if (connectedClients.find(senderSocket) != connectedClients.end()) {
        senderNickname = connectedClients[senderSocket].nickname;
        senderColor = connectedClients[senderSocket].color;
    }
    
    // Форматирование сообщения с цветом
    std::string formattedMessage = "[" + senderColor + senderNickname + COLOR_RESET + "]: " + message + '\n';
    
    // Рассылка всем клиентам кроме отправителя
    for (auto& client : connectedClients) {
        if (client.first != senderSocket) {
            send(client.first, formattedMessage.c_str(), formattedMessage.size(), 0);
        }
    }
}

void TCPServer::handleClient(SOCKET clientSocket, sockaddr_in clientAddr) {
    char clientIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);
    std::string clientId = std::string(clientIp) + ":" + std::to_string(clientPort);

    // Добавление клиента во временную структуру
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        connectedClients[clientSocket] = {clientAddr, "", getRandomColor(), clientSocket};
    }

    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // Запрос никнейма
    bool nicknameRegistered = false;
    while (!nicknameRegistered && running) {
        std::string prompt = "Enter your nickname: ";
        sendToClient(clientSocket, prompt);

        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) break;

        buffer[bytesReceived] = '\0';
        std::string nickname(buffer);
        nickname.erase(std::remove(nickname.begin(), nickname.end(), '\n'), nickname.end());
        nickname.erase(std::remove(nickname.begin(), nickname.end(), '\r'), nickname.end());

        // Проверка и регистрация никнейма
        if (registerNickname(clientSocket, nickname)) {
            nicknameRegistered = true;
            
            
            // Уведомление сервера
            std::string color = connectedClients[clientSocket].color;
            std::string message = "Client connected: " + color + nickname + COLOR_RESET + " (" + clientId + ")";
            Logger::debug(message.c_str());
            
            // Приветственное сообщение
            std::string welcome = "Welcome, " + color + nickname + COLOR_RESET + "! \n\tType /help for commands.\n";
            sendToClient(clientSocket, welcome);
            
            // Уведомление чата
            std::string joinMsg = color + nickname + COLOR_RESET + " joined the chat";
            broadcastMessage(joinMsg, clientSocket);
        } else {
            std::string error = "Nickname '" + nickname + "' is already taken. Please choose another one.\n";
            sendToClient(clientSocket, error);
        }
    }

    // Основной цикл обработки сообщений
    while (running && nicknameRegistered) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) break;

        buffer[bytesReceived] = '\0';
        std::string message(buffer);
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
        message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());

        // Обработка команд
        if (message == "/help") {
            std::string help = "Available commands:\n"
                               "\t/users - List online users\n"
                               "\t/exit - Disconnect from server\n"
                               "\tSend private message: @username message\n"
                               "\tExample: @alice Hello! How are you?\n";
            sendToClient(clientSocket, help);
            continue;
        }
        
        if (message == "/users") {
            std::lock_guard<std::mutex> lock(clientsMutex);
            std::string userList = "Online users (" + std::to_string(connectedClients.size()) + "):\n";
            
            for (const auto& client : connectedClients) {
                userList += "  " + client.second.color + client.second.nickname + COLOR_RESET + "\n";
            }
            
            sendToClient(clientSocket, userList);
            continue;
        }
        
        if (message == "/exit") {
            break;
        }

        // Проверка на личное сообщение
        if (message.substr(0, PM_PREFIX.length()) == PM_PREFIX) {
            // Извлекаем никнейм получателя и текст сообщения
            size_t spacePos = message.find(' ');
            if (spacePos != std::string::npos && spacePos > PM_PREFIX.length()) {
                std::string targetNickname = message.substr(
                    PM_PREFIX.length(), 
                    spacePos - PM_PREFIX.length()
                );
                
                std::string pmMessage = message.substr(spacePos + 1);
                
                // Проверяем, не пустое ли сообщение
                if (!pmMessage.empty()) {
                    sendPrivateMessage(clientSocket, targetNickname, pmMessage);
                } else {
                    std::string error = "Private message cannot be empty\n";
                    sendToClient(clientSocket, error);
                }
            } else {
                std::string error = "Invalid private message format. Use: @username message\n";
                sendToClient(clientSocket, error);
            }
            continue;
        }

        // Рассылка обычного сообщения
        broadcastMessage(message, clientSocket);
        
        // Логирование на сервере
        {
            std::string color = connectedClients[clientSocket].color;
            std::string nickname = connectedClients[clientSocket].nickname;
            std::string logMsg = "[" + color + nickname + COLOR_RESET + "]: " + message;
            Logger::debug(logMsg.c_str());
        }
    }

    // Обработка отключения клиента
    std::string nickname;
    std::string color;
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        if (connectedClients.find(clientSocket) != connectedClients.end()) {
            nickname = connectedClients[clientSocket].nickname;
            color = connectedClients[clientSocket].color;
            connectedClients.erase(clientSocket);
        }
    }

    // Освобождение никнейма
    if (!nickname.empty()) {
        std::lock_guard<std::mutex> lock(nicknamesMutex);
        usedNicknames.erase(nickname);
    }

    closesocket(clientSocket);

    if (!nickname.empty()) {
        // Уведомление сервера
        std::string message = "Client disconnected: " + color + nickname + COLOR_RESET + " (" + clientId + ")";
        Logger::debug(message.c_str());
        
        // Уведомление чата
        std::string leaveMsg = color + nickname + COLOR_RESET + " left the chat";
        broadcastMessage(leaveMsg, INVALID_SOCKET);
    }
}