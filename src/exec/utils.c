/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   utils.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/06 08:56:36 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/10 07:02:16 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"
#define LEN ft_strlen

static void	remove_quote_string(char *string, uintmax_t i[2], char *dst)
{
	t_quotetype quote;

	quote = 0;
	while (string[i[0]])
	{
		if (quote)
		{
			if (string[i[0]] == ((char)quote))
				quote = 0;
			else
				dst[i[1]++] = string[i[0]];
		}
		else if (string[i[0]] == SGL || string[i[0]] == DBL)
			quote = string[i[0]];
		else
			dst[i[1]++] = string[i[0]];
		i[0]++;
	}
}

static void helperTheMeeCooY(char **s, char *d)
{
	uintmax_t	i[2];

	ft_bzero(i, sizeof(i));
	remove_quote_string(*s, i, d);
	free(*s);
	*s = d;
}

bool	remove_quotes(t_command *cmd, bool mode)
{
	char		*dst;
	char		**args;

	if (mode)
	{
		args = cmd->args;
		while (*args)
		{
			dst = ft_calloc(ft_strlen(*args) + 1, sizeof(char));
			if (!dst)
				return (false);
			helperTheMeeCooY(args, dst);
			args++;
		}
		return (true);
	}
	dst = ft_calloc(ft_strlen(cmd->infile), sizeof(char));
	helperTheMeeCooY(&cmd->infile, dst);
	dst = ft_calloc(ft_strlen(cmd->outfile), sizeof(char));
	helperTheMeeCooY(&cmd->outfile, dst);
	return (true);
}

static	uintmax_t	eovn(const char *s)
{
	if (*s == '?')
		return (1);
	return (
		ft_min(
			ft_min(
				ft_min(
					ft_strlen_until(s, '\''),
					ft_strlen_until(s, '"')
				),
				ft_strlen_until(s, '=')
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

static bool	drop_dollar_quote(char **arg, uintmax_t i, t_quotetype in_quote)
{
	char	next;

	next = (*arg)[i + 1];
	if (in_quote != NOT || (next != '\'' && next != '"'))
		return (false);
	ft_memmove(&(*arg)[i], &(*arg)[i + 1], ft_strlen(&(*arg)[i + 1]) + 1);
	return (true);
}

static intmax_t	splice_var(char **ar, uintmax_t ind, char *evn, char *var)
{
	char	*eov;

	eov = &(*ar)[ind + 1] + ft_strlen(evn);
	if (ft_strlen(evn) + 1 >= ft_strlen(var))
	{
		ft_memcpy(&(*ar)[ind], var, ft_strlen(var));
		if (ft_strlen(var) <= ft_strlen(evn))
			ft_memmove(&(*ar)[ind + ft_strlen(var)], eov, ft_strlen(eov) + 1);
		return ((intmax_t)ft_strlen(var));
	}
	*ar = ft_recalloc(*ar, LEN(*ar), LEN(*ar) - LEN(evn) - 1 + LEN(var), 1);
	if (!*ar)
		return (-1);
	eov = &(*ar)[ind + 1] + ft_strlen(evn);
	ft_memmove(&(*ar)[ind] + ft_strlen(var), eov, ft_strlen(eov) + 1);
	ft_memcpy(&(*ar)[ind], var, ft_strlen(var));
	return ((intmax_t)ft_strlen(var));
}

intmax_t	handle_var(char **ar, uintmax_t ind, char **env)
{
	char		*evn;
	char		*var;
	intmax_t	inserted;
	uintmax_t	nlen;

	nlen = eovn(&(*ar)[ind + 1]);
	if (nlen == 0)
		return (1);
	evn = ft_substr(*ar, ind + 1, nlen);
	if (!evn)
		return (-1);
	var = ft_get_env(evn, env);
	if (!var)
		var = "";
	inserted = splice_var(ar, ind, evn, var);
	free(evn);
	return (inserted);
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
		if (track_quote((*arg)[i], in_quote) && (*arg)[i] == '$' && (*arg)[i + 1])
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
/*intmax_t	handle_var(char **ar, uintmax_t ind, char **env)
{
	char		*evn;
	char		*eov;
	char		*var;
	intmax_t	inserted;

	if (!ft_strncmp(*ar, "$?", 2))
		evn = ft_strdup("?");
	else
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
}*/

// @doc expand
// @kind func
// @desc Handles the expansion of variables in the args list.
// @param cmd: [[t_command]] *, The targeted command.
// @param env: char **, The environment.
// @returns bool, Exit status.
/*bool	expand(t_command *cmd, char ***env)
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
}*/

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
