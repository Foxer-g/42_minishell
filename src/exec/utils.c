/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   utils.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/06 08:56:36 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/06 09:00:08 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(t_command *cmd)
{
	char	*tmp;
	char	**args;

	args = cmd->args;
	while (*args)
	{
		if (**args == '\'' || **args == '"')
		{
			tmp = ft_substr(*args, 1, ft_strlen(*args) - 2);
			free(*args);
			*args = tmp;
		}
		args++;
	}
}
