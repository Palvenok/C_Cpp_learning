# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall -Wextra -std=c++20 -pedantic
INCLUDES = -Iinclude

# Имя исполняемого файла
TARGET = App

# Директории
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Исходные файлы
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Объектные файлы
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Создание директорий если их нет
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Правило по умолчанию
all: $(BIN_DIR)/$(TARGET)

# Сборка исполняемого файла
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Компиляция каждого .cpp файла в .o файл
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# make clean - Очистка
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Указание, что эти цели не являются реальными файлами
.PHONY: all clean

# make run - Запуска сборки
run:
	$(BIN_DIR)/$(TARGET)