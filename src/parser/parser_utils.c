#include "minishell.h"
#include "parser.h"

// @doc parsing_error
// @kind func
// @desc Parsing error management.
// @param message: char *, Error message.
// @param token: char *, Invalid token content.
// @param env: char **, Environment variables.
// @return bool, Error status.

bool	*parsing_error(char *message, char *token, char **env)
{
	if (!token)
		return (0);
	write(2, message, ft_strlen(message));
	write(2, token, ft_strlen(message));
	set_exit_code(1, env);
	return (1);
}
