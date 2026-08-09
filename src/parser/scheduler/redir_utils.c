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

	if (r[0].index[0] != -1)
	{
		i = 0;
		while (r[0].index[i + 1] != -1)
			i++;
		free(cmd->infile);
		cmd->infile = ft_strdup(r[0].file[i]);
		if (!cmd->infile)
			return (!parsing_error(MALLOC, "", env));
	}
	if (r[1].index[0] != -1)
	{
		i = 0;
		while (r[1].index[i] != -1)
			ft_ffopen(r[1].file[i++], "w");
		free(cmd->outfile);
		cmd->outfile = ft_strdup(r[1].file[i - 1]);
		if (!cmd->outfile)
			return (!parsing_error(MALLOC, "", env));
		cmd->append = (r[1].type == APPEND);
	}
	return (true);
}

static int32_t	is_redir(char c1, char c2)
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

void	free_redir(t_redir *redir)
{
	free(redir[0].index);
	free(redir[1].index);
	free(redir[0].file);
	free(redir[1].file);
	free(redir);
}

static bool	fill_redir(t_redir *r, t_command cmd)
{
	intmax_t	i;
	intmax_t	n[2];
	int32_t		type;
	int32_t		pos;

	i = 0;
	ft_bzero(n, sizeof(n));
	while (cmd.args[i])
	{
		type = is_redir(cmd.args[i][0], cmd.args[i][1]);
		if (type)
		{
			pos = type != I_REDIR;
			r[pos].type = type;
			r[pos].index[n[pos]] = i;
			r[pos].index[n[pos] + 1] = -1;
			r[pos].file[n[pos]] = cmd.args[i + 1];
			r[pos].file[n[pos] + 1] = NULL;
			n[pos]++;
			i += 2;
		}
		else
			i++;
	}
	return (true);
}

t_redir	*find_redir(t_command cmd, char ***env)
{
	t_redir		*r;
	intmax_t	n;

	r = ft_calloc(2, sizeof(t_redir));
	if (!r)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	n = args_len(cmd) + 1;
	r[0].index = ft_calloc(n, sizeof(intmax_t));
	r[1].index = ft_calloc(n, sizeof(intmax_t));
	r[0].file = ft_calloc(n, sizeof(char *));
	r[1].file = ft_calloc(n, sizeof(char *));
	if (!r[0].index || !r[1].index || !r[0].file || !r[1].file)
	{
		free_redir(r);
		parsing_error(MALLOC, "", env);
		return (NULL);
	}
	r[0].index[0] = -1;
	r[1].index[0] = -1;
	fill_redir(r, cmd);
	return (r);
}
