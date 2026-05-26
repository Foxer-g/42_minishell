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

// @doc F0xer
// @kind san
// @desc Things have been do to squeeze tokenizer in 25 lines.
// @level 37

#include "parser.h"

// @doc token_stack_len
// @kind func
// @desc Calculate the amount of tokens in the input string.
// @param input: char *, Input string to calculate from.
// @return int64_t, Amount of tokens in the input string.

static int64_t	token_stack_len(char *input)
{
	int64_t	len;

	len = 0;
	while (input[0])
	{
		if (ft_strnstr("()'\"\n", &input[0], 5))
			len++;
		else if (ft_strnstr("$*", &input[0], 2) || !ft_iscmd_chr(input[0]))
		{
			input++;
			while (ft_iscmd_chr(input[0]))
				input++;
			len++;
		}
		else if (ft_strnstr("|<>&;", &input[0], 5))
		{
			if (input[0] == input[1])
				input++;
			input++;
			len++;
		}
		return (len);
	}
	return (0);
}

// @doc get_token_len
// @kind func
// @desc Calculate tokens lenght, starting at the start index.
// @param input: char *, String to find the token and calculate it lenght.
// @param start: int64_t, Index to start calculating token lenght.
// @return int64_t, Next token lenght from start index.

static int64_t	get_token_len(char *input, int64_t start)
{
	int64_t	i;

	i = start;
	if (ft_strnstr("()'\"\n", &input[i], 5))
		i++;
	else if (ft_strnstr("$*", &input[i], 2) || ft_iscmd_chr(input[i]))
	{
		i++;
		while (ft_iscmd_chr(input[i]))
			i++;
	}
	else if (ft_strnstr("|<>&;", &input[i], 5))
	{
		if (input[i] == input[i + 1])
			i++;
		i++;
	}
	return (i - start);
}

static int32_t	get_token_type(char *token)
{
	(void)token;
	return (0);
}

// @doc tokenizer
// @kind func
// @desc Create a token array out of the input.
// @param input: char *, String given by the user.
// @return [[t_token]], A token array to be parsed.

t_token	*tokenizer(char *input)
{
	t_token	*tkn_lst;
	char	**tmp;
	int64_t	i[4];

	tmp = ft_split(input, ' ');
	if (!tmp)
		return (0);
	tkn_lst = malloc(token_stack_len(input) * sizeof(t_token));
	if (!tkn_lst)
		return (free_nt_tab(tmp, (int32_t) nt_tablen((void **) tmp)));
	ft_memset(i, -1, 4);
	while (tmp[++i[0]])
	{
		i[2] = 0;
		while (tmp[i[0]][i[2]])
		{
			i[3] = get_token_len(tmp[i[0]], i[2]);
			tkn_lst[++i[1]].content = ft_substr(tmp[i[0]], i[2], i[3]);
			tkn_lst[i[1]].type = get_token_type(tkn_lst[i[1]].content);
			i[2] += i[3];
		}
	}
	free_nt_tab(tmp, (int32_t) nt_tablen((void **) tmp));
	free(input);
	return (tkn_lst);
}
