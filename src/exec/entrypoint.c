/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   entrypoint.c                                        ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo rboutelo@student.42angouleme.fr        ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/04/26 01:31:07 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/05 23:16:23 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

// @doc setup_here_doc
// @kind func
// @desc Sets up a here doc for a single command
// @param target_pipe: [[t_ffile]][2], The pipe to set up and use.
// @param used_pipe: bool, true if the pipe is used, false if opening it failed.
// @returns [[t_ffile]], The read end of the pipe.
static int	setup_here_doc(t_ffile target_pipe[2], const char *delimiter,
	bool *used_pipe)
{
	char	*line;

	if (pipe(target_pipe) < 0)
		return (-1);
	*used_pipe = true;
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strcmp(line, delimiter) || *line == EOF)
			break ;
		line[ft_strlen(line)] = '\n';
		write(target_pipe[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(target_pipe[1]);
	return (target_pipe[0]);
}

// @doc execution_pipeline
// @kind func
// @desc Internal entrypoint that's only accessed through [[entrypoint]].
// @param cmd: [[t_command]] *, The command to execute.
// @param env: char **, The environment to use for the command.
// @returns int8_t, The exit status.
static int8_t	execution_pipeline(t_command *cmd, char ***env)
{
	t_ffile	here_doc_pipe[2];
	bool	pipe_used;
	bool	status;

	pipe_used = false;
	if (!ft_strcmp(cmd->infile, "heredoc"))
		cmd->infd = setup_here_doc(here_doc_pipe, "EOF", &pipe_used);
	else if (ft_strcmp(cmd->infile, "stdin"))
		cmd->infd = ft_ffopen(cmd->infile, "r");
	if (ft_strcmp(cmd->outfile, "stdout"))
		cmd->outfd = ft_ffopen(cmd->outfile, "w");
	if (cmd->infd < 0 || cmd->outfd < 0)
		return (false);
	status = exec_single(cmd, env);
	if (pipe_used)
		ft_close_pipe(here_doc_pipe);
	return (status);
}

// @doc setup_pipe
// @kind func
// @desc Sets up the pipe between the current and next command if required
// @param [[t_command]] **, the commands list.
// @returns bool, The exit status.
static bool	setup_pipe(t_command *cmds)
{
	uintmax_t	to_resolve;
	t_ffile		wpipe[2];
	t_command	*first;

	first = cmds;
	to_resolve = 0;
	while (cmds && cmds->path && cmds++)
		to_resolve++;
	if (to_resolve < 2)
		return (true);
	while (--to_resolve)
	{
		if (pipe(wpipe) < 0)
		{
			perror("pipe");
			return (false);
		}
		first->outfd = wpipe[WE];
		(first + 1)->infd = wpipe[RE];
		first++;
	}
	return (true);
}

// @doc handle_var
// @kind func
// @desc Handles the injection of a variable in an arg.
// @param arg: char **, The argument to perform the expand on.
// @param index: uintmax_t, The index at which the expand is located.
// @param env: char **, The environment.
// @returns bool, Exit status.
bool	handle_var(char **arg, uintmax_t ind, char **env)
{
	char	*evn;
	char	*eov;
	char	*var;

	evn = ft_substr(*arg, ind + 1, ft_strlen_until(&(*arg)[ind + 1], ' '));
	if (!evn)
		return (true);
	eov = &(*arg)[ind] + ft_strlen_until(&(*arg)[ind], ' ');
	var = ft_get_env(evn, env);
	if (ft_strlen(evn) + 1 >= ft_strlen(var))
	{
		ft_memcpy(&(*arg)[ind], var, ft_min(ft_strlen(evn), ft_strlen(var)));
		if (ft_min(ft_strlen(evn), ft_strlen(var)) == ft_strlen(var))
			ft_memmove(&(*arg)[ind + ft_strlen(var)], eov, ft_strlen(eov) + 1);
		free(evn);
		return (false);
	}
	*arg = ft_recalloc(*arg, ft_strlen(*arg), ft_strlen(*arg)
		 - ft_strlen(evn) + ft_strlen(var), sizeof(char));
	if (!*arg)
		return (true);
	eov = &(*arg)[ind] + ft_strlen_until(&(*arg)[ind], ' ');
	ft_memmove(eov - ft_strlen(evn) + ft_strlen(var), eov, ft_strlen(eov));
	ft_memcpy(&(*arg)[ind], var, ft_strlen(var));
	free(evn);
	return (false);
}

// @doc expand
// @kind func
// @desc Handles the expansion of variables in the args list.
// @param cmd: [[t_command]] *, The targeted command.
// @param env: char **, The environment.
// @returns bool, Exit status.
bool	expand(t_command *cmd, char ***env)
{
	char		**args;
	uintmax_t	i;
	t_quotetype	in_quote;

	in_quote = NOT;
	args = cmd->args;
	while (*args)
	{
		i = 0;
		while (*args && (*args)[i])
		{
			if ((*args)[i] == (char)in_quote)
				in_quote = NOT;
			else if (((*args)[i] == '\'' || (*args)[i] == '"') && !in_quote)
				in_quote = (*args)[i];
			if ((*args)[i] == '$' && (*args)[i + 1] && in_quote != SGL)
			{
				if (handle_var(args, i, *env))
					return (!parsing_error(MALLOC, "", env));
			}
			i++;
		}
		args++;
	}
	return (true);
}

// @doc entrypoint
// @kind func
// @desc The main entrypoint of the execution pipeline.
// @param cmds: [[t_command]] **, the list of commands to execute.
// @param env: char ***, the environment to work off of.
// @returns int8_t, The exit status.
int32_t	entrypoint(t_command *cmds, char ***env)
{
	t_command	*cmdsc;
	int32_t		status;

	cmdsc = cmds;
	while (cmds)
	{
		if (!setup_pipe(cmds))
		{
			error("You confused the heck out of the parser.");
			return (1);
		}
		execution_pipeline(cmds++, env);
	}
	while (cmdsc)
	{
		if (!cmds[0].builtin)
			waitpid(cmds[0].pid, &status, 0);
		else
			status = cmds[0].pid;
		cmdsc++;
	}
	ft_set_exit_code(status, env);
	return (status);
}
