/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                         ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 01:44:34 by f0xer             #+#    #+#             */
/*   Updated: 2026/08/08 01:05:25 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <linux/limits.h>
# include <sys/wait.h>
# include "libft.h"
# include "file.h"
# include "parser.h"
# ifndef EXEC_SOURCE
#  include "exec.h"
# else
#  include "exec_int.h"
# endif

# define WE 1
# define RE 0

# define PROMPT_A "\x1b[22;36m"
# define PROMPT_B "\x1b[0m@\x1b[32m"
# define PROMPT_C "\x1b[0m$ "

// @doc t_command
// @kind type
// @desc Bloated representation of a command.
typedef struct s_command
{
	char	*path;
	char	**args;
	pid_t	pid;
	t_ffile	infd;
	t_ffile	outfd;
	t_ffile	append;
	char	*infile;
	char	*outfile;
	bool	builtin;
}	t_command;

enum e_builtin
{
	CD = '\0',
	PWD = 'd',
	ECHO = 'h',
	EXIT = 'i',
	EXPORT = 'p',
	UNSET = 's',
	ENV = 'v',
};

extern sig_atomic_t	g_sig_handle;

char		**command_to_args(char *command);
char		*join_tab(char **tab);
char		*prompt(char **aenv);
t_command	*parser(char *input, char ***env);
void		sig_handle(int32_t signal, siginfo_t *info, void *context);
void		error(const char *err_message);
bool		expand(t_command *cmd, char ***env);
bool		expand_here_doc(char **line, char ***env);

void		test_print(t_command *input);
#endif
