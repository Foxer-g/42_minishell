#include "minishell.h"
#include "parser.h"

// @doc parsing_error
// @kind func
// @desc Parsing error management.
// @param message: char *, Error message.
// @param token: char *, Invalid token content.
// @param env: char **, Environment variables.
// @return bool, Error status.

bool	parsing_error(char *message, char *token, char ***env)
{
	if (!token)
		return (false);
	write(2, message, ft_strlen(message));
	write(2, token, ft_strlen(token));
	ft_set_exit_code(1, env);
	return (true);
}

static char	*invalid_char(t_token *token)
{
	int64_t	i;

	i = 0;
	while (token[i].content)
	{
		if (token[i].type == SEMI || token[i].type == NLINE
			|| token[i].type == DAND || token[i].type == DPIPE
			|| token[i].type == O_PAR || token[i].type == C_PAR
			|| token[i].type == AMP_SAND || token[i].type == WILDCARD
			|| token[i].type == -1)
			return (token[i].content);
		i++;
	}
	return (NULL);
}

static char	*invalid_quotes(t_token *tkn)
{
	int64_t	i;
	int64_t	s;
	int64_t	d;

	i = 0;
	s = 0;
	d = 0;
	while (tkn[i].content)
	{
		s += 1 * (tkn[i].type == SQUOTE && d % 2 == 0);
		d += 1 * (tkn[i].type == DQUOTE && d % 2 == 0);
		i++;
	}
	if (s % 2 != 0)
		return ("'");
	else if (d % 2 != 0)
		return ("\"");
	return (NULL);
}

// @doc check_invalid_paterns
// @kind func
// @desc Check invalid patern or chars.
// @param token_lst: [[t_token]], Token list to check.
// @param env: char **, Environment variables.
// @return bool, Token list validity result.

bool	check_invalid_paterns(t_token *token_lst, char ***env)
{
	if (parsing_error(INV_CHAR, invalid_char(token_lst), env))
		return (1);
	else if (parsing_error(SYNTAX, invalid_quotes(token_lst), env))
		return (1);
	return (0);
}
