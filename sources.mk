SRC_DIR = src/
EXEC_SRCS_DIR = $(SRC_DIR)exec/
BUILTINS_SRCS_DIR = $(EXEC_SRCS_DIR)builtins/
PARSING_SRCS_DIR = $(SRC_DIR)parser/
TOKENIZER_SRCS_DIR = $(PARSING_SRCS_DIR)token/
COMMAND_GEN_SRCS_DIR = $(PARSING_SRCS_DIR)command_gen/
COMMAND_UTILS_SRCS_DIR = $(PARSING_SRCS_DIR)command_utils/
SCHEDULER_SRCS_DIR = $(PARSING_SRCS_DIR)scheduler/

EXEC_FILES = cmd_handler.c entrypoint.c utils.c
BUILTINS_FILES = cd.c echo.c env.c exit.c export.c pwd.c unset.c

PARSING_FILES = parser_check.c parser.c
TOKENIZER_FILES = tokenizer.c token_utils.c
COMMAND_GEN_FILES = command_gen.c command_gen_utils.c
COMMAND_UTILS_FILES = command_manipulator.c command_duplication.c command_lens.c
SCHEDULER_FILES = scheduler.c scheduler_utils.c redir_utils.c piping_utils.c heredoc_utils.c

EXEC_SOURCES = $(addprefix $(EXEC_SRCS_DIR), $(EXEC_FILES))
BUILTINS_SOURCES = $(addprefix $(BUILTINS_SRCS_DIR), $(BUILTINS_FILES))
PARSING_SOURCES = $(addprefix $(PARSING_SRCS_DIR), $(PARSING_FILES))
TOKENIZER_SOURCES = $(addprefix $(TOKENIZER_SRCS_DIR), $(TOKENIZER_FILES))
COMMAND_GEN_SOURCES = $(addprefix $(COMMAND_GEN_SRCS_DIR), $(COMMAND_GEN_FILES))
COMMAND_UTILS_SOURCES = $(addprefix $(COMMAND_UTILS_SRCS_DIR), $(COMMAND_UTILS_FILES))
SCHEDULER_SOURCES = $(addprefix $(SCHEDULER_SRCS_DIR), $(SCHEDULER_FILES))

SOURCES = $(SRC_DIR)main.c $(SRC_DIR)utils.c $(EXEC_SOURCES) $(BUILTINS_SOURCES) $(PARSING_SOURCES) $(TOKENIZER_SOURCES) $(COMMAND_GEN_SOURCES) $(COMMAND_UTILS_SOURCES) $(SCHEDULER_SOURCES)
