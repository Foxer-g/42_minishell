/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:17:48 by neumann           #+#    #+#             */
/*   Updated: 2026/08/05 00:51:45 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_command cmd, char ***env)
{
	char		**vars;
	uintmax_t	i;

	vars = cmd.args;
	while (*++vars)
	{
		i = -1;
		while ((*env)[++i])
		{
			if (!ft_strncmp(*vars, (*env)[i], ft_strlen_until(*vars, '=')))
			{
				if (ft_strchr(*vars, '='))
				{
					free((*env)[i]);
					(*env)[i] = ft_strdup("");
				}
				else
				{
					free((*env)[i]);
					(*env)[i] = NULL;
				}
			}
		}
	}
	return (0);
}
