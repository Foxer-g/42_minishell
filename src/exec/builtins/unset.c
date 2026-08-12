/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:17:48 by neumann           #+#    #+#             */
/*   Updated: 2026/08/12 06:58:42 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_remove_at(char ***env, uintmax_t i)
{
	free((*env)[i]);
	while ((*env)[i])
	{
		(*env)[i] = (*env)[i + 1];
		i++;
	}
}

int	unset(t_command cmd, char ***env)
{
	char		**vars;
	uintmax_t	i[2];

	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	vars = cmd.args;
	while (*++vars)
	{
		i[1] = ft_strlen_until(*vars, '=');
		i[0] = 0;
		while ((*env)[i[0]])
		{
			if (!(!ft_strncmp(*vars, (*env)[i[0]], i[1]) && ((*env)[i[0]][i[1]]
				== '=' || !(*env)[i[0]][i[1]])))
				i[0]++;
			else if (ft_strchr(*vars, '='))
			{
				free((*env)[i[0]]);
				(*env)[i[0]++] = ft_strdup("");
			}
			else
				env_remove_at(env, i[0]);
		}
	}
	return (0);
}
