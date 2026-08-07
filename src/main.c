
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 03:01:38 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/07 03:42:15 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
	rl_catch_signals = 0;
}

int32_t	minishell_action(char *input, char ***env)
{
	t_command	*parsed_input;
	int32_t		res;

	parsed_input = parser(input, env);
	if (!parsed_input)
	{
		free(input);
		return (-1);
	}
	test_print(parsed_input);
	ft_printf("\n\e[32m========================================================\e[0m\n\n");
	res = entrypoint(parsed_input, env);
	ft_printf("\n\e[35m========================================================\e[0m\n\n");
	test_print(parsed_input);
	free_command(parsed_input);
	free(input);
	return (res);
}

static int32_t	main_exit(char **env, int32_t res)
{
	rl_clear_history();
	ft_printf("exit\n");
	ft_free_nt_tab(env, ft_nt_tablen((void *)env));
	return (res);
}

int32_t	main(int32_t ac, char **av, const char **aenv)
{
	char	*input;
	char	**env;
	int32_t	res;

	sig_init();
	env = ft_copy_env(aenv);
	res = 0;
	if (ac != 1)
	{
		minishell_action(join_tab(++av), &env);
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		return (0);
	}
	input = prompt(env);
	while (input)
	{
		if (g_sig_handle == SIGINT)
			res = 130;
		if (input[0] && !(res < 0))
		{
			add_history(input);
			res = -(minishell_action(input, &env) + 1);
			g_sig_handle = 0;
		}
		input = prompt(env);
	}
	return (main_exit(env, res));
}
