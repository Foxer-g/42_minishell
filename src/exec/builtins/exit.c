/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   exit.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/09 05:38:13 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/10 03:33:30 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_exit(t_command cmd, char ***ev)
{
	const uintmax_t	ac = ft_nt_tablen((void *)cmd.args);
	int32_t			exit_code;

	(void)ev;
	if (ac == 1)
		return (-(ft_atoi(ft_get_env("?", *ev))+ 1));
	exit_code = ft_atol(cmd.args[1]);
	if (errno == EINVAL)
	{
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
			cmd.args[1]);
		exit_code = 2;
	}
	else if (ac > 2)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		exit_code = 2;
	}
	else
	{
		exit_code = ((exit_code % 256) + 256) % 256;
		if (cmd.outpipe_end == -1 && cmd.infd == STDIN_FILENO)
			exit_code = -(exit_code + 1);
	}
	return (exit_code);
}
