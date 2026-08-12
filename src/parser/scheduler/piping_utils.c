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

intmax_t	*find_pipe(t_command *cmd, char ***env)
{
	intmax_t	i[2];
	intmax_t	*res;

	ft_bzero(i, sizeof(i));
	while (cmd[i[0]].infile)
	{
		if (cmd[i[0]].path && cmd[i[0]].path[0] == '|')
			i[1]++;
		i[0]++;
	}
	if (!i[1])
		return (NULL);
	res = ft_calloc(i[1] + 2, sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	i[0] = -1;
	while (cmd[++i[0]].infile)
		if (cmd[i[0]].path && cmd[i[0]].path[0] == '|')
			res[++res[0]] = i[0];
	if (res[res[0]] == i[0] - 1)
	{
		free(res);
		return ((void *)-1);
	}
	return (res);
}

bool	is_valid_pipes(const intmax_t *pipes, char ***env)
{
	intmax_t	i;

	i = 0;
	if (pipes[0] == 0)
		return (parsing_error(SYNTAX, "|", env));
	while (++i <= pipes[0])
	{
		if (pipes[i] == 0)
			return (!parsing_error(INV_PIPES, "|", env));
		if (i < pipes[0] && pipes[i] + 1 == pipes[i + 1])
			return (!parsing_error(INV_PIPES, "|", env));
	}
	return (true);
}
