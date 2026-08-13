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

static bool	heredocs_handler(t_command **cmd, char ***env)
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
	ft_clear_filelist();
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
	const intmax_t	*pipes = find_pipe(cmd, env);
	t_command		*res;
	intmax_t		i[2];

	if (!pipes)
		return (full_cmd_dup(cmd, env));
	if (pipes == (void *)-1 || !is_valid_pipes(pipes, env))
		return ((void *)((uintptr_t) !parsing_error(INV_PIPES, "|", env)));
	res = ft_calloc(cmd_len(cmd) / 2 + 2, sizeof(t_command));
	if (!res)
	{
		free((void *)pipes);
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	}
	if (!is_valid_pipes(pipes, env))
		return ((void *)((uintptr_t) free_command(res)));
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
	t_command	*res;
	intmax_t	i;

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
	i = 0;
	while (res[i].infile)
	{
		if (!ft_strcmp("<<", res[i].infile))
		{
			free(res[i].path);
			res[i].path = ft_strdup(res[i].args[1]);
		}
		i++;
	}
	return (res);
}
