/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 23:50:19 by toespino          #+#    #+#             */
/*   Updated: 2026/08/07 23:58:45 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_command	*is_error(t_command *cmd, intmax_t len, char ***env)
{
	intmax_t	i;

	i = 0;
	while (i < len)
	{
		if (!cmd[i].infile)
		{
			free_command(cmd);
			parsing_error(MALLOC, "", env);
			return (NULL);
		}
		i++;
	}
	return (cmd);
}

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
