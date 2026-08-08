/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   utils.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/06 08:56:36 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/08 08:17:51 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"
#define LEN ft_strlen

void	remove_quotes(t_command *cmd)
{
	char	*tmp;
	char	**args;

	args = cmd->args;
	while (*args)
	{
		if ((**args == '\'' || **args == '"') && (*args)[ft_strlen(*args) - 1] == **args)
		{
			tmp = ft_substr(*args, 1, ft_strlen(*args) - 2);
			free(*args);
			*args = tmp;
		}
		args++;
	}
}

static	uintmax_t	eovn(const char *s)
{
	return (
		ft_min(
			ft_min(
				ft_strlen_until(s, '\''),
				ft_strlen_until(s, '"')
			),
			ft_min(
				ft_strlen_until(s, ' '),
				ft_strlen_until(s, '$')
			)
		)
	);
}

// @doc handle_var
// @kind func
// @desc Handles the injection of a variable in an arg.
// @param arg: char **, The argument to perform the expand on.
// @param index: uintmax_t, The index at which the expand is located.
// @param env: char **, The environment.
// @returns bool, Exit status.
intmax_t	handle_var(char **ar, uintmax_t ind, char **env)
{
	char		*evn;
	char		*eov;
	char		*var;
	intmax_t	inserted;

	evn = ft_substr(*ar, ind + 1, eovn(&(*ar)[ind + 1]));
	if (!evn)
		return (-1);
	eov = &(*ar)[ind + 1] + eovn(&(*ar)[ind + 1]);
	var = ft_get_env(evn, env);
	inserted = (intmax_t)ft_strlen(var);
	if (ft_strlen(evn) + 1 >= ft_strlen(var))
	{
		ft_memcpy(&(*ar)[ind], var, ft_strlen(var));
		if (ft_min(ft_strlen(evn), ft_strlen(var)) == ft_strlen(var))
			ft_memmove(&(*ar)[ind + ft_strlen(var)], eov, ft_strlen(eov) + 1);
		free(evn);
		return (inserted);
	}
	*ar = ft_recalloc(*ar, LEN(*ar), LEN(*ar) - LEN(evn) + LEN(var), 1);
	if (!*ar)
		return (-1);
	eov = &(*ar)[ind + 1] + eovn(&(*ar)[ind + 1]);
	ft_memmove(eov - (LEN(evn) + 1) + LEN(var), eov, LEN(eov) + 1);
	ft_memcpy(&(*ar)[ind], var, ft_strlen(var));
	free(evn);
	return (inserted);
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
	intmax_t	n;

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
				n = handle_var(args, i, *env);
				if (n < 0)
					return (!parsing_error(MALLOC, "", env));
				i += (uintmax_t)n;
				continue ;
			}
			i++;
		}
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
