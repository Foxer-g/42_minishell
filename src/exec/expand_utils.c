/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   expand_utils.c                                      ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42.fr>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/06 08:56:36 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/18 11:27:54 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCE
#include "minishell.h"
#define LEN ft_strlen

static	uintmax_t	eovn(const char *s)
{
	uintmax_t	len;

	if (*s == '?')
		return (1);
	len = 0;
	while (ft_isalnum(s[len]) || s[len] == '_')
		len++;
	return (len);
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
	*ar = ft_recalloc(*ar, LEN(*ar), LEN(*ar) - LEN(evn) + LEN(var), 1);
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
