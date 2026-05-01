# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 13:42:42 by rboutelo          #+#    #+#              #
#    Updated: 2025/10/21 11:26:54 by rboutelo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include sources.mk
NAME	=	minishell.a
INCLUDES = ./includes
BUILD_DIR = build/
OBJECTS = $(SOURCES:src/%.c=$(BUILD_DIR)%.o)
CFLAGS = -Wall -Werror -Wextra -ggdb
CC = cc

all: $(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

libft.a:
	make -C libft
	cp libft/libft.a .

$(NAME): $(BUILD_DIR) $(OBJECTS) libft.a
	ar rv $(NAME) $(OBJECTS)

clean:
	-rm -f $(OBJECTS)

fclean: clean
	-rm -f $(NAME)

re: fclean all

compile_commands.json:
	bear -- make

frama-c-available:
	@frama-c -v

kernel: frama-c-available $(SOURCES) compile_commands.json
	@frama-c $(SOURCES) -compilation-db compile_commands.json -save kernel.sav

eva: kernel
	@frama-c -load kernel.sav -eva -eva-precision 3 -save eva.sav

frama-c: eva

.PHONY: clean all re fclean frama-c-available frama-c kernel eva
