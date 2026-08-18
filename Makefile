include sources.mk
NAME	=	minishell
BUILD_DIR = build/
INCLUDES = ./includes
OBJECTS = $(SOURCES:src/%.c=$(BUILD_DIR)%.o)
CFLAGS = -Wall -Wextra -Werror -ggdb
LDFLAGS = libft/libft.a -lreadline
CC = cc

all: $(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

libft/libft.a:
	make -C libft

$(NAME): $(BUILD_DIR) $(OBJECTS) libft/libft.a
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	-rm -f $(OBJECTS)
	make clean -C libft

fclean: clean
	-rm -f $(NAME)
	-rm -fr $(BUILD_DIR)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
