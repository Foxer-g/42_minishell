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

static bool	command_expand(t_command **cmd, char ***env)
{
	intmax_t	i;

	i = 0;
	while ((*cmd)[i].infile)
	{
		if (!expand(&((*cmd)[i]), env))
			return (false);
		if (!strtrim_cmd_end(&((*cmd)[i]), env))
			return (false);
		if (!remove_quotes(&((*cmd)[i])))
			return (!parsing_error(MALLOC, "", env));
		if (!cmddup_without_empty(&((*cmd)[i]), env))
			return (false);
		i++;
	}
	return (true);
}

static bool	redir_apply(t_command **cmd, char ***env)
{
	intmax_t	i;
	char		**tmp;
	t_redir		*red;

	i = 0;
	while ((*cmd)[i].path)
	{
		red = find_redir((*cmd)[i], env);
		if (!red)
			return (false);
		tmp = cmddup_without_redir((*cmd)[i], red, env);
		if (!tmp || !command_redir_set(&(*cmd)[i], red, env))
		{
			free_redir(red);
			ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
			return (!parsing_error(MALLOC, "", env));
		}
		command_exec_set(&(*cmd)[i], tmp, ft_nt_tablen((void *)tmp));
		ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
		free_redir(red);
		i++;
	}
	ft_clear_filelist();
	return (true);
}

static t_command	*piping(t_command *cmd, char ***env)
{
	const intmax_t	*pipes = find_pipe(cmd, env);
	t_command		*res;
	intmax_t		i;
	intmax_t		j;

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
	i = 0;
	j = 0;
	while (cmd[i].path)
		if (cmd[i++].path[0] != '|')
			res[j++] = cmd_dup(cmd[i - 1], env);
	free((void *)pipes);
	return (is_error(res, j, env));
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

	tmp = command_expand(&raw_command, env);
	if (!tmp)
		return (NULL);
	tmp = redir_apply(&raw_command, env);
	if (!tmp)
		return (NULL);
	res = piping(raw_command, env);
	if (!res)
		return (NULL);
	tmp = heredocs_handler(&res, env);
	if (!tmp)
	{
		free_command(res);
		return (NULL);
	}
	return (res);
}
