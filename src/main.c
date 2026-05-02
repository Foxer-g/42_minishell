/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 03:09:23 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/02 04:12:20 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prompt(void)
{
	char	*str_prompt;
	char	*out;
	char	*path;
	char	*usr;

	str_prompt = ft_strdup("\\001");
	usr = getenv("USER");
	path = getenv("PWD");
	if (ft_strlen(path) > (ft_strlen(getenv("HOME"))))
	{
		path += ft_strlen(getenv("HOME")) + 5;
		path[0] = '~';
	}
	str_prompt = extend(str_prompt, PROMPT_A);
	str_prompt = extend(str_prompt, usr);
	str_prompt = extend(str_prompt, PROMPT_B);
	str_prompt = extend(str_prompt, path);
	str_prompt = extend(str_prompt, PROMPT_C);
	str_prompt = extend(str_prompt, "\\002");
	out = readline(str_prompt);
	free(str_prompt);
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
