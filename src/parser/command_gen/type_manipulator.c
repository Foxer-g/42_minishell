/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_manipulator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:49:53 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:49:54 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

bool	command_exec_set(t_command *command, char **cmd, uint64_t len)
{
	uint64_t	i;

	if (!cmd)
		return (true);
	free((*command).path);
	(*command).path = ft_strdup(cmd[0]);
	if (!(*command).path)
		return (false);
	if ((*command).args)
		ft_free_nt_tab((*command).args, ft_nt_tablen((void *)(*command).args));
	(*command).args = ft_calloc(len + 1, sizeof(char *));
	if (!(*command).args)
		return (false);
	i = 0;
	while (i < len && cmd[i])
	{
		(*command).args[i] = ft_strdup(cmd[i]);
		if (!(*command).args[i])
			return (ft_free_nt_tab((*command).args, i), false);
		i++;
	}
	return (true);
}

bool	command_redir_set(t_command *command, t_redir *redir, char ***env)
{
	if (redir[0].type)
	{
		free(command->infile);
		command->infile = ft_strdup(command->args[redir[0].index + 1]);
		if (!command->infile)
			return (!parsing_error(MALLOC, "", env));
	}
	if (redir[1].type)
	{
		free(command->outfile);
		command->outfile = ft_strdup(command->args[redir[1].index + 1]);
		if (!command->outfile)
			return (!parsing_error(MALLOC, "", env));
		command->append = (redir[1].type == APPEND);
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

t_command	cmd_dup(t_command cmd, char ***env)
{
	t_command	res;

	res = init_command(env);
	command_exec_set(&res, cmd.args, args_len(cmd));
	free(res.infile);
	free(res.outfile);
	res.infile = ft_strdup(cmd.infile);
	if (!res.infile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.outfile = ft_strdup(cmd.outfile);
	if (!res.outfile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.append = cmd.append;
	if (!res.infile || !res.outfile)
	{
		free_command(&res);
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	}
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
