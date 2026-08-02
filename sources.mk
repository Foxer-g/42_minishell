SRC_DIR = src/
EXEC_SRCS_DIR = $(SRC_DIR)exec/
PARSING_SRCS_DIR = $(SRC_DIR)parser/

EXEC_FILES = cmd_handler.c entrypoint.c
PARSING_FILES = parser_check.c parser_utils.c\
	token/tokenizer.c token/utils.c\
	command_gen/command_gen.c command_gen/command_gen.c command_gen/type_manipulator.c\
	scheduler/scheduler.c scheduler/scheduler_utils.c scheduler/redir_utils.c scheduler/piping_utils.c
EXEC_SOURCES = $(addprefix $(EXEC_SRCS_DIR), $(EXEC_FILES))
PARSING_SOURCES = $(addprefix $(PARSING_SRCS_DIR), $(PARSING_FILES))

SOURCES = $(SRC_DIR)main.c $(SRC_DIR)utils.c $(EXEC_SOURCES) $(PARSING_SOURCES)
