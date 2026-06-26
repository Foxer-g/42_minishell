/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 05:31:25 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/07 08:41:28 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_command	*parser(char *input, char **env)
{
	t_token		*token_lst;
	t_command	*raw_commands;
	t_command	*filtered_commands;

	token_lst = tokenizer(input);
	if (!token_lst)
		return (NULL);
	if (check_invalid_paterns(token_lst, env))
	{
		free_tokens(token_lst);
		return (NULL);
	}
	raw_commands = command_gen(token_lst);
	free_token(token_lst);
	if (!raw_commands)
		return (NULL);
	filtered_commands = scheduler(raw_commands);
	free_command(raw_commands);
	if (!filtered_commands)
		return (NULL);
	return (filtered_commands);
}
