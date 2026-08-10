/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 06:54:56 by f0xer             #+#    #+#             */
/*   Updated: 2026/08/07 21:39:34 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H 
# include "libft.h" 

# define MALLOC "\e[31;36merror \e[0m\e[31m: Malloc error\e[0m"
# define SYNTAX "\e[31;36merror \e[0m\e[31m: Syntax error near : \e[0m"
# define PARSING "\e[31;36merror \e[0m\e[31m: Parsing error near : \e[0m"
# define INV_CHAR "\e[31;36merror \e[0m\e[31m: Invalid character : \e[0m"
# define INV_PIPES "\e[31;36merror \e[0m\e[31m: Invalid pipe near : \e[0m"
# define INV_HEREDOC "\e[31;36merror \e[0m\e[31m: Invalid heredoc near : \e[0m"

typedef enum e_token_type
{
	O_PAR,
	C_PAR,
	QUOTE,
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
	char		**file;
	int32_t		type;
	intmax_t	*index;
}	t_redir;

typedef struct s_command	t_command;

/*----------------------*/
/*    parser includes   */
/*----------------------*/

t_command	*parser(char *input, char ***env);

bool		check_invalid_paterns(t_token *token_lst, char ***env);
bool		parsing_error(char *message, char *token, char ***env);

/*----------------------*/
/*  tokenizer includes  */
/*----------------------*/

t_token		*tokenizer(char *input, char ***env);

bool		iscmd_chr(char c);
bool		tkn_from_split(t_token **tkn, int64_t *i, char *split, char ***env);
int32_t		short_type(char *token);
int32_t		composed_type(char *token);
void		free_token(t_token *token_lst);
int64_t		get_token_len(char *input, int64_t start);
int32_t		get_token_type(char *token);
void		quote_to_command(t_token *tkn);

/*----------------------*/
/* command gen includes */
/*----------------------*/

t_command	*command_gen(t_token **tkns, char ***env);

intmax_t	count_space(char *str);
char		**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env);
bool		quote_join(t_token **tkns, char ***env);

void		free_command(t_command *cmds);
t_command	init_command(char ***env);
t_command	cmd_dup(t_command cmd, char ***env);
bool		command_exec_set(t_command *command, char **cmd, uint64_t len);
bool		command_redir_set(t_command *command, t_redir *redir, char ***env);

/*----------------------*/
/*  scheduler includes  */
/*----------------------*/

t_command	*scheduler(t_command *raw_command, char ***env);
bool		strtrim_cmd_end(t_command *cmd, char ***env);
intmax_t	args_len(t_command cmd);
bool		cmd_set_hd(t_command *cmd, intmax_t index, char ***env);
intmax_t	*find_heredoc(t_command *cmd, char ***env);
bool		is_valid_heredocs(const intmax_t *heredocs, t_command *cmd);
t_command	*is_error(t_command *cmd, intmax_t len, char ***env);
bool		cmddup_without_empty(t_command *cmd, char ***env);

t_redir		*find_redir(t_command cmd, char ***env);
int32_t		is_redir(char c1, char c2);
char		**cmddup_without_redir(t_command cmd, char ***env);

void		free_redir(t_redir *redir);
t_redir		*redir_calloc(intmax_t n, char ***env);

intmax_t	*find_pipe(t_command *cmd, char ***env);
bool		is_valid_pipes(const intmax_t *pipes, char ***env);
intmax_t	cmd_len(t_command *cmd);
t_command	*full_cmd_dup(t_command *src, char ***env);
#endif
