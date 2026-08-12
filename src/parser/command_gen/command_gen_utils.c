/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_gen_utils.c                                 ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:49:46 by toespino          #+#    #+#             */
/*   Updated: 2026/08/12 05:54:10 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static bool	ends_with_space(const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	return (len > 0 && s[len - 1] == ' ');
}

static bool	gluable(t_token_type t)
{
	return (t == COMMAND || t == QUOTE || t == DQUOTE || t == ENV_DESC);
}

static char	*resolve_quote_content(intmax_t i[3], t_token *o,
	t_token_type *res_type)
{
	char	*tmp;

	tmp = ft_strdup(o[i[0]].content);
	if (!tmp)
		return (NULL);
	if (o[i[0]].type != DQUOTE && o[i[0]].type != QUOTE)
	{
		*res_type = o[i[0]].type;
		return (tmp);
	}
	i[2] = o[i[0]].type;
	i[0]++;
	while (((intmax_t) o[i[0]].type) != i[2])
		tmp = ft_extend(tmp, o[i[0]++].content);
	if (tmp)
		tmp = ft_extend(tmp, &o[i[0]].content[0]);
	*res_type = COMMAND;
	return (tmp);
}

static bool	append_quote_token(intmax_t i[3], t_token *o, t_token *res,
	char ***env)
{
	char			*tmp;
	t_token_type	res_type;
	bool			glue;

	glue = (i[1] > 0 && i[0] > 0 && !ends_with_space(o[i[0] - 1].content)
			&& gluable(o[i[0]].type) && gluable(res[i[1] - 1].type));
	tmp = resolve_quote_content(i, o, &res_type);
	if (!tmp)
		return (!parsing_error(MALLOC, "", env));
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
		if (!append_quote_token(i, *tkns, res, env))
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
