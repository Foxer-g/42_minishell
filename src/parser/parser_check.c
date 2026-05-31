#include "minishell.h"
#include "parser.h"

static char	*invalid_char(t_token token)
{
	int64_t	i;

	i = 0;
	while (token_lst[i].content)
	{
		if (token_lst.type == SEMI || token_lst.type == NLINE)
			return (token.content);
		i++;
	}
	return (NULL);
}

static bool	*parenthesis_order(t_token *token_lst)
{
	(void)token_lst;
	return (0);
}

static char	*invalid_parenthesis(t_token *tkns)
{
	int64_t	i;
	int64_t	o;
	int64_t	c;
	int64_t	fp;

	i = 0;
	o = 0;
	c = 0;
	fp = -1;
	while (tkns[i].content)
	{
		fp = i * (fp == -1) * (tkns[i].type == O_PAR || tkns[i].type == C_PAR);
		o += 1 * (tkns[i].type == O_PAR);
		c += 1 * (tkns[i].type == C_PAR);
		i++;
	}
	if (o != c || tkns[fp].type == C_PAR)
		return (tkns[fp].content);
	else if (parenthesis_order(tkns))
		return ("putain con");
	return (0);
}

static char	*invalid_quotes(t_token *tkn)
{
	int64_t	i;
	int64_t	s;
	int64_t	d;
	int64_t	fp[2];

	i = 0;
	s = 0;
	d = 0;
	fp = -1;
	while (tkn[i].content)
	{
		fp = i * (fp == -1) * (tkn[i].type == SQUOTE || tkn[i].type == DQUOTE);
		s += 1 * (tkn[i].type == SQUOTE);
		d += 1 * (tkn[i].type == DQUOTE);
		i++;
	}
	if (s % 2 != 0)
		return ("'");
	else if (s % 2 != 0)
		return ("\"");
	return (NULL);
}

// @doc check_invalid_paterns
// @kind func
// @desc Check invalid patern or chars.
// @param token_lst: [[t_token]], Token list to check.
// @param env: char **, Environment variables.
// @return bool, Token list validity result.

bool	check_invalid_paterns(t_token *token_lst, char **env)
{
	if (parsing_error(CHAR, invalid_char(token_lst), env))
		return (1);
	else if (parsing_error(SYNTAX, invalid_parenthesis(token_lst), env))
		return (1);
	else if (parsing_error(SYNTAX, invalid_quotes(token_lst), env))
		return (1);
	return (0);
}
