/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 03:09:23 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/01 22:23:32 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prompt(void)
{
	char	*prompt;
	char	*out;
	char	*path;
	char	*usr;

	prompt = "\001";
	usr = getenv("USER");
	path = getenv("PWD");
	if (ft_strlen(path) > (ft_strlen(usr) + 6))
	{
		path += ft_strlen(usr) + 5;
		path[0] = '~';
	}
	prompt = extend(prompt, PROMPT_A);
	prompt = extend(prompt, usr);
	prompt = extend(prompt, PROMPT_B);
	prompt = extend(prompt, path);
	prompt = extend(prompt, PROMPT_C);
	prompt = extend(prompt, "\002");
	out = readline(prompt);
	return (out);
}

int32_t	main(int32_t ac, char **av, char **aenv)
{
	t_command	*parsed_input;
	char		*input;

	if (ac != 1)
	{
		input = "hehehehe";
//		input = join_tab(++av);
//		parsed_input = parser(input);
//		exec(parsed_input, &aenv);
		printf("res : %s\n", input);
	}
	else
	{
		input = prompt();
		while (input)
		{
			add_history(input);
//			parsed_input = parser(input);
//			exec(parsed_input, &aenv);
			printf("res : %s\n", input);
			free(input);
			input = prompt(); 
		}
		return (2);
	}
}
