/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   entrypoint.c                                                  ___        */
/*                                                            _ \'-_,#        */
/*   By: neumann </var/spool/mail/neumann>                   _\'--','`|       */
/*                                                           \`---`  /        */
/*   Created: 2026/05/04 04:59:46 by neumann                  `----'`         */
/*   Updated: 2026/05/12 06:58:55 by neumann                                  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// @doc entrypoint
// @kind func
// @desc The main entrypoint of the execution pipeline.
// @param cmds: command **, the list of commands to execute.
// @param env: char **, the environment to work off of.
void	entrypoint(t_command **cmds, char **env)
{
	t_ffile			wpipe[2];

	while (*cmds)
	{
		(*cmds)->infd = STDIN_FILENO;
		(*cmds)->outfd = STDOUT_FILENO;
		if (ft_strcmp((*cmds)->infile, "stdin")
			&& ft_strcmp((*cmds)->infile, "|"))
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
