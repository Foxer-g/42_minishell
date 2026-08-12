/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:08 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:56:31 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

bool	command_redir_set(t_command *cmd, t_redir *r, char ***env)
{
	if (!set_infile(cmd, r, env))
		return (false);
	if (!set_outfile(cmd, r, env))
		return (false);
	if (!remove_quotes(cmd, false))
		return (!parsing_error(MALLOC, "", env));
	return (true);
}

int32_t	is_redir(char c1, char c2)
{
	if (!c1)
		return (0);
	if (!c2)
		return (((c1 == '<') * I_REDIR) + ((c1 == '>') * O_REDIR));
	if (c1 != c2 && (c1 == '<' || c1 == '>'))
		return (((c1 == '<') * I_REDIR) + ((c1 == '>') * O_REDIR));
	else if (c1 != c2)
		return (0);
	if (c1 == c2 && c1 == '>')
		return (APPEND);
	return (0);
}

static bool	fill_one_element(t_redir *r, t_command c, intmax_t i, intmax_t *n)
{
	int32_t		t;
	intmax_t	j;

	t = is_redir(c.args[i][0], c.args[i][1]);
	j = t != I_REDIR;
	r[j].type = t;
	r[j].index[n[j]] = i;
	r[j].index[n[j] + 1] = -1;
	r[j].file[n[j]] = ft_strdup(c.args[i + 1]);
	if (!r[j].file[n[j]])
		return (false);
	r[j].file[n[j] + 1] = NULL;
	n[j]++;
	return (true);
}

static bool	fill_redir(t_redir *r, t_command c, char ***env)
{
	intmax_t	i;
	intmax_t	n[2];

	i = 0;
	ft_bzero(n, sizeof(n));
	while (c.args[i])
	{
		if (!c.args[i][0])
		{
			i++;
			continue ;
		}
		if (is_redir(c.args[i][0], c.args[i][1]))
		{
			if (!fill_one_element(r, c, i, n))
				return (!parsing_error(MALLOC, "", env));
			i += 2;
		}
		else
			i++;
	}
	return (true);
}

t_redir	*find_redir(t_command c, char ***env)
{
	t_redir		*r;
	intmax_t	n;

	n = args_len(c) + 1;
	r = redir_calloc(n, env);
	if (!r)
		return (NULL);
	n = 0;
	while (c.args[n])
	{
		if (c.args[n][0] && is_redir(c.args[n][0], c.args[n][1])
			&& (!c.args[n + 1] || *c.args[n + 1] == '|'))
		{
			free_redir(r);
			parsing_error(PARSING, c.args[n], env);
			return (NULL);
		}
		n++;
	}
	if (!fill_redir(r, c, env))
	{
		free_redir(r);
		return (NULL);
	}
	return (r);
}
