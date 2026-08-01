/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   entrypoint.c                                        ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo rboutelo@student.42angouleme.fr        ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/04/26 01:31:07 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/07/30 19:32:04 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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

	pipe_used = false;
	if (ft_strcmp(cmd->infile, "heredoc"))
		cmd->infd = setup_here_doc(here_doc_pipe, "EOF", &pipe_used);
	else if (ft_strcmp(cmd->infile, "stdin") && ft_strcmp(cmd->infile, "|"))
		cmd->infd = ft_ffopen(cmd->infile, "r");
	if (ft_strcmp(cmd->outfile, "stdout") && ft_strcmp(cmd->outfile, "|"))
		cmd->outfd = ft_ffopen(cmd->outfile, "w");
	if (cmd->infd < 0 || cmd->outfd < 0)
		return (1);
	exec_single(cmd, *env);
	close_pipe(here_doc_pipe);
	return (0);
}

// @doc setup_pipe
// @kind func
// @desc Sets up the pipe between the current and next command if required
// @param [[t_command]] ***, the commands list.
// @returns bool, The exit status.
static bool	setup_pipe(t_command ***cmds)
{
	uintmax_t	to_resolve;
	t_ffile		wpipe[2];
	t_command	**current;
	int32_t		error;

	to_resolve = 0;
	while (**cmds && !ft_strcmp((**cmds)->path, "|") && *(*cmds)++)
		to_resolve++;
	current = *cmds;
	error = 0;
	while (to_resolve--)
	{
		error = pipe(wpipe);
		if (error < 0)
		{
			perror("pipe");
			break ;
		}
		(*current)->outfd = wpipe[WE];
		(*(current + 1))->infd = wpipe[RE];
		current++;
	}
	if (error)
		return (false);
	return (true);
}

// @doc handle_var
// @kind func
// @desc Handles the injection of a variable in an arg.
// @param arg: char **, The argument to perform the expand on.
// @param index: uintmax_t, The index at which the expand is located.
// @param env: char **, The environment.
// @returns bool, Exit status.
bool	handle_var(char **arg, uintmax_t index, char **env)
{
	char	*env_var_name;
	char	*end_of_var;
	char	*var;

	env_var_name = ft_substr(*arg, index + 1, ft_strlen_until(
				&(*arg)[index + 1], ' '));
	end_of_var = &(*arg)[index] + ft_strlen_until(&(*arg)[index], ' ');
	var = get_env(env_var_name, env);
	if (ft_strlen(env_var_name) + 1 >= ft_strlen(var))
	{
		ft_memcpy(&(*arg)[index], var, min(ft_strlen(env_var_name), ft_strlen(
					var)));
		if (min(ft_strlen(env_var_name), ft_strlen(var)) == ft_strlen(var))
			ft_memmove(&(*arg)[index + ft_strlen(var)], end_of_var, ft_strlen(
					end_of_var) + 1);
		free(env_var_name);
		return (false);
	}
	*arg = ft_realloc(*arg, ft_strlen(*arg) - ft_strlen(env_var_name)
			+ ft_strlen(var));
	ft_memmove(end_of_var - ft_strlen(env_var_name) + ft_strlen(var),
		end_of_var, ft_strlen(end_of_var));
	ft_memcpy(&(*arg)[index], var, ft_strlen(var));
	free(env_var_name);
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

	in_quote = NOT;
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
				handle_var(args, i, env);
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
// @param env: char ***, the environment to work off of.
// @param early_stop: bool, if it should stop after the first command.
// @returns int8_t, The exit status.
int8_t	entrypoint(t_command **cmds, char ***env, bool early_stop)
{
	bool	op;

	while (*cmds)
	{
		if (setup_pipe(&cmds))
		{
			error("You confused the heck out of the parser.");
			return (1);
		}
		op = !ft_strcmp((*cmds)->path, "||");
		if (op || !ft_strcmp((*cmds)->path, "&&"))
		{
			entrypoint((void *)(*cmds)->infile, env, true);
			if (op && ft_strcmp(get_env("$?", *env), "0"))
				entrypoint((void *)(*cmds)->outfile, env, true);
			else if (!op && !ft_strcmp(get_env("$?", *env), "0"))
				entrypoint((void *)(*cmds)->outfile, env, true);
			continue ;
		}
		expand(*cmds, *env);
		execution_pipeline(*cmds++, env);
		if (early_stop)
			return (0);
	}
	return (0);
}
