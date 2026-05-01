#include "minishell.h"

void	entrypoint(t_command **cmds, char **env) {
	t_ffile			wpipe[2];

	while (*cmds)
	{
		(*cmds)->infd = STDIN_FILENO;
		(*cmds)->outfd = STDOUT_FILENO;
		if (ft_strcmp((*cmds)->infile, "stdin") && ft_strcmp((*cmds)->infile, "|"))
			(*cmds)->infd = ft_ffopen((*cmds)->infile, "r");
		if (ft_strcmp((*cmds)->infile, "stdout"))
			(*cmds)->outfd = ft_ffopen((*cmds)->outfile, "w");
		if (!ft_strcmp((*cmds)->outfile, "|"))
		{
			pipe(wpipe);
			(*cmds)->outfd = wpipe[WE];
			(*(cmds + 1))->infd = wpipe[RE];
		}
		exec_single(*cmds, env);
		cmds++;
	}
}
