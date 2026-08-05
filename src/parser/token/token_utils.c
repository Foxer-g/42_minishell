#include "minishell.h"
#include "parser.h"

intmax_t	count_space(char *str)
{
	intmax_t	i;
	intmax_t	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			res++;
		i++;
	}
	return (res);
}

// @doc iscmd_chr
// @kind func
// @desc Check if the char is in the command type charset.
// @param c: char, Char to check.
// @return bool, Result of the check.

bool	iscmd_chr(char c)
{
	if (ft_strchr("$*'\"()|&;\n<>", c))
		return (false);
	return (true);
}

int32_t	short_type(char *token)
{
	if (token[0] == '(')
		return (O_PAR);
	else if (token[0] == ')')
		return (C_PAR);
	else if (token[0] == '\'')
		return (QUOTE);
	else if (token[0] == '"')
		return (DQUOTE);
	else if (token[0] == '|')
		return (PIPE);
	else if (token[0] == '&')
		return (AMP_SAND);
	else if (token[0] == ';')
		return (SEMI);
	else if (token[0] == '\n')
		return (NLINE);
	else if (token[0] == '<')
		return (I_REDIR);
	else if (token[0] == '>')
		return (O_REDIR);
	else if (token[0] == '*')
		return (WILDCARD);
	else if (iscmd_chr(token[0]))
		return (COMMAND);
	return (-1);
}

int32_t	composed_type(char *token)
{
	if (ft_strchr(token, '$'))
		return (ENV_DESC);
	if (ft_strlen(token) == 1 && ft_strnstr(token, "*", 2))
		return (WILDCARD);
	if (ft_strlen(token) == 2 && ft_strnstr(token, "||", 3))
		return (DPIPE);
	if (ft_strlen(token) == 2 && ft_strnstr(token, "&&", 3))
		return (DAND);
	if (ft_strlen(token) == 2 && ft_strnstr(token, ">>", 3))
		return (APPEND);
	if (ft_strlen(token) == 2 && ft_strnstr(token, "<<", 3))
		return (HEREDOC);
	if (iscmd_chr(token[0]))
		return (COMMAND);
	return (-1);
}

void	free_token(t_token *token_lst)
{
	int64_t	i;

	i = 0;
	while (token_lst[i].content)
	{
		free(token_lst[i].content);
		token_lst[i].content = NULL;
		token_lst[i].type = 0;
		i++;
	}
	free(token_lst);
}
