/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 03:01:38 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/05 21:54:15 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 03:09:23 by f0xer             #+#    #+#             */
/*   Updated: 2026/07/28 19:25:01 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sig_handle = 0;

void	sig_init(void)
{
	struct sigaction	sig_action;

	sig_action.sa_sigaction = sig_handle;
	sig_action.sa_flags = SA_SIGINFO;
	sigemptyset(&sig_action.sa_mask);
	sigaction(SIGINT, &sig_action, NULL);
	sigaction(SIGQUIT, &sig_action, NULL);
}


void	minishell_action(char *input, char ***env)
{
	t_command	*parsed_input;

	parsed_input = parser(input, env);
	if (!parsed_input)
		return ;
	test_print(parsed_input);
	entrypoint(parsed_input, env);
	free_command(parsed_input);
	free(input);
}

int32_t	main(int32_t ac, char **av, const char **aenv)
{
	char				*input;
	char				**env;

	sig_init();
	env = ft_copy_env(aenv);
	if (ac != 1)
	{
		input = join_tab(++av);
		minishell_action(input, &env);
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
	}
	else
	{
		input = prompt(env);
		while (input)
		{
			if (input[0])
			{
				add_history(input);
				minishell_action(input, &env);
			}
			input = prompt(env);
		}
		rl_clear_history();
		ft_printf("exit\n");
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		return (0);
	}
}
