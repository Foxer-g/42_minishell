/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/07/30 19:18:30 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/14 05:00:45 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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

// @doc setup_here_doc
// @kind func
// @desc Sets up a here doc for a single command
// @param target_pipe: [[t_ffile]][2], The pipe to set up and use.
// @param used_pipe: bool, true if the pipe is used, false if opening it failed.
// @param env: char **, The environment.
// @returns [[t_ffile]], The read end of the pipe.
static void	exec_here_doc(t_command *cmd,
	const char *delimiter, char **env)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strcmp(line, delimiter))
			break ;
		expand_here_doc(&line, &env);
		write(cmd->hd_pipe, line, ft_strlen(line));
		write(cmd->hd_pipe, "\n", 1);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	ft_ffclose(&cmd->hd_pipe);
	get_next_line(-1);
}

char	*get_path_or_exit(t_command *cmd, t_command *o, char **env)
{
	char	**envpath;
	char	*path;
	int32_t	exit_code;

	envpath = ft_split(ft_get_env("PATH", env), ':');
	errno = 0;
	path = ft_find_exec(cmd->path, (void *)envpath);
	if (errno == EINVAL || errno == ENOENT)
	{
		exit_code = 2 + (125 * ((errno == ENOENT) | !ft_strcmp("..", cmd->path)));
		if (exit_code == 127)
			ft_dprintf(2, "%s: Command not found", cmd->path);
		else
			ft_dprintf(2, ".: filename argument required\n"
				".: usage: . [-p path] filename [arguments]");
		ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		fail_free(cmd, path, o);
		exit(exit_code);
	}
	ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
	return (path);
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
	if (!ft_strcmp(cmd->infile, "<<"))
		exec_here_doc(cmd, *cmd->args, env);
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
	if (builtin && !has_pipe(o))
	{
		if (!ft_strcmp("<<", command->infile))
			exec_here_doc(command, *command->args, *env);
		pid = builtin(*command, env);
		command->builtin = true;
		cleanup(command, pid);
		return (true);
	}
	else if (!command->path || !*command->path)
	{
		if (!ft_strcmp("<<", command->infile))
			exec_here_doc(command, *command->args, *env);
		pid = 0;
		command->builtin = true;
		cleanup(command, pid);
		return (true);
	}
	sig_parent();
	proceed_to_fork(command, builtin, env, o);
	return (false);
}
