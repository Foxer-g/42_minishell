/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   echo.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42angouleme.fr>      ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/03 01:40:00 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/15 06:33:34 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_command cmd, char ***env)
{
	bool	nl;

	(void)env;
	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	nl = true;
	cmd.args++;
	if (*cmd.args)
		nl = !!ft_strncmp(*cmd.args, "-n", 2);
	if (!nl)
		cmd.args++;
	while (*cmd.args)
	{
		ft_putstr_fd(*cmd.args, cmd.outfd);
		cmd.args++;
		if (*cmd.args)
			ft_putchar_fd(' ', cmd.outfd);
	}
	if (nl)
		ft_putchar_fd('\n', cmd.outfd);
	return (0);
}
