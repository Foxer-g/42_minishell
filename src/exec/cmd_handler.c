/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/07/30 19:18:30 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/02 05:18:57 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"
#define BLTS {"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL}

// @doc safe_exec_setup
// @kind func
// @desc Sets up the stdin and stdout for a command.
// @param out: t_ffile, the file that serves as stdout.
// @param in: t_ffile, the file that serves as stdin.
// @returns int32_t, the exit status.
int32_t	safe_exec_setup(t_ffile out, t_ffile in)
{
	int32_t	failed;

	if (out == STDOUT_FILENO && in == STDIN_FILENO)
		return (0);
	failed = -1;
	if (in >= 0)
		failed = dup2(in, STDIN_FILENO);
	if (failed < 0)
	{
		if (out >= 0)
			ft_ffclose(out);
		if (in >= 0)
			ft_ffclose(in);
		return (-1);
	}
	if (out >= 0)
		failed = dup2(out, STDOUT_FILENO);
	if (out >= 0 && out != STDOUT_FILENO && out != STDIN_FILENO)
		ft_ffclose(out);
	if (in >= 0 && in != STDOUT_FILENO && in != STDIN_FILENO)
		ft_ffclose(in);
	if (failed < 0 || out < 0)
		return (-1);
	return (0);
}

// @doc execute
// @kind func
// @desc Ececute a prepared command with a specific stdin and stdout file.
// @param cmd: [[t_command]], the command to execute.
// @param out: t_ffile, the file that serves as stdout.
// @param in: t_ffile, the file that serves as stdin.
// @param env: char **, the environment that will be passed to execve.
// @returns int8_t, exit status.
int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env)
{
	int32_t	failed;

	failed = safe_exec_setup(out, in);
	if (failed == 0)
	{
		execve(cmd.path, cmd.args, env);
		failed = errno;
		perror(cmd.args[0]);
	}
	free(cmd.path);
	ft_free_nt_tab(cmd.args, ft_nt_tablen((void **)cmd.args));
	ft_clear_filelist();
	exit(127 * (failed == ENOENT) | 1);
}

t_cmd_fun	get_func(enum e_builtin builtin)
{
	void	*funcs[119] = {
	[CD] = cd,
	[PWD] = pwd,
	[ECHO] = echo,
	[EXIT] = minishell_exit,
	[EXPORT] = export,
	[UNSET] = unset,
	[ENV] = env,
	};

	return (funcs[builtin]);
}

// @doc exec_single
// @kind func
// @desc Execute a single command/handles builtins.
// @param command: [[t_command]] *, the command to execute.
// @param env: char **, the environment to work off of.
// @returns int8_t, exit status.
int8_t	exec_single(t_command *command, char ***env)
{
	int8_t	status;
	uint8_t	i;

	i = 0;
	while (BLTS[i])
	{
		if (!ft_strcmp(BLTS[i], command->path))
		{
			status = (get_func(command->path[2]))(*command, env);
		}
		i++;
	}
	else
		status = execute(*command, command->outfd, command->infd, env);
	return (status);
}
