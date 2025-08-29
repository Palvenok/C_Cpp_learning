# Define variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
TARGET = app
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Default target: builds the program
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule to compile each .c file into a .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Phony target for cleaning compiled files
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)