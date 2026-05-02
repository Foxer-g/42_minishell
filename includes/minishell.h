/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 01:44:34 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/01 22:22:03 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "file.h"

# define WE 1
# define RE 0

# define PROMPT_A "\x1b[22;36;45m"
# define PROMPT_B "\x1b[0m @ \x1b[32;45m"
# define PROMPT_C "\x1b[0m $"

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

int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env);
char	**command_to_args(char *command);
void	entrypoint(t_command **cmds, char **env);
int8_t	exec_single(t_command *command, char **env);
#endif
