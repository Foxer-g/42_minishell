/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 04:23:12 by f0xer             #+#    #+#             */
/*   Updated: 2026/08/11 22:51:23 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// @doc F0xer
// @kind san
// @desc Things have been done to squeeze tokenizer in 25 lines.
// @level 48

#include "parser.h"

// @doc token_stack_len
// @kind func
// @desc Calculate the amount of tokens in the input string.
// @param input: char **, Splited input string to calculate from.
// @return int64_t, Amount of tokens in the input string.

static int64_t	token_stack_len(char **input)
{
	int64_t	len;
	int64_t	i;

	len = 0;
	while (*input)
	{
		i = 0;
		while ((*input)[i])
		{
			i += get_token_len(*input, i);
			len++;
		}
		input++;
	}
	return (len);
}

// @doc get_token_len
// @kind func
// @desc Calculate tokens lenght, starting at the start index.
// @param input: char *, String to find the token and calculate it lenght.
// @param start: int64_t, Index to start calculating token lenght.
// @return int64_t, Next token lenght from start index.

int64_t	get_token_len(char *input, int64_t start)
{
	int64_t	i;

	i = start;
	if (ft_strchr("()'\"\n;", input[i]))
		i++;
	else if (ft_strchr("|<>&", input[i]))
	{
		i++;
		if (input[i - 1] == input[i] && input[i])
			i++;
		while (input[i] == ' ')
			i++;
		return (i - start);
	}
	else if (ft_strchr("$*", input[i]) || iscmd_chr(input[i]))
	{
		i++;
		while (input[i] && input[i] != ' ' && iscmd_chr(input[i]))
			i++;
	}
	else
		i++;
	while (input[i] == ' ')
		i++;
	return (i - start);
}

// @doc get_token_type
// @kind func
// @desc Determine the type of the input token.
// @param token: char *, Token to get the type from.
// @return int32_t, The token type.

int32_t	get_token_type(char *token)
{
	if (ft_strlen(token) - count_space(token) == 1)
		return (short_type(token));
	else if (ft_strchr("$*|<>&", token[0]) || iscmd_chr(token[0]))
		return (composed_type(token));
	return (0);
}

// @doc tokenizer_fill
// @kind func
// @desc Fill a [[t_token]] array with the tmp.
// @param tkn_lst: [[t_token]] *, Token array to fill up.
// @param tmp: char **, Array of string to fill token.
// @param env: char ***, Environement variables.
// @return bool, Completion status.

static bool	tokenizer_fill(t_token *tkn_lst, char **tmp, char ***env)
{
	intmax_t	i[2];

	ft_bzero(i, sizeof(i));
	i[1] = -1;
	while (tmp[++i[1]])
	{
		if (!tkn_from_split(&tkn_lst, &i[0], tmp[i[1]], env))
		{
			ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
			free_token(tkn_lst);
			return (!parsing_error(MALLOC, "", env));
		}
	}
	return (true);
}

// @doc tokenizer
// @kind func
// @desc Create a token array out of the input.
// @param input: char *, String given by the user.
// @param env: char ***, Environement variables.
// @return [[t_token]] *, A token array to be parsed.

t_token	*tokenizer(char *input, char ***env)
{
	t_token		*tkn_lst;
	char		**tmp;

	tmp = ft_preserving_split(input, ' ');
	if (!tmp)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	tkn_lst = ft_calloc(token_stack_len(tmp) + 1, sizeof(t_token));
	if (!tkn_lst)
	{
		ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	}
	if (!tokenizer_fill(tkn_lst, tmp, env))
		return (NULL);
	quote_to_command(tkn_lst);
	ft_free_nt_tab(tmp, ft_nt_tablen((void *)tmp));
	return (tkn_lst);
}
