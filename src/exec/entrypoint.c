/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   entrypoint.c                                        ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo rboutelo@student.42angouleme.fr        ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/04/26 01:31:07 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/08 04:47:50 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

// @doc setup_here_doc
// @kind func
// @desc Sets up a here doc for a single command
// @param target_pipe: [[t_ffile]][2], The pipe to set up and use.
// @param used_pipe: bool, true if the pipe is used, false if opening it failed.
// @param env: char **, The environment.
// @returns [[t_ffile]], The read end of the pipe.
static int	setup_here_doc(t_command *cmd,
	t_ffile target_pipe[2], bool *used_pipe)
{
	if (pipe(target_pipe) < 0)
		return (-1);
	*used_pipe = true;
	cmd->infd = target_pipe[RE];
	cmd->hd_pipe = target_pipe[WE];
	return (target_pipe[0]);
}

// @doc execution_pipeline
// @kind func
// @desc Internal entrypoint that's only accessed through [[entrypoint]].
// @param cmd: [[t_command]] *, The command to execute.
// @param env: char **, The environment to use for the command.
// @returns int8_t, The exit status.
static int8_t	execution_pipeline(t_command *cmd, t_command *o, char ***env)
{
	t_ffile	here_doc_pipe[2];
	bool	pipe_used;
	bool	status;

	pipe_used = false;
	if (!ft_strcmp(cmd->infile, "<<"))
		setup_here_doc(cmd, here_doc_pipe, &pipe_used);
	else if (ft_strcmp(cmd->infile, "stdin"))
		cmd->infd = ft_ffopen(cmd->infile, "r");
	if (ft_strcmp(cmd->outfile, "stdout") && !cmd->append)
		cmd->outfd = ft_ffopen(cmd->outfile, "w");
	else if (ft_strcmp(cmd->outfile, "stdout"))
		cmd->outfd = ft_ffopen(cmd->outfile, "a");
	if (cmd->infd < 0 || cmd->outfd < 0)
		return (false);
	status = exec_single(cmd, o, env);
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
		first->outpipe_end = wpipe[RE];
		(first + 1)->infd = wpipe[RE];
		first++;
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
	if (!setup_pipe(cmds))
		return (-256);
	while (cmds->infile)
	{
		remove_quotes(cmds);
		execution_pipeline(cmds++, cmdsc, env);
	}
	status = 0;
	while (cmdsc->infile)
	{
		if (!cmdsc[0].builtin)
 			waitpid(cmdsc[0].pid, &status, 0);
		else
			status = cmdsc[0].pid;
		cmdsc++;
	}
	sig_init();
	ft_set_exit_code(status, env);
	return (status);
}
