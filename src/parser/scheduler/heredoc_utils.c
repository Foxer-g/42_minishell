/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                     ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 04:06:26 by toespino          #+#    #+#             */
/*   Updated: 2026/08/18 12:22:48 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	fill_args_without_hd(char ***res, t_command cmd,
		intmax_t index, char ***env)
{
	intmax_t	i;
	intmax_t	j;

	(*res)[0] = ft_strdup(cmd.args[index + 1]);
	if (!(*res)[0])
		return (!parsing_error(MALLOC, "", env));
	i = 0;
	j = 1;
	while (cmd.args[i])
	{
		if (i != index && i != index + 1)
		{
			(*res)[j] = ft_strdup(cmd.args[i]);
			if (!(*res)[j])
				return (!parsing_error(MALLOC, "", env));
			j++;
		}
		i++;
	}
	return (true);
}

static char	**args_to_tab_without_hd(t_command cmd, intmax_t index, char ***env)
{
	char	**res;

	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res)
	{
		parsing_error(MALLOC, "", env);
		return (NULL);
	}
	if (!fill_args_without_hd(&res, cmd, index, env))
	{
		ft_free_nt_tab(res, args_len(cmd));
		return (NULL);
	}
	return (res);
}

bool	cmd_set_hd(t_command *cmd, intmax_t index, char ***env)
{
	char	**tmp;

	tmp = args_to_tab_without_hd(*cmd, index, env);
	if (!tmp)
		return (false);
	if (!tmp[1])
	{
		free(cmd->path);
		cmd->path = NULL;
		ft_free_nt_tab(cmd->args, args_len(*cmd));
		cmd->args = tmp;
		tmp = NULL;
	}
	else
	{
		if (!command_exec_set(cmd, tmp, args_len(*cmd) - 1))
			return (ft_free_nt_tab(tmp, args_len(*cmd)));
		ft_free_nt_tab(tmp, args_len(*cmd));
	}
	free(cmd->infile);
	cmd->infile = ft_strdup("<<");
	return (cmd->infile != NULL);
}

static intmax_t	*heredoc_error(intmax_t *res, char *arg, char ***env)
{
	free(res);
	parsing_error(PARSING, arg, env);
	return (NULL);
}

intmax_t	*find_heredoc(t_command *c, char ***env)
{
	intmax_t	*res;
	intmax_t	i;
	intmax_t	j;

	res = ft_calloc(cmd_len(c) + 1, sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	i = -1;
	while (c[++i].infile)
	{
		res[i] = -1;
		if (!c[i].args)
			continue ;
		j = -1;
		while (c[i].args[++j])
		{
			if (c[i].args[j][0] == '<' && c[i].args[j][1] == '<')
			{
				if (res[i] != -1)
					return (heredoc_error(res, c[i].args[j], env));
				res[i] = j;
			}
		}
	}
	return (res);
}
