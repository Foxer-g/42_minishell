/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 07:31:09 by f0xer             #+#    #+#             */
/*   Updated: 2026/08/08 21:15:33 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_pipe(t_command *cmds)
{
	uintmax_t	len;

	len = 0;
	while (cmds->path)
	{
		len++;
		cmds++;
	}
	return (len > 1);
}

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
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		g_sig_handle = SIGINT;
		write(1, "^C\n", 3);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
		g_sig_handle = SIGQUIT;
}

char	*prompt(char **env)
{
	char	*str_prompt;
	char	*out;
	char	*path[2];
	char	*usr;

	str_prompt = ft_strdup(PROMPT_A);
	usr = ft_get_env("USER", env);
	path[0] = ft_strdup(ft_get_env("PWD", env));
	path[1] = path[0];
	if (ft_strlen(path[0]) > (ft_strlen(ft_get_env("HOME", env))))
	{
		path[0] += ft_strlen(ft_get_env("HOME", env)) - 1;
		path[0][0] = '~';
	}
	str_prompt = ft_extend(str_prompt, usr);
	str_prompt = ft_extend(str_prompt, PROMPT_B);
	str_prompt = ft_extend(str_prompt, path[0]);
	str_prompt = ft_extend(str_prompt, PROMPT_C);
	out = readline(str_prompt);
	free(str_prompt);
	free(path[1]);
	return (out);
}


void	test_print(t_command *input)
{
	intmax_t	i;
	intmax_t	j;

	i = 0;
	while (input[i].infile)
	{
		ft_printf("Element : %d\n--------------\n", i);
		ft_printf("Path : %s\nArguments :\n", input[i].path);
		j = 0;
		while (input[i].args[j])
		{
			ft_printf("    - \"%s\"\n", input[i].args[j]);
			j++;
		}
		ft_printf("Pid : %i\n", input[i].pid);
		ft_printf("Infd : %i\n", input[i].infd);
		ft_printf("Outfd : %i\n", input[i].outfd);
		ft_printf("Append : %i\n", input[i].append);
		ft_printf("Infile : %s\n", input[i].infile);
		ft_printf("Outfile : %s\n", input[i].outfile);
		ft_printf("Builtin : %i\n", input[i].builtin);
		i++;
	}
}
/**/
