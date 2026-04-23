/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 01:44:34 by f0xer             #+#    #+#             */
/*   Updated: 2026/04/24 01:44:38 by f0xer            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include "libft.h"

typedef struct s_command
{
	char	*path;
	char	**arguments;
	__pid_t	pid;
}	t_command;

int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env);
char	**command_to_args(char *command);
#endif
