/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:13 by toespino          #+#    #+#             */
/*   Updated: 2026/08/11 23:05:09 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

bool	heredocs_handler(t_command **cmd, char ***env)
{
	const intmax_t	*heredocs = find_heredoc(*cmd, env);
	intmax_t		i;

	if (!heredocs)
		return (false);
	if (!is_valid_heredocs(heredocs, *cmd))
	{
		free((void *)heredocs);
		return (!parsing_error(INV_HEREDOC, "<<", env));
	}
	i = 0;
	while ((*cmd)[i].infile)
	{
		if (heredocs[i] != -1 && !cmd_set_hd(&(*cmd)[i], heredocs[i], env))
		{
			free((void *)heredocs);
			return (false);
		}
		i++;
	}
	free((void *)heredocs);
	return (true);
}

bool	redir_apply(t_command *cmd, char ***env)
{
	t_redir	*r;
	char	**tmp;

	r = find_redir(*cmd, env);
	if (!r)
		return (false);
	if (r[0].type == HEREDOC)
	{
		if (!heredocs_handler(&cmd, env))
			return (free_redir(r));
	}
	if (!command_redir_set(cmd, r, env))
		return (free_redir(r));
	tmp = cmddup_without_redir(*cmd);
	if (!tmp)
		return (free_redir(r));
	if (!command_exec_set(cmd, tmp, ft_nt_tablen((void *)tmp)))
	{
		ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
		return (free_redir(r));
	}
	ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
	free_redir(r);
	return (true);
}

static bool	command_expand(t_command **cmd, char ***env)
{
	intmax_t	i;

	i = -1;
	while ((*cmd)[++i].infile)
	{
		if (!expand(&(*cmd)[i], env))
			return (false);
		if (!strtrim_cmd_end(&(*cmd)[i], env))
			return (false);
	}
	i = -1;
	while ((*cmd)[++i].infile)
	{
		if (!redir_apply(&(*cmd)[i], env))
			return (false);
	}
	return (true);
}

static t_command	*piping(t_command *cmd, char ***env)
{
	const intmax_t	*pipes = find_pipe(cmd, env);
	t_command		*res;
	intmax_t		i[2];

	if (!pipes)
		return (full_cmd_dup(cmd, env));
	if (pipes == (void *)-1)
		return ((void *)((uintptr_t) !parsing_error(INV_PIPES, "|", env)));
	if (!is_valid_pipes(pipes, env))
		return (free((void *)pipes), NULL);
	res = ft_calloc(cmd_len(cmd) / 2 + 2, sizeof(t_command));
	if (!res)
	{
		free((void *)pipes);
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	}
	ft_bzero(i, sizeof(i));
	while (cmd[i[0]].path || cmd[i[0]].args)
	{
		if (!cmd[i[0]].path || cmd[i[0]].path[0] != '|')
			res[i[1]++] = cmd_dup(cmd[i[0]], env);
		i[0]++;
	}
	free((void *)pipes);
	return (is_error(res, i[1], env));
}

t_command	*scheduler(t_command *raw_command, char ***env)
{
	t_command	*r;
	intmax_t	i;

	if (!command_expand(&raw_command, env))
		return (NULL);
	r = piping(raw_command, env);
	if (!r)
		return (NULL);
	if (!command_finalize(&r, env))
		return ((void *)((uintptr_t)free_command(r)));
	i = -1;
	while (r[++i].infile)
	{
		if (!ft_strcmp("<<", r[i].infile) && r[i].args && r[i].args[1])
		{
			free(r[i].path);
			r[i].path = ft_strdup(r[i].args[1]);
		}
		else if (!ft_strcmp("<<", r[i].infile) && r[i].args && !r[i].args[1])
		{
			free(r[i].path);
			r[i].path = NULL;
		}
	}
	return (r);
}
