/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   cd.c                                                ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/03 00:51:02 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 06:41:04 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	update_pwd(char ***env)
{
	char	pathbuf[PATH_MAX];

	if (getcwd(pathbuf, PATH_MAX))
	{
		ft_set_env("PWD", env, ft_strdup(pathbuf));
		return (true);
	}
	perror("cd");
	return (false);
}

int	cd(t_command cmd, char ***env)
{
	const uintmax_t	ac = ft_nt_tablen((void *)cmd.args);

	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	if (ac > 2)
	{
		ft_dprintf(2, "minishell: cd: too many arguments\n");
		return (2);
	}
	if (!cmd.args[1])
	{
		ft_dprintf(STDERR_FILENO, "Sorry, cd needs at least one argument\n");
		return (1);
	}
	if (chdir(cmd.args[1]))
	{
		perror("cd");
		return (1);
	}
	if (!update_pwd(env))
		return (1);
	return (0);
}
