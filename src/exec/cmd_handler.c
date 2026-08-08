/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 03:01:38 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/08 04:53:34 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/07/30 19:18:30 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/05 21:23:06 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
	failed = -1;
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
	close(cmd->hd_pipe);
	get_next_line(-1);
}

void	fail_free(t_command *cmd, char *path, t_command *o)
{
	ft_ffclose(&cmd->outpipe_end);
	free(path);
	free_command(o);
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
	int32_t	failed;
	char	*path;
	char	**envpath;

	path = NULL;
	if (!ft_strcmp(cmd->infile, "<<"))
		exec_here_doc(cmd, *cmd->args, env);
	failed = safe_exec_setup(cmd);
	if (failed == 0)
	{
		envpath = ft_split(ft_get_env("PATH", env), ':');
		path = ft_find_exec(cmd->path, (void *)envpath);
		ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
		if (!ft_strcmp(cmd->infile, "<<"))
			execve(path, cmd->args + 1, env);
		else
			execve(path, cmd->args, env);
		failed = errno;
		perror(cmd->args[0]);
	}
	ft_free_nt_tab(env, ft_nt_tablen((void *)env));
	fail_free(cmd, path, o);
	ft_clear_filelist();
	exit(127 * (failed == ENOENT) | 1);
}

/*
Relic of the old code, indexed using the third character to differentiate
builtins.
t_cmd_fun	get_func(enum e_builtin builtin)
{
	const t_cmd_fun funcs[] = {
	[CD] = cd,
	[PWD] = pwd,
	[ECHO] = echo,
	[EXIT] = minishell_exit,
	[EXPORT] = export,
	[UNSET] = unset,
	[ENV] = minishell_env,
	};

	return (funcs[builtin]);
}
*/

static void	cleanup(t_command *command, pid_t pid)
{
	command->pid = pid;
	if (command->hd_pipe != 0)
		ft_ffclose(&command->hd_pipe);
	if (command->infd != STDIN_FILENO)
		ft_ffclose(&command->infd);
	if (command->outfd != STDOUT_FILENO)
		ft_ffclose(&command->outfd);
}

// @doc exec_single
// @kind func
// @desc Execute a single command/handles builtins.
// @param command: [[t_command]] *, the command to execute.
// @param env: char **, the environment to work off of.
// @returns bool, Was it a built-in?
bool	exec_single(t_command *command, t_command *o, char ***env)
{
	const char		*blts[] = {"cd", "echo", "env", "exit",
		"export", "pwd", "unset", NULL};
	const t_cmd_fun	funcs[] = {cd, echo, minishell_env,
		minishell_exit, export, pwd, unset, NULL};
	uint8_t			i;
	int32_t			pid;

	i = -1;
	while (blts[++i])
	{
		if (!ft_strcmp(blts[i], command->path))
		{
			pid = funcs[i](*command, env);
			command->builtin = true;
			cleanup(command, pid);
			return (true);
		}
	}
	pid = fork();
	if (!pid)
		execute(command, o, *env);
	else if (pid < 0)
		return (false);
	cleanup(command, pid);
	return (false);
}
