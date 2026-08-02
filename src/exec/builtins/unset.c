/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:17:48 by neumann           #+#    #+#             */
/*   Updated: 2026/08/02 05:10:11 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_command cmd, char ***env)
{
	char	**target;
	char	**vars;

	vars = cmd.args + 1;
	while (*vars++)
	{
		if (ft_strchr(*(vars - 1), '='))
			ft_set_env(*(vars - 1), env, "");
		if (ft_strchr(*(vars - 1), '='))
			return (0);
		while (*env++)
		{
			if (ft_strcmp(**(env - 1), *(vars - 1)))
				continue ;
			target = *env;
			break ;
		}
		if (*env)
			free(*target);
		while (*env && *(env + 1))
			env++;
		*target = **env;
		**env = NULL;
	}
	return (0);
}
