/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   expand_core.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/12 06:15:56 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 06:18:16 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

static bool	drop_dollar_quote(char **arg, uintmax_t i, t_quotetype in_quote)
{
	char	next;

	next = (*arg)[i + 1];
	if (in_quote != NOT || (next != '\'' && next != '"'))
		return (false);
	ft_memmove(&(*arg)[i], &(*arg)[i + 1], ft_strlen(&(*arg)[i + 1]) + 1);
	return (true);
}

static bool	track_quote(char c, t_quotetype *in_quote)
{
	if (c == (char)*in_quote)
		*in_quote = NOT;
	else if ((c == '\'' || c == '"') && !*in_quote)
		*in_quote = c;
	return (*in_quote != SGL);
}

static bool	scan_arg(char **arg, char ***env, t_quotetype *in_quote)
{
	uintmax_t	i;
	intmax_t	n;

	i = 0;
	while ((*arg)[i])
	{
		if (track_quote((*arg)[i], in_quote)
			&& (*arg)[i] == '$' && (*arg)[i + 1])
		{
			if (drop_dollar_quote(arg, i, *in_quote))
				continue ;
			n = handle_var(arg, i, *env);
			if (n < 0)
				return (!parsing_error(MALLOC, "", env));
			i += (uintmax_t)n;
			continue ;
		}
		i++;
	}
	return (true);
}

bool	expand(t_command *cmd, char ***env)
{
	char		**args;
	t_quotetype	in_quote;

	in_quote = NOT;
	args = cmd->args;
	while (*args)
	{
		if (!scan_arg(args, env, &in_quote))
			return (false);
		args++;
	}
	return (true);
}

// @doc expand
// @kind func
// @desc Handles the expansion of variables in the args list.
// @param cmd: [[t_command]] *, The targeted command.
// @param env: char **, The environment.
// @returns bool, Exit status.
bool	expand_here_doc(char **line, char ***env)
{
	uintmax_t	i;

	i = 0;
	while (*line && (*line)[i])
	{
		if ((*line)[i] == '$' && (*line)[i + 1])
		{
			if (handle_var(line, i, *env))
				return (!parsing_error(MALLOC, "", env));
		}
		i++;
	}
	return (true);
}
