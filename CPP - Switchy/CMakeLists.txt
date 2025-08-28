cmake_minimum_required(VERSION 3.16)
project(Switchy)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизационные флаги для Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")

# Установка папки куда будет собираться проект
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Подключаем заголовочные файлы
include_directories(include)

# Указываем исходники
set(SOURCES
    src/main.cpp
    src/Utils.cpp
    src/KeyboardHook.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32)



