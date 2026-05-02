/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 03:09:23 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/02 08:27:32 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	main(int32_t ac, char **av, char **aenv)
{
	t_command	*parsed_input;
	char		*input;

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
		input = prompt(aenv);
		while (input)
		{
			add_history(input);
//			parsed_input = parser(input);
//			exec(parsed_input, &aenv);
			ft_printf("input : %s\n", input);
			free(input);
			input = prompt(aenv); 
		}
		rl_clear_history();
		ft_printf("exit\n");
		return (0);
	}
}
