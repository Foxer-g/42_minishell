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
	char	*str;

	len = 0;
	while (*input)
	{
		len++;
		str = *input;
		while (*str)
		{
			if (ft_strchr("|<>&", *str))
				str += 1 + (*str == str[1]);
			else if (ft_strchr("$*", *str) || iscmd_chr(*str))
				while (*++str && iscmd_chr(*str))
					;
			else
				str++;
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

static int64_t	get_token_len(char *input, int64_t start)
{
	int64_t	i;

	i = start;
	if (ft_strchr("()'\"\n;", input[i]))
		i++;
	else if (ft_strchr("|<>&", input[i]))
	{
		if (input[i] == input[i + 1])
			i++;
		i++;
	}
	else if (ft_strchr("$*", input[i]) || iscmd_chr(input[i]))
	{
		i++;
		while (iscmd_chr(input[i]))
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

static int32_t	get_token_type(char *token)
{
	if (ft_strlen(token) - count_space(token) == 1)
		return (short_type(token));
	else if (ft_strchr("$*|<>&", token[0]) || iscmd_chr(token[0]))
		return (composed_type(token));
	return (0);
}

// @doc tkn_from_split
// @kind func
// @desc Generate tokens form a splitted input.
// @param tkn: [[t_token]] **, Token list to fill.
// @param i: int64_t *, Token list index.
// @param split: char *, Splitted input.
// @param env: char***, Environment variable.
// @return bool, Exit status.

static bool	tkn_from_split(t_token **tkn, int64_t *i, char *split, char ***env)
{
	int64_t	len;
	int64_t	j;

	j = 0;
	while (split[j])
	{
		len = get_token_len(split, j);
		(*tkn)[*i].content = ft_substr(split, j, len);
		if (!(*tkn)[*i].content)
			return (!parsing_error(MALLOC, "", env));
		(*tkn)[*i].type = get_token_type((*tkn)[*i].content);
		j += len;
		(*i)++;
	}
	return (true);
}

// @doc tokenizer
// @kind func
// @desc Create a token array out of the input.
// @param input: char *, String given by the user.
// @return [[t_token]], A token array to be parsed.

t_token	*tokenizer(char *input, char ***env)
{
	t_token		*tkn_lst;
	char		**tmp;
	intmax_t	i;
	intmax_t	j;

	tmp = ft_preserving_split(input, ' ');
	if (!tmp)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, " : tokenizer.c : tokenizer : tmp", env)));
	tkn_lst = ft_calloc(token_stack_len(tmp) + 1, sizeof(t_token));
	if (!tkn_lst)
	{
		ft_free_nt_tab(tmp, (int32_t) ft_nt_tablen((void *) tmp));
		return ((void *)((uintptr_t) !parsing_error(MALLOC, " : tokenizer.c : tokenizer : tkn_lst", env)));
	}
	i = 0;
	j = -1;
	while (tmp[++j])
		if (!tkn_from_split(&tkn_lst, &i, tmp[j], env))
			return (NULL);
	ft_free_nt_tab(tmp, ft_nt_tablen((void *) tmp));
	return (tkn_lst);
}
