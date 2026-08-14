/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:58:56 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:58:57 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

// @doc tkn_from_split
// @kind func
// @desc Generate tokens form a splitted input.
// @param tkn: [[t_token]] **, Token list to fill.
// @param i: int64_t *, Token list index.
// @param split: char *, Splitted input.
// @param env: char***, Environment variables.
// @return bool, Exit status.

bool	tkn_from_split(t_token **tkn, int64_t *i, char *split, char ***env)
{
	int64_t	len;
	int64_t	j;

	j = 0;
	while (split[j])
	{
		len = get_token_len(split, j);
		(*tkn)[*i].content = ft_substr(split, j, len);
		if (!(*tkn)[*i].content)
		{
			free_token((*tkn));
			return (!parsing_error(MALLOC, "", env));
		}
		(*tkn)[*i].type = get_token_type((*tkn)[*i].content);
		j += len;
		(*i)++;
	}
	return (true);
}

// @doc short_type
// @kind func
// @desc Get the type of a single element token.
// @param token: char *, Token content.
// @return int32_t, Token type.

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
	else if (iscmd_chr(token[0]) || token[0] == '$')
		return (COMMAND);
	return (-1);
}

// @doc composed_type
// @kind func
// @desc Get the type of a composed type token.
// @param tkn: char *, Token content.
// @return int32_t, Token type.

int32_t	composed_type(char *tkn)
{
	if (ft_strchr(tkn, '$'))
		return (ENV_DESC);
	if ((ft_strlen(tkn) - count_space(tkn)) == 1 && ft_strnstr(tkn, "*", 2))
		return (WILDCARD);
	if ((ft_strlen(tkn) - count_space(tkn)) == 2 && ft_strnstr(tkn, "||", 3))
		return (DPIPE);
	if ((ft_strlen(tkn) - count_space(tkn)) == 2 && ft_strnstr(tkn, "&&", 3))
		return (DAND);
	if ((ft_strlen(tkn) - count_space(tkn)) == 2 && ft_strnstr(tkn, ">>", 3))
		return (APPEND);
	if ((ft_strlen(tkn) - count_space(tkn)) == 2 && ft_strnstr(tkn, "<<", 3))
		return (HEREDOC);
	if (iscmd_chr(tkn[0]))
		return (COMMAND);
	return (-1);
}

// @doc free_token
// @kind func
// @desc Free a NULL terminated [[t_token]] array.
// @param token_lst: [[t_token]], Token array to free.

void	*free_token(t_token *token_lst)
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
	return (NULL);
}
