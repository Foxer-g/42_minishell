/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:04 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:56:22 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_command	*full_cmd_dup(t_command *src, char ***env)
{
	intmax_t	i;
	t_command	*res;

	res = ft_calloc(cmd_len(src) + 1, sizeof(t_command));
	if (!res)
	{
		parsing_error(MALLOC, "", env);
		return (NULL);
	}
	i = 0;
	while (src[i].infile)
	{
		res[i] = cmd_dup(src[i], env);
		if (!res[i].infile)
		{
			free_command(res);
			parsing_error(MALLOC, "", env);
			return (NULL);
		}
		i++;
	}
	return (res);
}

intmax_t	cmd_len(t_command *cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd[i].infile)
		i++;
	return (i);
}

intmax_t	*find_pipe(t_command *cmd, char ***env)
{
	intmax_t	i;
	intmax_t	nb;
	intmax_t	*res;

	i = 0;
	nb = 0;
	while (cmd[i].infile)
		if (cmd[i++].path[0] == '|')
			nb++;
	if (!nb)
		return (NULL);
	i = 0;
	res = ft_calloc(nb + 2, sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	while (cmd[i].infile)
		if (cmd[i++].path[0] == '|')
			res[++res[0]] = i - 1;
	if (res[res[0]] == i - 1)
	{
		free(res);
		return ((void *)-1);
	}
	return (res);
}

t_command	*is_error(t_command *cmd, intmax_t len, char ***env)
{
	intmax_t	i;

	i = 0;
	while (i < len)
	{
		if (!cmd[i].infile)
		{
			free_command(cmd);
			parsing_error(MALLOC, "", env);
			return (NULL);
		}
		i++;
	}
	return (cmd);
}

bool	is_valid_pipes(const intmax_t *pipes, char ***env)
{
	intmax_t	i;

	i = 0;
	if (pipes[0] == 0)
		return (parsing_error(SYNTAX, "|", env));
	while (++i <= pipes[0])
		if (pipes[i] == 0 || (i < pipes[0] - 1 && pipes[i] + 2 == pipes[i + 1]))
			return (!parsing_error(INV_PIPES, "|", env));
	return (true);
}
