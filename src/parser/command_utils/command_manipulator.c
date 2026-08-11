/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_manipulator.c                               ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:49:53 by toespino          #+#    #+#             */
/*   Updated: 2026/08/08 04:34:51 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

bool	command_exec_set(t_command *command, char **cmd, uint64_t len)
{
	uint64_t	i;

	if (!cmd)
		return (true);
	free(command->path);
	command->path = NULL;
	if (!len)
	{
		if (command->args)
			ft_free_nt_tab(command->args, ft_nt_tablen((void *)command->args));
		command->args = NULL;
		return (true);
	}
	command->path = ft_strdup(cmd[0]);
	if (!command->path)
		return (false);
	if (command->args)
		ft_free_nt_tab(command->args, ft_nt_tablen((void *)command->args));
	command->args = ft_calloc(len + 1, sizeof(char *));
	if (!command->args)
		return (false);
	i = 0;
	while (i < len && cmd[i])
	{
		command->args[i] = ft_strdup(cmd[i]);
		if (!command->args[i++])
			return (ft_free_nt_tab(command->args, i - 1), false);
	}
	return (true);
}

t_command	init_command(char ***env)
{
	t_command	res;

	res.path = NULL;
	res.args = NULL;
	res.pid = 0;
	res.infd = STDIN_FILENO;
	res.outfd = STDOUT_FILENO;
	res.outpipe_end = -1;
	res.hd_pipe = 0;
	res.append = false;
	res.infile = ft_strdup("stdin");
	if (!res.infile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.outfile = ft_strdup("stdout");
	if (!res.infile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.builtin = false;
	return (res);
}

void	free_command(t_command *cmds)
{
	uint64_t	i;
	uint64_t	j;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i].path || cmds[i].args)
	{
		free(cmds[i].path);
		if (cmds[i].args)
		{
			j = 0;
			while (cmds[i].args[j])
			{
				free(cmds[i].args[j]);
				j++;
			}
			free(cmds[i].args);
		}
		free(cmds[i].infile);
		free(cmds[i].outfile);
		i++;
	}
	free(cmds);
}
