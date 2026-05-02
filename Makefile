# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 13:42:42 by rboutelo          #+#    #+#              #
#    Updated: 2026/05/02 02:36:28 by f0xer            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include sources.mk
NAME	=	minishell
INCLUDES = ./includes
BUILD_DIR = build/
OBJECTS = $(SOURCES:src/%.c=$(BUILD_DIR)%.o)
CFLAGS = -Wall -Wextra -ggdb
LDFLAGS = -L libft -lft -lreadline
CC = cc

all: $(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

libft.a:
	make -C libft

$(NAME): $(BUILD_DIR) $(OBJECTS) libft.a
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)


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

.PHONY: clean all re fclean frama-c-available frama-c kernel eva libft.a
