/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42angouleme.fr>      ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/07/30 19:18:30 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/15 06:33:28 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

// @doc safe_exec_setup
// @kind func
// @desc Sets up the stdin and stdout for a command.
// @param out: t_ffile, the file that serves as stdout.
// @param in: t_ffile, the file that serves as stdin.
// @returns int32_t, the exit status.
int32_t	safe_exec_setup(t_command *cmd)
{
	int32_t	failed;

	if (cmd->outfd == STDOUT_FILENO && cmd->infd == STDIN_FILENO)
		return (0);
	failed = 0;
	if (cmd->infd != STDIN_FILENO)
		failed = dup2(cmd->infd, STDIN_FILENO);
	if (failed < 0)
	{
		if (cmd->outfd >= 0)
			ft_ffclose(&cmd->outfd);
		if (cmd->infd >= 0)
			ft_ffclose(&cmd->infd);
		return (-1);
	}
	if (cmd->outfd != STDOUT_FILENO)
		failed = dup2(cmd->outfd, STDOUT_FILENO);
	if (failed < 0 || cmd->outfd < 0)
		return (-1);
	if (cmd->outfd != STDOUT_FILENO)
		ft_ffclose(&cmd->outfd);
	if (cmd->infd != STDIN_FILENO)
		ft_ffclose(&cmd->infd);
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
void	execute(t_command *cmd, t_command *o, char **env)
{
	const int32_t	exit_code[] = {[ENOENT] = 127, [EACCES] = 126,};
	int32_t			failed;
	char			*path;

	path = NULL;
	failed = false;
	if (!ft_strcmp(cmd->infile, "<<"))
		failed = !exec_here_doc(cmd, *cmd->args, env);
	if (!failed)
		failed = safe_exec_setup(cmd);
	if (failed == 0)
	{
		path = get_path_or_exit(cmd, o, env);
		sig_child();
		if (!ft_strcmp(cmd->infile, "<<"))
			execve(path, cmd->args + 1, env);
		else
			execve(path, cmd->args, env);
		failed = errno;
		perror(cmd->args[0]);
	}
	ft_free_nt_tab(env, ft_nt_tablen((void *)env));
	fail_free(cmd, path, o);
	exit(exit_code[failed]);
}

static void	proceed_to_fork(t_command *command,
	t_cmd_fun builtin, char ***env, t_command *o)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		sig_child();
		if (builtin && has_pipe(o))
		{
			pid = builtin(*command, env);
			ft_free_nt_tab(*env, ft_nt_tablen((void *)*env));
			fail_free(command, NULL, o);
			ft_clear_filelist();
			exit(pid);
		}
		else
		{
			if (command->infd < 0 || command->outfd < 0)
				exit(1);
			execute(command, o, *env);
		}
	}
	else if (pid < 0)
		return ;
	cleanup(command, pid);
}

// @doc exec_single
// @kind func
// @desc Execute a single command/handles builtins.
// @param command: [[t_command]] *, the command to execute.
// @param env: char **, the environment to work off of.
// @returns bool, Was it a built-in?
bool	exec_single(t_command *command, t_command *o, char ***env)
{
	int32_t		pid;
	t_cmd_fun	builtin;

	builtin = get_builtin(command);
	if ((builtin && !has_pipe(o)) || !command->path || !*command->path)
	{
		command->builtin = true;
		pid = 0;
		if (!ft_strcmp("<<", command->infile))
		{
			if (!here_doc_drain(command, *command->args))
				pid = 130;
		}
		else if (command->path && *command->path)
			pid = builtin(*command, env);
		cleanup(command, pid);
		return (true);
	}
	sig_parent();
	proceed_to_fork(command, builtin, env, o);
	return (false);
}
