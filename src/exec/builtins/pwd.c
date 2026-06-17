/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   pwd.c                                               ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/09 05:17:46 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/06/16 01:05:56 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(int ac)
{
	char	*cwd;

	if (ac != 1)
	{
		printf("pwd: expected 0 arguments; got %d\n", ac - 1);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("System libc likely isn't glibc (getcwd does not malloc).\n");
		printf("Falling back to manual allocation.\n");
		cwd = malloc(PATH_MAX * sizeof(char));
		if (!getcwd(cwd, PATH_MAX))
		{
			perror("pwd");
			free(cwd);
			return (1);
		}
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
