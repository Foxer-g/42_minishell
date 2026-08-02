/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 04:54:12 by neumann           #+#    #+#             */
/*   Updated: 2026/08/02 05:05:26 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_env_as_bash(char ***env)
{
	char *const	*aenv = *env;
	char		*var_name;
	uintmax_t	var_name_len;

	while (*aenv)
	{
		var_name_len = ft_strlen_until(*aenv, '=');
		var_name = ft_substr(*aenv, 0, var_name_len);
		if (ft_strchr(*aenv, '='))
		{
			printf("declare -x %s=\"%s\"\n", var_name,
				*aenv + var_name_len + 1);
		}
		else
			printf("declare -x %s\n", var_name);
		free(var_name);
		aenv++;
	}
}

int	export(t_command cmd, char ***env)
{
	const uintmax_t	ac = ft_nt_tablen((void*)cmd.args);
	char	*var;

	if (ac == 1)
	{
		display_env_as_bash(env);
		return (0);
	}
	while (*++cmd.args)
	{
		var = ft_strndup(*cmd.args, ft_strlen_until(*cmd.args, '='));
		ft_set_env(var, env, *cmd.args + ft_strlen_until(*cmd.args, '='));
		free(var);
	}
	return (0);
}
