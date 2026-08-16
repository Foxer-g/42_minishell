/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toespino <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 04:23:50 by toespino          #+#    #+#             */
/*   Updated: 2026/08/16 04:34:48 by toespino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_init(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(action));
	sigemptyset(&action.sa_mask);
	action.sa_sigaction = sig_handle;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &action, NULL);
	action.sa_handler = SIG_IGN;
	action.sa_flags = 0;
	sigaction(SIGQUIT, &action, NULL);
}

void	sig_handle(int32_t signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		g_sig_handle = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig_parent(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(action));
	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_IGN;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}

void	sig_child(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(action));
	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_DFL;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}
