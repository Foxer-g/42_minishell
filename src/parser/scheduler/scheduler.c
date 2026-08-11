/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:13 by toespino          #+#    #+#             */
/*   Updated: 2026/08/11 22:49:17 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#define RMV_QT remove_quotes

static bool	heredocs_handler(t_command **cmd, char ***env)
{
	const intmax_t	*heredocs;
	intmax_t		i;

	heredocs = find_heredoc(*cmd, env);
	if (!heredocs)
		return (false);
	if (!is_valid_heredocs(heredocs, *cmd))
		return (!parsing_error(INV_HEREDOC, "<<", env));
	i = 0;
	while ((*cmd)[i].infile)
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
	free((void *)heredocs);
	return (true);
}

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
		tmp = cmddup_without_redir((*cmd)[i]);
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

static bool	command_finalize(t_command **cmd, char ***env)
{
	intmax_t	i;

	i = -1;
	while ((*cmd)[++i].infile)
	{
		if (!RMV_QT(&(*cmd)[i], true)
			&& !RMV_QT(&(*cmd)[i], false))
			return (!parsing_error(MALLOC, "", env));
		if (!cmddup_without_empty(&(*cmd)[i], env))
			return (false);
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
	}
	if (!heredocs_handler(cmd, env))
		return (false);
	i = -1;
	while ((*cmd)[++i].infile)
	{
		if (!redir_apply(cmd, env))
			return (false);
	}
	return (true);
}

static t_command	*piping(t_command *cmd, char ***env)
{
	const intmax_t	*pipes;
	t_command		*res;
	intmax_t		i;
	intmax_t		j;

	pipes = find_pipe(cmd, env);
	if (!pipes)
		return (full_cmd_dup(cmd, env));
	if (pipes == (void *)-1)
		return ((void *)((uintptr_t)!parsing_error(INV_PIPES, "|", env)));
	res = ft_calloc(cmd_len(cmd) / 2 + 2, sizeof(t_command));
	if (!res)
	{
		free((void *)pipes);
		return ((void *)((uintptr_t)!parsing_error(MALLOC, "", env)));
	}
	if (!is_valid_pipes(pipes, env))
	{
		free((void *)pipes);
		free_command(res);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (cmd[i].path || cmd[i].args)
	{
		if (!cmd[i].path || cmd[i].path[0] != '|')
			res[j++] = cmd_dup(cmd[i], env);
		i++;
	}
	free((void *)pipes);
	return (is_error(res, j, env));
}

t_command	*scheduler(t_command *raw_command, char ***env)
{
	t_command	*res;

	if (!command_expand(&raw_command, env))
		return (NULL);
	res = piping(raw_command, env);
	if (!res)
		return (NULL);
	if (!command_finalize(&res, env))
	{
		free_command(res);
		return (NULL);
	}
	return (res);
}
