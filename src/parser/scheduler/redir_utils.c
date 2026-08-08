/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:08 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:56:31 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

bool	strtrim_cmd_end(t_command *cmd, char ***env)
{
	char		**tmp;
	intmax_t	i;

	tmp = ft_calloc(args_len(*cmd) + 1, sizeof(char *));
	if (!tmp)
		return (!parsing_error(MALLOC, "", env));
	i = 0;
	while ((*cmd).args[i])
	{
		tmp[i] = ft_strtrim((*cmd).args[i], " ");
		if (!tmp[i])
		{
			ft_free_nt_tab(tmp, i);
			return (!parsing_error(MALLOC, "", env));
		}
		i++;
	}
	command_exec_set(cmd, tmp, args_len(*cmd));
	ft_free_nt_tab(tmp, args_len(*cmd));
	return (true);
}

intmax_t	args_len(t_command cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd.args[i])
		i++;
	return (i);
}

static int32_t	is_redir(char c1, char c2)
{
	if (!c1)
		return (0);
	if (!c2)
		return (((c1 == '<') * I_REDIR) + ((c1 == '>') * O_REDIR));
	if (c1 != c2 && (c1 == '<' || c1 == '>'))
		return (((c1 == '<') * I_REDIR) + ((c1 == '>') * O_REDIR));
	else if (c1 != c2)
		return (0);
	if (c1 == c2 && c1 == '>')
		return (APPEND);
	return (0);
}

char	**cmddup_without_redir(t_command cmd, t_redir *redir, char ***env)
{
	char		**res;
	intmax_t	i;
	intmax_t	j;

	i = -1;
	j = 0;
	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	while (cmd.args[++i])
	{
		if (!((redir[0].index && (i == redir[0].index
						|| i == redir[0].index + 1)) || ((redir[1].index
						&& (i == redir[1].index || i == redir[1].index + 1)))))
		{
			res[j] = ft_strdup(cmd.args[i]);
			if (!res[j++])
			{
				ft_free_nt_tab(res, j);
				return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
			}
		}
	}
	return (res);
}

t_redir	*find_redir(t_command cmd, char ***env)
{
	t_redir		*redir;
	intmax_t	i;
	int32_t		type;

	redir = ft_calloc(2, sizeof(t_redir));
	if (!redir)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	i = 0;
	while (cmd.args[i++])
	{
		type = is_redir(cmd.args[i - 1][0], cmd.args[i - 1][1]);
		if (type)
		{
			if (i - 1 == 0 || !cmd.args[i - 1] || redir[type != I_REDIR].type)
			{
				free(redir);
				parsing_error(PARSING, cmd.args[i - 1], env);
				return (NULL);
			}
			redir[type != I_REDIR].type = type;
			redir[type != I_REDIR].index = (++i) - 2;
			redir[type != I_REDIR].file = cmd.args[i - 1];
		}
	}
	return (redir);
}
