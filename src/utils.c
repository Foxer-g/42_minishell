/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 07:31:09 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/04 04:57:31 by f0xer            ###   ########.fr       */
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

void	sig_handle(int32_t signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	g_sig_handle = signal;
}

char	*prompt(char **env)
{
	char	*str_prompt;
	char	*out;
	char	*path;
	char	*usr;

	str_prompt = ft_strdup(PROMPT_A);
	usr = get_env("USER=", env);
	path = get_env("PWD=", env);
	if (ft_strlen(path) > (ft_strlen(get_env("HOME=", env))))
	{
		path += ft_strlen(get_env("HOME=", env)) - 1;
		path[0] = '~';
	}
	str_prompt = extend(str_prompt, usr);
	str_prompt = extend(str_prompt, PROMPT_B);
	str_prompt = extend(str_prompt, path);
	str_prompt = extend(str_prompt, PROMPT_C);
	out = readline(str_prompt);
	free(str_prompt);
	return (out);
}

void	test_print(t_command input)
{
	intmax_t i;

	while (input[i].infile)
	{
		ft_printf("Element : %d\n--------------\n", i);
		ft_printf("Path : %s\nArguments :\n", input[i].path);
		j = 0;
		while (input[i].args[j])
		{
			ft_printf("    - %s\n", input[0].args[j]);
			j++;
		}
		ft_printf("Pid : %i\n", input[i].pid);
		ft_printf("Infd : %i\n", input[i].infd);
		ft_printf("Outfd : %i\n", input[i].outfd);
		ft_printf("Append : %i\n", input[i].append);
		ft_printf("Infile : %s\n", input[i].infile);
		ft_printf("Outfile : %s\n", input[i].outfile);
		i++;
	}
}
