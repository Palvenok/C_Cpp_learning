cmake_minimum_required(VERSION 3.16)
project(TestApp)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизационные флаги для Release
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")

# Установка папки куда будет собираться проект
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)

# Подключаем заголовочные файлы
include_directories(include)

# Указываем исходники
set(SOURCES
    src/main.cpp
    src/TCPServer.cpp
    src/ServerLogger.cpp
)

# Создаём исполняемый файл
add_executable(app ${SOURCES})

target_link_libraries(app PRIVATE ws2_32 wsock32)

