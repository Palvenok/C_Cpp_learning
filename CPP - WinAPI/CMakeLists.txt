cmake_minimum_required(VERSION 3.16)
project(TestApp)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизационные флаги для Release
# set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG")

# Установка папки куда будет собираться проект
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Подключаем заголовочные файлы
include_directories(include)

add_subdirectory(libs/filelogger)


# Указываем исходники
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

# Создаём исполняемый файл, WIN32 - для отключения окна консоли
add_executable(${PROJECT_NAME} ${SOURCES})

# Копируем папку ассетов с содержимым в папку сборки
# add_custom_command(TARGET app POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             ${CMAKE_SOURCE_DIR}/assets
#             ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
# )

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Линковка с WinAPI
target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32)


target_link_libraries(${PROJECT_NAME} PRIVATE filelogger)



