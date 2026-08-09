/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:17:48 by neumann           #+#    #+#             */
/*   Updated: 2026/08/09 07:00:21 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
	uintmax_t	i;
	uintmax_t	len;

	vars = cmd.args;
	while (*++vars)
	{
		len = ft_strlen_until(*vars, '=');
		i = 0;
		while ((*env)[i])
		{
			if (!(!ft_strncmp(*vars, (*env)[i], len) && ((*env)[i][len] == '='
				|| !(*env)[i][len])))
				i++;
			else if (ft_strchr(*vars, '='))
			{
				free((*env)[i]);
				(*env)[i++] = ft_strdup("");
			}
			else
				env_remove_at(env, i);
		}
	}
	return (0);
}
