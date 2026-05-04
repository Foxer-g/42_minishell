/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cmd_handler.c                                              _             */
/*                                                            _ \'-_,#        */
/*   By: neumann </var/spool/mail/neumann>                   _\'--','`|       */
/*                                                           \`---`  /        */
/*   Created: 2026/05/04 04:59:40 by neumann                  `----'`         */
/*   Updated: 2026/05/04 05:00:03 by neumann                                  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	safe_exec_setup(t_ffile out, t_ffile in)
{
	int32_t	failed;

	if (out == STDOUT_FILENO && in == STDIN_FILENO)
		return (0);
	failed = -1;
	if (in >= 0)
		failed = dup2(in, STDIN_FILENO);
	if (failed < 0)
	{
		if (out >= 0)
			ft_ffclose(out);
		if (in >= 0)
			ft_ffclose(in);
		return (-1);
	}
	if (out >= 0)
		failed = dup2(out, STDOUT_FILENO);
	if (out >= 0 && out != STDOUT_FILENO && out != STDIN_FILENO)
		ft_ffclose(out);
	if (in >= 0 && in != STDOUT_FILENO && in != STDIN_FILENO)
		ft_ffclose(in);
	if (failed < 0 || out < 0)
		return (-1);
	return (0);
}

int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env)
{
	int32_t	failed;

	failed = safe_exec_setup(out, in);
	if (failed == 0)
	{
		execve(cmd.path, cmd.arguments, env);
		failed = errno;
		perror(cmd.arguments[0]);
	}
	free(cmd.path);
	free_nt_tab(cmd.arguments, nt_tablen((void **)cmd.arguments));
	clear_filelist();
	exit(127 * (failed == ENOENT) | 1);
}

int8_t	exec_single(t_command *command, char **env)
{
	int8_t	status;

	status = execute(*command, command->outfd, command->infd, env);
	return (status);
}
