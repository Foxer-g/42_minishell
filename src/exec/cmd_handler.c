#include "minishell.h"

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
