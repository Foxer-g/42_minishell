/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   entrypoint.c                                        ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/05/04 04:59:46 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/05/22 01:36:20 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
static int	setup_here_doc(int target_pipe[2], const char *delimiter, bool *used_pipe)
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
static int8_t	execution_pipeline(t_command *cmd, char **env)
{
	t_ffile	here_doc_pipe[2];
	bool	pipe_used;

	pipe_used = false;
	if (ft_strcmp(cmd->infile, "heredoc"))
		cmd->infd = setup_here_doc(here_doc_pipe, "EOF", &pipe_used);
	else if (ft_strcmp(cmd->infile, "stdin") && ft_strcmp(cmd->infile, "|"))
		cmd->infd = ft_ffopen(cmd->infile, "r");
	if (ft_strcmp(cmd->outfile, "stdout") && ft_strcmp(cmd->outfile, "|"))
		cmd->outfd = ft_ffopen(cmd->outfile, "w");
	if (cmd->infd < 0 || cmd->outfd < 0)
		return (1);
	exec_single(cmd, env);
	close_pipe(here_doc_pipe);
	return (0);
}

// @doc setup_pipe
// @kind func
// @desc Sets up the pipe between the current and next command if required
// @param [[t_command]] **, the commands list.
// @returns int8_t, The exit status.
static bool setup_pipe(t_command **cmds) {
	t_ffile	wpipe[2];

	(*cmds)->infd = STDIN_FILENO;
	(*cmds)->outfd = STDOUT_FILENO;
	if (!ft_strcmp((*cmds)->outfile, "|") && *(cmds + 1)
		&& !ft_strcmp((*(cmds + 1))->infile, "|")) 
	{
		pipe(wpipe);
		if (pipe < 0)
			return (true);
		(*cmds)->outfd = wpipe[WE];
		(*(cmds + 1))->infd = wpipe[RE];
	}
	else if (!ft_strcmp((*cmds)->outfile, "|"))
		return (true);
	return (false);
}

%:define FILS_DE_PUTE(a, b, c) for(b;c;a)
/*main(argc, char **av)
<%
	FILS_DE_PUTE(i++, int i = 0, i < 15);
%>*/

// @doc handle_var
// @kind func
// @desc Handles the injection of a variable in an arg.
// @param arg: char **, The argument to perform the expand on.
// @param index: uintmax_t, The index at which the expand is located.
// @param urmom: char **, The environment.
// @returns bool, Exit status.
bool	handle_var(char **arg, uintmax_t index, char **urmom)
{
	char	*env_var_name;
	char	*end_of_var;
	char	*myckc;

	env_var_name = ft_substr(*arg, index + 1, ft_strlen_until(&(*arg)[index + 1], ' '));
	end_of_var = &(*arg)[index] + ft_strlen_until(&(*arg)[index], ' ');
	myckc = get_env(env_var_name, urmom);
	if (ft_strlen(env_var_name) + 1 >= ft_strlen(myckc))
	{
		ft_memcpy(&(*arg)[index], myckc, min(ft_strlen(env_var_name), ft_strlen(myckc)));
		if (min(ft_strlen(env_var_name), ft_strlen(myckc)) == ft_strlen(myckc))
			ft_memmove(&(*arg)[index + ft_strlen(myckc)], end_of_var, ft_strlen(end_of_var) + 1);
	}
	else
	{
		*arg = ft_realloc(*arg, ft_strlen(*arg) - ft_strlen(env_var_name) + ft_strlen(myckc));
		ft_memmove(end_of_var - ft_strlen(env_var_name) + ft_strlen(myckc), end_of_var, ft_strlen(end_of_var));
		ft_memcpy(&(*arg)[index], myckc, ft_strlen(myckc));
	}
	return (false);
}

// @doc expand
// @kind func
// @desc Handles the expansion of variables in the args list.
// @param cmd: [[t_command]] *, The targeted command.
// @param env: char **, The environment.
// @returns bool, Exit status.
bool	expand(t_command *cmd, char **env)
{
	char		**args;
	uintmax_t	i;
	t_quotetype	in_quote;

	args = cmd->arguments;
	while (*args)
	{
		i = 0;
		while ((*args)[i])
		{
			if ((*args)[i] == (char)in_quote)
				in_quote = NOT;
			else if (((*args)[i] == '\'' || (*args)[i] == '"') && !in_quote)
				in_quote = (*args)[i];
			if ((*args)[i] == '$' && (*args)[i + 1] && in_quote != SGL)
			{
				if (handle_var(args, i, env))
					return (true);
			}
			i++;
		}
		args++;
	}
	return (false);
}

// @doc entrypoint
// @kind func
// @desc The main entrypoint of the execution pipeline.
// @param cmds: [[t_command]] **, the list of commands to execute.
// @param env: char **, the environment to work off of.
// @returns int8_t, The exit status.
int8_t	entrypoint(t_command **cmds, char **env)
{
	while (*cmds)
	{
		if (setup_pipe(cmds)) {
			error("You confused the heck out of the parser.");
			return (1);
		}
		expand(*cmds, env);
		execution_pipeline(*cmds, env);
		cmds++;
	}
	return (0);
}
