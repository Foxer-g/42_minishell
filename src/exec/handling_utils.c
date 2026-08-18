/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   handling_utils.c                                    ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42.fr>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/12 06:22:23 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/18 11:27:53 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

char	*get_path_or_exit(t_command *c, t_command *o, char **env)
{
	char	**envpath;
	char	*path;
	int32_t	exit_code;

	envpath = ft_split(ft_get_env("PATH", env), ':');
	errno = 0;
	path = ft_find_exec(c->path, (void *)envpath);
	if (errno == EINVAL || errno == ENOENT)
	{
		exit_code = 2 + (125 * ((errno == ENOENT) | !ft_strcmp("..", c->path)));
		if (exit_code == 127)
			ft_dprintf(2, "%s: Command not found\n", c->path);
		else
			ft_dprintf(2, ".: filename argument required\n"
				".: usage: . [-p path] filename [arguments]\n");
		ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		fail_free(c, path, o);
		exit(exit_code);
	}
	ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
	return (path);
}

void	cleanup(t_command *command, pid_t pid)
{
	command->pid = pid;
	if (command->hd_pipe != 0)
		ft_ffclose(&command->hd_pipe);
	if (command->infd != STDIN_FILENO)
		ft_ffclose(&command->infd);
	if (command->outfd != STDOUT_FILENO)
		ft_ffclose(&command->outfd);
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
	ft_ffclose(&cmd->hd_pipe);
	free(path);
	free_command(o);
	ft_clear_filelist();
}
