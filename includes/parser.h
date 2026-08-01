/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 06:54:56 by f0xer             #+#    #+#             */
/*   Updated: 2026/07/31 01:23:46 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include <stdint.h>
# undef RETURN

typedef enum e_token_type
{
	O_PAR,
	C_PAR,
	QUOTE,
	DQUOTE,
	COMMAND,
	ENV_DEC,
	DPIPE,
	PIPE,
	DAND,
	SEMI,
	RETURN,
	I_REDIR,
	O_REDIR,
	APPEND,
	HEREDOC,
	WILDCARD,
}	t_token_type;

typedef	struct s_token
{
	char	*content;
	int32_t	type;
}	t_token;

t_token	*tokenizer(char *input);
#endif
