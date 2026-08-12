/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   handling_utils.c                                    ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/12 06:22:23 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 07:21:01 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

void	cleanup(t_command *command, pid_t pid)
{
	command->pid = pid;
	if (command->hd_pipe != 0)
		ft_ffclose(&command->hd_pipe);
	if (command->infd != STDIN_FILENO)
		ft_ffclose(&command->infd);
	if (command->outfd != STDOUT_FILENO)
		ft_ffclose(&command->outfd);
	ft_clear_filelist();
}

t_cmd_fun	get_builtin(t_command *cmd)
{
	const char		*blts[] = {"cd", "echo", "env", "exit",
		"export", "pwd", "unset", NULL};
	const t_cmd_fun	funcs[] = {cd, echo, minishell_env,
		minishell_exit, export, pwd, unset, NULL};
	uint8_t			i;

	if (!cmd->path)
		return (NULL);
	i = -1;
	while (blts[++i])
	{
		if (!ft_strcmp(blts[i], cmd->path))
			return ((void *)funcs[i]);
	}
	return (NULL);
}

void	fail_free(t_command *cmd, char *path, t_command *o)
{
	ft_ffclose(&cmd->outpipe_end);
	free(path);
	free_command(o);
	ft_clear_filelist();
}
