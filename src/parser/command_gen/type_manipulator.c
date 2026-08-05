#include "parser.h"
#include "minishell.h"

void	command_exec_set(t_command *command, char **cmd, uint64_t len)
{
	int64_t	i;

	if (cmd)
	{
		free((*command).path);
		(*command).path = ft_strdup(cmd[0]);
		if ((*command).args)
		{
			i = 0;
			while ((*command).args[i])
				free((*command).args[i++]);
			free((*command).args);
		}
		(*command).args = ft_calloc((len + 1), sizeof(char *));
		if (!(*command).args)
			return ;
		i = 0;
		while (i < (int64_t) len)
		{
			(*command).args[i] = ft_strdup(cmd[i]);
			i++;
		}
	}
}

bool	command_redir_set(t_command *command, t_redir *redir, char ***env)
{
	if (redir[0].type)
	{
		free(command->infile);
		command->infile = ft_strdup(command->args[redir[0].index + 1]);
		if (!command->infile)
			return (!parsing_error(MALLOC, " : type_manipulator.c : command_redir_set : infile", env));
	}
	if (redir[1].type)
	{
		free(command->outfile);
		command->outfile = ft_strdup(command->args[redir[1].index + 1]);
		if (!command->outfile)
			return (!parsing_error(MALLOC, " : type_manipulator.c : command_redir_set : outfile", env));
		command->append = (redir[1].type == APPEND);
	}
	return (true);
}

t_command	init_command(char ***env)
{
	t_command	res;

	res.path = NULL;
	res.args = NULL;
	res.pid = 0;
	res.infd = STDIN_FILENO;
	res.outfd = STDOUT_FILENO;
	res.append = false;
	res.infile = ft_strdup("stdin");
	if (!res.infile)
		return ((t_command){.append=!parsing_error(MALLOC, " : type_manipulator.c : init_command : res.infile", env)});
	res.outfile = ft_strdup("stdout");
	if (!res.infile)
		return ((t_command){.append=!parsing_error(MALLOC, " : type_manipulator.c : init_command : res.outfile", env)});
	return (res);
}

t_command	cmd_dup(t_command cmd, char ***env)
{
	t_command	res;

	res = init_command(env);
	command_exec_set(&res, cmd.args, args_len(cmd));
	free(res.infile);
	free(res.outfile);
	res.infile = ft_strdup(cmd.infile);
	if (!res.infile)
		return ((t_command){.append=!parsing_error(MALLOC, " : type_manipulator.c : cmd_dup : res.infile", env)});
	res.outfile = ft_strdup(cmd.outfile);
	if (!res.outfile)
		return ((t_command){.append=!parsing_error(MALLOC, " : type_manipulator.c : cmd_dup : res.outfile", env)});
	res.append = cmd.append;
	if (!res.infile || !res.outfile)
	{
		free_command(&res);
		return ((t_command) {.append=!parsing_error(MALLOC, " : type manipulator.c : cmd_dup : infile || outfile", env)});
	}
	return (res);
}

void	free_command(t_command *cmds)
{
	uint64_t	i;
	uint64_t	j;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i].path || cmds[i].args)
	{
		free(cmds[i].path);
		if (cmds[i].args)
		{
			j = 0;
			while (cmds[i].args[j])
			{
				free(cmds[i].args[j]);
				j++;
			}
			free(cmds[i].args);
		}
		free(cmds[i].infile);
		free(cmds[i].outfile);
		i++;
	}
	free(cmds);
}
