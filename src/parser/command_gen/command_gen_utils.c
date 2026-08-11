/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_gen_utils.c                                 ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:49:46 by toespino          #+#    #+#             */
/*   Updated: 2026/08/11 00:53:30 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// @doc count_space
// @kind func
// @desc Count spaces in a string.
// @param str: char *, String to process.
// @return intmax_t, Spaces count.

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

// @doc tkn_to_tab
// @kind func
// @desc Duplicate token array content as a string array.
// @param tkns: [[t_token]] *, Token array to get content from.
// @param i: intmax_t, index to start from.
// @param len: intmax_t, Len to apply the dup.
// @param env: char ***, Environement variables.
// @return char **, String array copied from token array.

char	**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env)
{
	intmax_t	k;
	intmax_t	end;
	char		**res;

	k = 0;
	end = i + len;
	res = ft_calloc(len + 1, sizeof(char *));
	if (!res)
		return ((void *)((uintptr_t)parsing_error(MALLOC, "", env)));
	while (i < end)
	{
		res[k] = ft_strdup(tkns[i++].content);
		if (!res[k])
			return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
		k++;
	}
	return (res);
}

static bool	ends_with_space(const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	return (len > 0 && s[len - 1] == ' ');
}

static bool	gluable(t_token_type type)
{
	return (type == COMMAND || type == QUOTE || type == DQUOTE || type == ENV_DESC);
}

static bool	cmpnd_qt_cntnt(intmax_t i[3], t_token *o, t_token *res, char ***env)
{
	char			*tmp;
	t_token_type	res_type;
	bool			glue;

	glue = (i[1] > 0 && i[0] > 0 && !ends_with_space(o[i[0] - 1].content)
		&& gluable(o[i[0]].type) && gluable(res[i[1] - 1].type));
	tmp = ft_strdup(o[i[0]].content);
	if (!tmp)
		return (!parsing_error(MALLOC, "", env));
	if (o[i[0]].type == DQUOTE || o[i[0]].type == QUOTE)
	{
		i[2] = o[i[0]].type;
		i[0]++;
		while (((intmax_t) o[i[0]].type) != i[2])
			tmp = ft_extend(tmp, o[i[0]++].content);
		if (!tmp)
			return (!parsing_error(MALLOC, "", env));
		tmp = ft_extend(tmp, &o[i[0]].content[0]);
		if (!tmp)
			return (!parsing_error(MALLOC, "", env));
		res_type = COMMAND;
	}
	else
		res_type = o[i[0]].type;
	if (glue)
	{
		res[i[1] - 1].content = ft_extend(res[i[1] - 1].content, tmp);
		free(tmp);
		if (!res[i[1] - 1].content)
			return (!parsing_error(MALLOC, "", env));
	}
	else
		res[i[1]++] = (t_token){tmp, res_type};
	return (true);
}

// @doc quote_join
// @kind func
// @desc Join quotes in a token array.
// @param tkns: [[t_token]] **, Token array to apply quote join.
// @param env: char ***, Environement variables.
// @return bool, Return status.

bool	quote_join(t_token **tkns, char ***env)
{
	t_token		*res;
	intmax_t	i[3];

	ft_bzero(i, sizeof(i));
	res = ft_calloc(1, sizeof(t_token));
	if (!res)
		return (!parsing_error(MALLOC, "", env));
	while ((*tkns)[i[0]].content)
	{
		res = ft_recalloc(res, (i[1] + 1)
				* sizeof(t_token), i[1] + 2, sizeof(t_token));
		if (!res)
			return (!parsing_error(MALLOC, "", env));
		if (!cmpnd_qt_cntnt(i, *tkns, res, env))
		{
			free_token(res);
			return (false);
		}
		i[0]++;
	}
	free_token(*tkns);
	*tkns = res;
	return (true);
}
