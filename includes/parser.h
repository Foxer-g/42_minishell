/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 06:54:56 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/09 16:48:26 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H 
# include "libft.h" 

typedef enum e_token_type
{
	O_PAR,
	C_PAR,
	SQUOTE,
	DQUOTE,
	COMMAND,
	ENV_DESC,
	DPIPE,
	PIPE,
	DAND,
	AMP_SAND,
	SEMI,
	NLINE,
	I_REDIR,
	O_REDIR,
	APPEND,
	HEREDOC,
	WILDCARD,
}	t_token_type;

// @doc t_token
// @kind type
// @desc Token type.
// @field content: char *, Content of the token.
// @field type: int32_t, Type of the token.

typedef struct s_token
{
	char	*content;
	int32_t	type;
}	t_token;

t_token	*tokenizer(char *input);
bool	ft_iscmd_chr(char c);
int32_t	short_type(char *token);
int32_t	composed_type(char *token);
#endif
