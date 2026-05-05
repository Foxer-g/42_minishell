/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 03:09:23 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/04 06:26:49 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sig_handle = 0;

int32_t	main(int32_t ac, char **av, char **aenv)
{
	struct sigaction	sig_action;
	t_command			*parsed_input;
	char				*input;

	if (ac != 1)
	{
		input = join_tab(++av);
//		parsed_input = parser(input);
//		entry_point(parsed_input, &aenv);
//		test_print(parsed_input);
		ft_printf("input : %s\n", input);
		free(input);
	}
	else
	{
		sig_action.sa_sigaction = sig_handle;
		sig_action.sa_flags = SA_SIGINFO;
		sigemptyset(&sig_action.sa_mask);
		sigaction(SIGINT, &sig_action, NULL);
		sigaction(SIGQUIT, &sig_action, NULL);
		input = prompt(aenv);
		while (input)
		{
			add_history(input);
//			parsed_input = parser(input);
//			entry_point(parsed_input, &aenv);
			ft_printf("input : %s\n", input);
			free(input);
			input = prompt(aenv);
		}
		rl_clear_history();
		ft_printf("exit\n");
		return (0);
	}
}
