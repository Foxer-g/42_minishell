/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_manipulator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 04:06:32 by toespino          #+#    #+#             */
/*   Updated: 2026/08/15 06:15:05 by rboutelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

bool	free_redir(t_redir *redir)
{
	intmax_t	i;

	if (!redir)
		return (false);
	i = 0;
	while (redir[0].file && redir[0].file[i])
		free(redir[0].file[i++]);
	i = 0;
	while (redir[1].file && redir[1].file[i])
		free(redir[1].file[i++]);
	free(redir[0].index);
	free(redir[1].index);
	free(redir[0].file);
	free(redir[1].file);
	free(redir);
	return (true);
}

t_redir	*redir_calloc(intmax_t n, char ***env)
{
	t_redir	*res;

	res = ft_calloc(2, sizeof(t_redir));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	res[0].index = ft_calloc(n, sizeof(intmax_t));
	res[1].index = ft_calloc(n, sizeof(intmax_t));
	res[0].file = ft_calloc(n, sizeof(char *));
	res[1].file = ft_calloc(n, sizeof(char *));
	if (!res[0].index || !res[0].file || !res[1].index || !res[1].file)
	{
		free_redir(res);
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	}
	res[0].index[0] = -1;
	res[1].index[0] = -1;
	return (res);
}

bool	set_infile(t_command *cmd, t_redir *r, char ***env)
{
	intmax_t	i;

	if (r[0].index[0] == -1)
		return (true);
	i = 0;
	while (r[0].index[i + 1] != -1)
		i++;
	free(cmd->infile);
	cmd->infile = ft_strdup(r[0].file[i]);
	if (!cmd->infile)
		return (!parsing_error(MALLOC, "", env));
	return (true);
}

bool	set_outfile(t_command *cmd, t_redir *r, char ***env)
{
	intmax_t	i;

	if (r[1].index[0] == -1)
		return (true);
	if (!remove_quotes((t_command *)r, true))
		return (!parsing_error(MALLOC, "", env));
	i = 0;
	while (r[1].file[i])
		ft_ffopen(r[1].file[i++], "a");
	ft_clear_filelist();
	free(cmd->outfile);
	cmd->outfile = ft_strdup(r[1].file[i - 1]);
	if (!cmd->outfile)
		return (!parsing_error(MALLOC, "", env));
	cmd->append = r[1].type == APPEND;
	return (true);
}
