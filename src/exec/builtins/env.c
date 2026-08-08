/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                               ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 01:04:01 by neumann           #+#    #+#             */
/*   Updated: 2026/08/08 05:03:35 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_env(t_command cmd, char ***ev)
{
	const uintmax_t	ac = ft_nt_tablen((void*)cmd.args);
	uintmax_t		i;

	if (ac != 1)
	{
		printf("env: expected 0 arguments (args unsupported in minishell);"
			" got %lu\n", ac - 1);
		return (1);
	}
	i = 0;
	while ((*ev)[i])
	{
		if (ft_strchr((*ev)[i], '=') && ft_strncmp("?=", (*ev)[i], 2))
			printf("%s\n", (*ev)[i]);
		i++;
	}
	return (0);
}
