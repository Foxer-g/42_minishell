/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   pwd.c                                               ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/06/09 05:17:46 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/02 05:06:34 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_command cmd, char ***env)
{
	const uintmax_t	ac = ft_nt_tablen((void*)cmd.args);
	char	*cwd;

	(void)env;
	if (ac != 1)
	{
		printf("pwd: expected 0 arguments; got %lu\n", ac - 1);
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
