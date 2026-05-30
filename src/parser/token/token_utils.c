#include "minishell.h"
#include "parser.h"

// @doc ft_iscmd_chr
// @kind func
// @desc Check if the char is in the command type charset.
// @param c: char, Char to check.
// @return bool, Result of the check.

bool	ft_iscmd_chr(char c)
{
	if (ft_strnstr("$*'\"()|&;\n<>", &c, 12))
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
		return (SQUOTE);
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
	return (-1);
}

int32_t	composed_type(char *token)
{
	if (token[0] == '$')
		return (ENV_DESC);
	else if (token[0] == '*')
		return (WILDCARD);
	else if (token[0] == '|')
		return (DPIPE);
	else if (token[0] == '&')
		return (DAND);
	else if (token[0] == '>')
		return (APPEND);
	else if (token[0] == '<')
		return (HEREDOC);
	else if (ft_iscmd_chr(token[0]))
		return (COMMAND);
	return (-1);
}

void	free_tokens(t_token *token_lst)
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
