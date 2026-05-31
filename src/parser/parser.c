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
	if (check_invalid_paterns(token_lst, env))
	{
		free_tokens(token_lst);
		return (0);
	}	
	return (filtered_commands);
}
