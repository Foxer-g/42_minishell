#include "minishell.h"
#include "parser.h"

intmax_t	args_len(t_command cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd.args[i])
		i++;
	return (i);
}

intmax_t	cmd_len(t_command *cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd[i].infile)
		i++;
	return (i);
}
