#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"

typedef struct s_command
{
	char	*path;
	char	**arguments;
	__pid_t	pid;
}	t_command;

int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env);
#endif
