#include "minishell.h"
#include "parser.h"

static char	*invalid_char(t_token token)
{
	int64_t	i;

	i = 0;
	while (token[i].content)
	{
		if (token[i].type == SEMI || token[i].type == NLINE
			|| token[i].type == -1)
			return (token[i].content);
		i++;
	}
	return (NULL);
}

static bool	parenthesis_order(t_token *token_lst)
{
	int64_t	i;
	int64_t	j;

	i = 0;
	j = 0;
	while (token_lst[i].content)
	{
		if (token_lst[i].type == O_PAR)
			j++;
		else if (token_lst[i].type == C_PAR)
		{
			j--;
			if (j <= 0)
				return (true);
		}
		i++;
	}
	if (j != 0)
		return (2);
	return (0);
}

static char	*invalid_parenthesis(t_token *tkn)
{
    int64_t i;
    int64_t j;
    int64_t lp;

	i = 0;
	j = 0;
	lp = -1;
    while (tkn[i].content)
    {
        if (tkn[i].type == O_PAR || tkn[i].type == C_PAR)
            lp = i;
        if (tkn[i].type == O_PAR)
            j++;
        else if (tkn[i].type == C_PAR)
        {
            if (--j < 0)
                return tkn[i].content;
        }
        i++;
    }
    if (j != 0 && lp >= 0)
        return tkn[lp].content;
    return NULL;
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
