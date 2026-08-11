/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:13 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:50:14 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#define RMV_QT remove_quotes

bool	redir_apply(t_command **cmd, char ***env)
{
	t_redir		*r;
	char		**tmp;
	intmax_t	i;

	i = -1;
	while ((*cmd)[++i].infile)
	{
		r = find_redir((*cmd)[i], env);
		if (!r)
			return (!parsing_error(MALLOC, "", env));
		if (!command_redir_set(&(*cmd)[i], r, env))
			return (!free_redir(r));
		tmp = cmddup_without_redir((*cmd)[i], env);
		if (!tmp)
			return (!free_redir(r));
		if (!command_exec_set(&(*cmd)[i], tmp, ft_nt_tablen((void *)tmp)))
		{
			ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
			return (!free_redir(r));
		}
		ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
		free_redir(r);
	}
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
		if (!redir_apply(cmd, env))
			return (false);
		if (!RMV_QT(&(*cmd)[i], true) && !RMV_QT(&(*cmd)[i], false))
			return (!parsing_error(MALLOC, "", env));
		if ((*cmd)[i].args && (*cmd)[i].args[0] && (*cmd)[i].args[0][0])
			if (!cmddup_without_empty(&(*cmd)[i], env))
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
	res = ft_calloc(cmd_len(cmd) / 2 + 2, sizeof(t_command));
	if (!is_valid_pipes(pipes, env) || !res)
	{
		if (!res)
			parsing_error(MALLOC, "scheduler.c : piping : res", env);
		free((void *)pipes);
		return (NULL);
	}
	ft_bzero(i, sizeof(i));
	while (cmd[i[0]].path || cmd[i[0]].args)
	{
		if (cmd[i[0]].path && cmd[i[0]].path[0] != '|')
			res[i[1]++] = cmd_dup(cmd[i[0]], env);
		i[0]++;
	}
	free((void *)pipes);
	return (is_error(res, i[1], env));
}

static bool	heredocs_handler(t_command **cmd, char ***env)
{
	const intmax_t	*heredocs = find_heredoc(*cmd, env);
	intmax_t		i;

	if (!heredocs)
		return (false);
	i = 0;
	if (!is_valid_heredocs(heredocs, *cmd))
		return (!parsing_error(INV_HEREDOC, "<<", env));
	while ((*cmd)[i].args)
	{
		if (heredocs[i] != -1)
		{
			if (!cmd_set_hd(&(*cmd)[i], heredocs[i], env))
			{
				free((void *)heredocs);
				return (false);
			}
		}
		i++;
	}
	free((void *) heredocs);
	return (true);
}

t_command	*scheduler(t_command *raw_command, char ***env)
{
	t_command	*res;
	bool		tmp;

	res = piping(raw_command, env);
	if (!res)
		return (NULL);
	tmp = command_expand(&res, env);
	if (!tmp)
		return (NULL);
	tmp = heredocs_handler(&res, env);
	if (!tmp)
	{
		free_command(res);
		return (NULL);
	}
	return (res);
}
