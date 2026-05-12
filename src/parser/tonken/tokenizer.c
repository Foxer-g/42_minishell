/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 04:23:12 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/09 16:52:13 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int64_t	token_stack_len(char *inp)
{
	int64_t	len;

	len = 0;
	while (input[0])
	{
		if (input[0] == '(' || input[0] == ')' || input[0] == '\''
			|| input[0] == '"' || input[0] == '\n' || input[0] == '*')
			len++;
		else if (input[0] == '$' || ft_isalnum(input[0]))
		{
			input++;
			while (ft_isalpha(input[0]))
				input++;
			len++;
		}
		else if (input[0] == '|' || input[0] == '<' || input[0] == '>'
			|| input[0] == '&' || input[0] == ';')
		{
			if (input[0] == input[1])
				input++;
			input++;
			len++;
		}
		return (len);
	}
}

t_token	*tokenizer(char *input)
{
	int64_t	i;
	int64_t	j;
	t_token	token_lst;

	i = 0;
	token_lst = malloc(token_stack_len(input) * sizeof(t_token));
	while (input[i])
	{
		if (input[i] == '$' || ft_isalnum(input[i]) || input[i] == '*')
			text_token(input, &i, &token_lst, j);
		else if (input[i] == '\'' || input[i] == '"' || input[i] == '('
			|| input[i] == ')')
			delimiter_token(input, &i, &token, j);
		else if (input[i] == '|' || input == '&' || input == ';'
			|| input == '\n')
			interaction_token(input, &i, &token_lst, j);
		else if (input[i] == '<' || input[i] == '>')
			io_token(input, &i, &token_lst, j);
		if (!token_lst[j]->content)
			free(token_lst);
		j++;
	}
	return (token_lst);
}
