#include "minishell.h"
#include "parser.h"

static bool	command_expand(t_command **cmd, char ***env)
{
	intmax_t	i;
	bool		res;

	i = 0;
	while ((*cmd)[i].infile)
	{
		if (!expand(&((*cmd)[i]), env))
			return (false);
		res = strtrim_cmd_end(&((*cmd)[i]), env);
		if (!res)
			return (parsing_error(MALLOC, " : scheduler.c : command_expand : res", env));
		i++;
	}
	return (true);
}

static bool	redir_apply(t_command **cmd, char ***env)
{
	intmax_t	i;
	char		**tmp;
	t_redir		*red;

	i = 0;
	while ((*cmd)[i].path)
	{
		red = find_redir((*cmd)[i], env);
		if (!red)
			return (false);
		tmp = cmddup_without_redir((*cmd)[i], red, env);
		if (!tmp || !command_redir_set(&(*cmd)[i], red, env))
		{
			free(red);
			if (tmp)
			{
				ft_printf("not tmp\n");
				ft_free_nt_tab(tmp, args_len((*cmd)[i]) - 2);
			}
			return (!parsing_error(MALLOC, " : scheduler.c : redir_apply : tmp || redir set", env));
		}
		command_exec_set(&(*cmd)[i], tmp, ft_nt_tablen((void *)tmp));
		free(red);
		i++;
	}
	return (true);
}

static t_command	*piping(t_command *cmd, char ***env)
{
	const intmax_t	*pipes = find_pipe(cmd, env);
	t_command		*res;
	intmax_t		i;
	intmax_t		j;

	if (!pipes)
		return (full_cmd_dup(cmd, env));
	res = ft_calloc(cmd_len(cmd) / 2 + 2, sizeof(t_command));
	if (!is_valid_pipes(pipes, env) || !res)
	{
		if (!res)
			parsing_error(MALLOC, "scheduler.c : piping : res", env);
		free((void *)pipes);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (cmd[i].path)
		if (cmd[i++].path[0] != '|')
			res[j++] = cmd_dup(cmd[i - 1], env);
	return (is_error(res, j, env));
}

static bool	heredocs_handler(t_command **cmd, char ***env)
{
	const intmax_t	*heredocs = find_heredoc(*cmd, env);
	intmax_t		i;

	if (!heredocs)
		return (false);
	i = 0;
	while ((*cmd)[i].args)
	{
		if (heredocs[i] != -1)
		{
			if (!cmd_set_hd(&(*cmd[i]), heredocs[i], env))
			{
				free((void *)heredocs);
				return (false);
			}
		}
		i++;
	}
	free((void *) heredocs);
	return (true);
}

t_command	*scheduler(t_command *raw_command, char ***env)
{
	t_command	*res;
	bool		tmp;

	tmp = command_expand(&raw_command, env);
	if (!tmp)
		return (NULL);
	tmp = redir_apply(&raw_command, env);
	if (!tmp)
		return (NULL);
	res = piping(raw_command, env);
	if (!res)
		return (NULL);
	tmp = heredocs_handler(&res, env);
	if (!tmp)
	{
		free_command(res);
		return (NULL);
	}
	return (res);
}
