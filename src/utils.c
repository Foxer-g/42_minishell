/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 07:31:09 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/02 08:40:23 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_tab(char **tab)
{
	char	*res;
	char	*temp;
	int32_t	i;

	res = ft_strdup(tab[0]);
	i = 1;
	while (tab[i])
	{
		temp = ft_strjoin(res, " ");
		free(res);
		res = ft_strjoin(temp, tab[i]);
		free(temp);
		i++;
	}
	return (res);
}

char    *prompt(char **aenv)
{
        char    *str_prompt;
        char    *out;
        char    *path;
        char    *usr;

        str_prompt = ft_strdup(PROMPT_A);
        usr = get_env("USER=", aenv);
        path = get_env("PWD=", aenv);
        if (ft_strlen(path) > (ft_strlen(get_env("HOME=", aenv))))
        {
                path += ft_strlen(get_env("HOME=", aenv)) - 1;
                path[0] = '~';
        }
//        str_prompt = extend(str_prompt, PROMPT_A);
        str_prompt = extend(str_prompt, usr);
        str_prompt = extend(str_prompt, PROMPT_B);
        str_prompt = extend(str_prompt, path);
        str_prompt = extend(str_prompt, PROMPT_C);
//        str_prompt = extend(str_prompt, "\x02");
        out = readline(str_prompt);
        free(str_prompt);
        return (out);
}
