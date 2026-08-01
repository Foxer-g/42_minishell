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

typedef struct s_redir
{
	int32_t 	type;
	intmax_t	index;
	char		*file;
}	t_redir;

typedef struct s_command t_command;

t_command	*parser(char *input, char ***env);

bool		check_invalid_paterns(t_token *token_lst, char ***env);
bool		parsing_error(char *message, char *token, char ***env);

t_token		*tokenizer(char *input, char ***env);

bool		ft_iscmd_chr(char c);
int32_t		short_type(char *token);
int32_t		composed_type(char *token);
void		free_token(t_token *token_lst);

t_command	*command_gen(t_token *tkns, char ***env);

char		**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env);
bool		quote_join(t_token **tkns, char ***env);

void		free_command(t_command *cmds);
t_command	init_command(void);
t_command	cmd_dup(t_command cmd);
void		command_exec_set(t_command *command, char **cmd, uint64_t len);
bool		command_redir_set(t_command *command, t_redir *redir);

t_command	*scheduler(t_command *raw_command, char ***env);
bool		strtrim_cmd_end(t_command *cmd, const char c);
intmax_t	args_len(t_command cmd);
bool		cmd_set_hd(t_command *cmd, intmax_t index, char ***env);
intmax_t	*find_heredoc(t_command *cmd, char ***env);
t_command	*is_error(t_command *cmd, intmax_t len, char ***env);

t_redir		*find_redir(t_command cmd, char ***env);
char		**cmddup_without_redir(t_command cmd, t_redir *redir, char ***env);

intmax_t	*find_pipe(t_command *cmd, char ***env);
bool		is_valid_pipes(const intmax_t *pipes, char ***env);
intmax_t	cmd_len(t_command *cmd);
#endif
