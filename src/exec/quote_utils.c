/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   quote_utils.c                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/12 06:09:34 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 06:10:04 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"

static void	remove_quote_string(char *string, uintmax_t i[2], char *dst)
{
	t_quotetype	quote;

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

void	remove_quotes_inplace(char **s, char *d)
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

	if (!cmd->args)
		return (true);
	if (mode)
	{
		args = cmd->args;
		while (*args)
		{
			dst = ft_calloc(ft_strlen(*args) + 1, sizeof(char));
			if (!dst)
				return (false);
			remove_quotes_inplace(args, dst);
			args++;
		}
		return (true);
	}
	dst = ft_calloc(ft_strlen(cmd->infile) + 1, sizeof(char));
	remove_quotes_inplace(&cmd->infile, dst);
	dst = ft_calloc(ft_strlen(cmd->outfile) + 1, sizeof(char));
	remove_quotes_inplace(&cmd->outfile, dst);
	return (true);
}
