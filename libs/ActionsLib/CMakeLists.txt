cmake_minimum_required(VERSION 3.10)
project(ActionsLib LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаём интерфейсную библиотеку (header-only)
add_library(Actions INTERFACE)

# Указываем путь к заголовочному файлу
target_include_directories(Actions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Можно добавить установку, если планируешь распространять
# install(FILES Actions.h DESTINATION include)