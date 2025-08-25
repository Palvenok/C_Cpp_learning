cmake_minimum_required(VERSION 3.15)
project(SimpleFileLogger VERSION 0.1.0 LANGUAGES CXX)

add_library(filelogger
    src/FileLogger.cpp
    src/Logger.cpp
)

# Публичные заголовки
target_include_directories(filelogger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Требуемый стандарт C++
target_compile_features(filelogger PUBLIC cxx_std_17)

# Для старых GCC (до 9) может понадобиться линковка с stdc++fs из-за <filesystem>
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(filelogger PUBLIC stdc++fs)
endif()

# Опционально: более "красивое" имя артефакта
set_target_properties(filelogger PROPERTIES OUTPUT_NAME "filelogger")

# Установка (install) — чтобы использовать после "make install"
install(TARGETS filelogger
    EXPORT fileloggerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# Экспорт целевой сборки (для add_subdirectory это не обязательно)
install(EXPORT fileloggerTargets
    NAMESPACE filelogger::
    DESTINATION lib/cmake/filelogger
)
