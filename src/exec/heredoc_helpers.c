/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   heredoc_episode_two_electric_boogaloo.c             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo <rboutelo@student.42angouleme.fr>      ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/15 04:08:02 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/15 06:33:32 by rboutelo           ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#define EXEC_SOURCES
#include "minishell.h"

static void	sig_heredoc(int32_t signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		g_sig_handle = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
	}
}

static void	set_up_signals(struct sigaction *old)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_sigaction = sig_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old);
}

bool	here_doc_drain(t_command *cmd, const char *delimiter)
{
	struct sigaction	old;
	char				*line;
	bool				completed;

	set_up_signals(&old);
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

// @doc exec_here_doc
// @kind func
// @desc Executes a here doc for a single command
// @param cmd: [[t_command]] *, The command to execute the heredoc of.
// @param delimiter: const char *, The heredoc delimiter.
// @param env: char **, The environment.
// @returns bool, Wether the heredoc was stopped with a SIGINT.
bool	exec_here_doc(t_command *cmd, const char *delimiter, char **env)
{
	struct sigaction	old;
	char				*line;
	bool				completed;

	set_up_signals(&old);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (!ft_strcmp(line, delimiter))
			break ;
		expand_here_doc(&line, &env);
		write(cmd->hd_pipe, line, ft_strlen(line));
		write(cmd->hd_pipe, "\n", 1);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	completed = (g_sig_handle != SIGINT || line != NULL);
	free(line);
	sigaction(SIGINT, &old, NULL);
	ft_ffclose(&cmd->hd_pipe);
	get_next_line(-1);
	return (completed);
}
