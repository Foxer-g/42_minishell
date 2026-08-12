/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   exit.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/09 05:38:13 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 06:55:51 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_exit(t_command cmd, char ***ev)
{
	int32_t			exit_code;

	(void)ev;
	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	if (ft_nt_tablen((void *)cmd.args) == 1)
		return (-(ft_atoi(ft_get_env("?", *ev)) + 1));
	exit_code = ft_atol(cmd.args[1]);
	if (errno == EINVAL)
	{
		ft_dprintf(2, "exit: %s: numeric argument required\n", cmd.args[1]);
		exit_code = 2;
	}
	else if (ft_nt_tablen((void *)cmd.args) > 2)
	{
		ft_dprintf(2, "exit: too many arguments\n");
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
