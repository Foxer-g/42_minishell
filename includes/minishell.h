/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                         ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 01:44:34 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/19 17:36:16 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "file.h"
//# include "parser.h"
# ifndef EXEC_SOURCE
#  include "exec.h"
# else
# include "exec_int.h"
#endif

# define WE 1
# define RE 0

# define PROMPT_A "\x1b[22;36;45m"
# define PROMPT_B "\x1b[0m @ \x1b[32;45m"
# define PROMPT_C "\x1b[0m $ "

// @doc t_command
// @kind type
// @desc Bloated representation of a command.
typedef struct s_command
{
	char	*path;
	char	**arguments;
	pid_t	pid;
	t_ffile	infd;
	t_ffile	outfd;
	bool	append;
	char	*infile;
	char	*outfile;
}	t_command;

extern sig_atomic_t	g_sig_handle;

char	**command_to_args(char *command);
char	*join_tab(char **tab);
char	*prompt(char **aenv);
void	sig_handle(int32_t signal, siginfo_t *info, void *context);
void 	error(const char *err_message);
#endif
