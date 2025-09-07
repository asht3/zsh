CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = my_zsh
SRCS = main.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean fclean re