/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   utils.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/06 08:56:36 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/07 05:49:29 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

void	remove_quotes(t_command *cmd)
{
	char	*tmp;
	char	**args;

	args = cmd->args;
	while (*args)
	{
		if (**args == '\'' || **args == '"')
		{
			tmp = ft_substr(*args, 1, ft_strlen(*args) - 2);
			free(*args);
			*args = tmp;
		}
		args++;
	}
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
