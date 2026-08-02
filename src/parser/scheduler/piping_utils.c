#include "minishell.h"
#include "parser.h"

intmax_t	cmd_len(t_command *cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd[i].infile)
		i++;
	return (i);
}

intmax_t	*find_pipe(t_command *cmd, char ***env)
{
	intmax_t	i;
	intmax_t	nb;
	intmax_t	*res;

	i = 0;
	while (cmd[i].infile)
		if (cmd[i++].path[0] == '|')
			nb++;
	i = 0;
	res = ft_calloc(nb + 2, sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, "", env)));
	while (cmd[i].infile)
		if (cmd[i++].path[0] == '|')
			res[res[0]++] = i - 1;
	if (res[res[0]] == i - 1)
	{
		free(res);
		return ((void *)((uintptr_t)!parsing_error(SYNTAX, "|", env)));
	}
	return (res);
}

t_command	*is_error(t_command *cmd, intmax_t len, char ***env)
{
	intmax_t	i;

	i = 0;
	while (i < len)
	{
		if (!cmd[i].infile)
		{
			free_command(cmd);
			parsing_error(MALLOC, "", env);
			return (NULL);
		}
		i++;
	}
	return (cmd);
}

bool	is_valid_pipes(const intmax_t *pipes, char ***env)
{
	intmax_t	i;

	i = 0;
	if (pipes[0] == 0)
		return (parsing_error(SYNTAX, "|", env));
	while (++i <= pipes[0])
		if (pipes[i] == 1 || (i < pipes[0] - 1 && pipes[i] + 2 == pipes[i + 1]))
			return (!parsing_error(INV_PIPES, "|", env));
	return (true);
}
