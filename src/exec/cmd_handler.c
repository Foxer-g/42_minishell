/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 03:01:38 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/08 01:11:08 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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

// @doc setup_here_doc
// @kind func
// @desc Sets up a here doc for a single command
// @param target_pipe: [[t_ffile]][2], The pipe to set up and use.
// @param used_pipe: bool, true if the pipe is used, false if opening it failed.
// @param env: char **, The environment.
// @returns [[t_ffile]], The read end of the pipe.
static void	exec_here_doc(t_command cmd,
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
		write(cmd.append, line, ft_strlen(line));
		write(cmd.append, "\n", 1);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(cmd.append);
	get_next_line(-1);
}

// @doc execute
// @kind func
// @desc Ececute a prepared command with a specific stdin and stdout file.
// @param cmd: [[t_command]], the command to execute.
// @param out: t_ffile, the file that serves as stdout.
// @param in: t_ffile, the file that serves as stdin.
// @param env: char **, the environment that will be passed to execve.
// @returns int8_t, exit status.
void	execute(t_command cmd, t_ffile out, t_ffile in, char **env)
{
	int32_t	failed;
	char	*path;
	char	**envpath;

	if (!ft_strcmp(cmd.infile, "<<"))
		exec_here_doc(cmd, *cmd.args, env);
	failed = safe_exec_setup(out, in);
	if (failed == 0)
	{
		envpath = ft_split(ft_get_env("PATH", env), ':');
		path = ft_find_exec(cmd.path, (void *)envpath);
		ft_free_nt_tab(envpath, ft_nt_tablen((void *)envpath));
		if (!ft_strcmp(cmd.infile, "<<"))
			execve(path, cmd.args + 1, env);
		else
			execve(path, cmd.args, env);
		failed = errno;
		perror(cmd.args[0]);
	}
	free(cmd.path);
	ft_free_nt_tab(cmd.args, ft_nt_tablen((void *)cmd.args));
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
	if (command->append != 0)
		ft_ffclose(command->append);
	if (command->infd != STDIN_FILENO)
		ft_ffclose(command->infd);
	if (command->outfd != STDOUT_FILENO)
		ft_ffclose(command->outfd);
}

// @doc exec_single
// @kind func
// @desc Execute a single command/handles builtins.
// @param command: [[t_command]] *, the command to execute.
// @param env: char **, the environment to work off of.
// @returns bool, Was it a built-in?
bool	exec_single(t_command *command, char ***env)
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
		execute(*command, command->outfd, command->infd, *env);
	else if (pid < 0)
		return (false);
	cleanup(command, pid);
	return (false);
}
