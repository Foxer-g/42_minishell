#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include "libft.h"

# define WE 1
# define RE 0

typedef struct s_command
{
	char	*path;
	char	**arguments;
	pid_t	pid;
	char	*infile;
	char	*outfile;
}	t_command;

int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env);
char	**command_to_args(char *command);
void	exec_single(t_command *command, t_ffile stdin, t_ffile stdout);
#endif
