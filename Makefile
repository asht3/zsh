CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = my_zsh
SRCS = src/main.c \
	   src/utils.c \
	   src/process.c \
	   src/builtins.c \

# Object files will be placed in the obj/ directory
OBJ_DIR = obj
OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Create the obj/ directory if it doesn't exis
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Full clean
fclean: clean
	rm -f $(TARGET)

# Rebuild
re: fclean $(TARGET)

# Execute the target
run: $(TARGET)
	./$(TARGET)

.PHONY: clean fclean re