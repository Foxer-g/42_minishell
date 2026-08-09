#include "minishell.h"
#include "parser.h"

// @doc args_len
// @kind func
// @desc Count the arguments of a [[t_command]].
// @param cmd: [[t_command]], Command to process.
// @return intmax_t, Arguments count.

intmax_t	args_len(t_command cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd.args[i])
		i++;
	return (i);
}

// @doc cmd_len
// @kind func
// @desc Get length of a command array.
// @param cmd: [[t_command]] *, Command array to process.
// @return intmax_t, Command array length.

intmax_t	cmd_len(t_command *cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd[i].infile)
		i++;
	return (i);
}
