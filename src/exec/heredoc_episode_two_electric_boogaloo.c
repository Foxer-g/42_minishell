/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   heredoc_episode_two_electric_boogaloo.c             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/15 04:08:02 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/15 04:08:17 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCES
#include "minishell.h"

static void	sig_tagle(int32_t signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
		g_sig_handle = SIGINT;
}

bool	here_doc_drain(t_command *cmd, const char *delimiter)
{
	struct sigaction	sa;
	struct sigaction	old;
	char				*line;
	bool				completed;

	sa.sa_handler = SIG_IGN;
	sa.sa_sigaction = sig_tagle;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strcmp(line, delimiter))
			break ;
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	completed = (g_sig_handle != SIGINT || line != NULL);
	free(line);
	sigaction(SIGINT, &old, NULL);
	ft_ffclose(&cmd->hd_pipe);
	return (completed);
}
