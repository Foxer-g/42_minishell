/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_lens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 04:06:15 by toespino          #+#    #+#             */
/*   Updated: 2026/08/12 04:06:15 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// @doc count_space
// @kind func
// @desc Count spaces in a string.
// @param str: char *, String to process.
// @return intmax_t, Spaces count.

intmax_t	count_space(char *str)
{
	intmax_t	i;
	intmax_t	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			res++;
		i++;
	}
	return (res);
}

// @doc args_len
// @kind func
// @desc Count the arguments of a [[t_command]].
// @param cmd: [[t_command]], Command to process.
// @return intmax_t, Arguments count.

intmax_t	args_len(t_command cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd.args[i])
		i++;
	return (i);
}

// @doc cmd_len
// @kind func
// @desc Get length of a command array.
// @param cmd: [[t_command]] *, Command array to process.
// @return intmax_t, Command array length.

intmax_t	cmd_len(t_command *cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd[i].infile)
		i++;
	return (i);
}
