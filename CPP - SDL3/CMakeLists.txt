cmake_minimum_required(VERSION 3.16)
project(TestApp)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизационные флаги для Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")

# Установка папки куда будет собираться проект
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)

# Подключаем SDL3 - должна быть установлена через pacman mingw64/ucrt64
find_package(SDL3 REQUIRED)

# Подключаем заголовочные файлы
include_directories(include)

# Указываем исходники
set(SOURCES
    src/main.cpp
)

# Создаём исполняемый файл, WIN32 - для отключения окна консоли
add_executable(app WIN32 ${SOURCES})

# Линковка библиотеки SDL3
target_link_libraries(app SDL3::SDL3)

# Копируем папку ассетов с содержимым в папку сборки
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

# Копируем библиотеку SDL3
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/SDL3.dll"
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
