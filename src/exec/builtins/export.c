/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 04:54:12 by neumann           #+#    #+#             */
/*   Updated: 2026/08/12 07:00:44 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	display_env_as_bash(t_ffile fd, char ***env)
{
	char *const	*aenv = *env;
	char		*var_name;
	uintmax_t	var_name_len;

	while (*aenv)
	{
		var_name_len = ft_strlen_until(*aenv, '=');
		var_name = ft_substr(*aenv, 0, var_name_len);
		if (ft_strncmp("?=", *aenv, 2))
		{
			if (ft_strchr(*aenv, '='))
			{
				ft_dprintf(fd, "declare -x %s=\"%s\"\n", var_name,
					*aenv + var_name_len + 1);
			}
			else
				ft_dprintf(fd, "declare -x %s\n", var_name);
		}
		free(var_name);
		aenv++;
	}
	return (0);
}

bool	is_valid_name_char(int32_t val)
{
	return (ft_isalnum(val) || val == '_');
}

bool	is_valid_name(char *name)
{
	if (!(*name == '_' || ft_isalpha(*name)))
		return (false);
	if (!ft_str_is_valid(name, is_valid_name_char))
		return (false);
	return (true);
}

int	export(t_command cmd, char ***env)
{
	char			*var;
	uintmax_t		eq_pos;
	int32_t			exit;

	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	exit = 0;
	if (ft_nt_tablen((void *)cmd.args) == 1)
		return (display_env_as_bash(cmd.outfd, env));
	while (*++cmd.args)
	{
		eq_pos = ft_strlen_until(*cmd.args, '=');
		var = ft_strndup(*cmd.args, ft_strlen_until(*cmd.args, '='));
		exit |= !is_valid_name(var);
		if ((*cmd.args)[eq_pos] == '=' && is_valid_name(var))
			ft_set_env(var, env, *cmd.args
				+ ft_strlen_until(*cmd.args, '=') + 1);
		else if (is_valid_name(var))
			ft_set_env_no_val(var, env);
		else
			ft_dprintf(2, "export: %s: not a valid identifier\n", var);
		free(var);
	}
	return (exit);
}
