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
	intmax_t	i;

	i = 0;
	if (r[0].index[0] != -1)
	{
		while (r[0].index[i + 1] != -1)
			i++;
		free(cmd->infile);
		cmd->infile = ft_strdup(r[0].file[i]);
		if (!cmd->infile)
			return (!parsing_error(MALLOC, "", env));
	}
	if (r[1].index[0] == -1)
	{
		if (!remove_quotes(cmd, false))
			return (!parsing_error(MALLOC, "", env));
		return (true);
	}
	if (!remove_quotes((t_command *)r, true))
		return (!parsing_error(MALLOC, "", env));
	i = 0;
	while (r[1].file[i])
	{
		ft_ffopen(r[1].file[i], "w");
		i++;
	}
	free(cmd->outfile);
	cmd->outfile = ft_strdup(r[1].file[i - 1]);
	if (!cmd->outfile)
		return (!parsing_error(MALLOC, "", env));
	if (!remove_quotes(cmd, false))
		return (!parsing_error(MALLOC, "", env));
	cmd->append = r[1].type == APPEND;
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

static bool	fill_redir(t_redir *r, t_command c, char ***env)
{
	intmax_t	i;
	intmax_t	n[2];
	int32_t		t;

	i = 0;
	ft_bzero(n, sizeof(n));
	while (c.args[i])
	{
		t = is_redir(c.args[i][0], c.args[i][1]);
		if (t)
		{
			r[t != I_REDIR].type = t;
			r[t != I_REDIR].index[n[t != I_REDIR]] = i;
			r[t != I_REDIR].index[n[t != I_REDIR] + 1] = -1;
			r[t != I_REDIR].file[n[t != I_REDIR]] = ft_strdup(c.args[i + 1]);
			if (!r[t != I_REDIR].file[n[t != I_REDIR]])
				return (!parsing_error(MALLOC, "", env));
			r[t != I_REDIR].file[n[t != I_REDIR] + 1] = NULL;
			n[t != I_REDIR]++;
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
		if (is_redir(c.args[n][0], c.args[n][1])
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
