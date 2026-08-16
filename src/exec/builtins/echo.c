/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   echo.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42angouleme.fr>      ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/03 01:40:00 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/16 06:56:57 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_n(int32_t c)
{
	return (c == 'n');
}

int	echo(t_command cmd, char ***env)
{
	bool	nl;

	(void)env;
	if (cmd.infd < 0 || cmd.outfd < 0)
		return (1);
	nl = true;
	while (*(++cmd.args))
	{
		if (**cmd.args != '-' || !ft_str_is_valid((*cmd.args) + 1, is_n))
			break ;
		nl = false;
	}
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
