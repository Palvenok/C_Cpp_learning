cmake_minimum_required(VERSION 3.16)
project(Vector2IntProject)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем тип сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Оптимизационные флаги для Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")

# Подключаем заголовочные файлы
include_directories(include)

# Указываем исходники
set(SOURCES
    src/main.cpp
    src/vector2int.cpp
)

# Создаём исполняемый файл
add_executable(vector2int_app ${SOURCES})
