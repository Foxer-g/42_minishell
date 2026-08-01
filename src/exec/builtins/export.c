/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   export.c                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/11 04:54:12 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/07/30 19:26:28 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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

int	export(int ac, char **av, char ***env)
{
	char	*var;

	if (ac == 1)
	{
		display_env_as_bash(env);
		return (0);
	}
	while (*++av)
	{
		var = ft_strndup(*av, ft_strlen_until(*av, '='));
		set_env(var, env, *av + ft_strlen_until(*av, '='));
		free(var);
	}
	return (0);
}
