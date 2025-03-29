CC = gcc
CFLAGS = -Wall -Wextra -Iheader
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = obj
TARGET = $(BUILD_DIR)/main

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

.PHONY: clean all