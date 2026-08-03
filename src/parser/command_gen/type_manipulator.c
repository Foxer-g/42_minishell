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

bool	command_redir_set(t_command *command, t_redir *redir)
{
	if (redir[0].type)
	{
		free(command->infile);
		command->infile = ft_strdup(command->args[redir[0].index + 1]);
		if (!command->infile)
			return (false);
	}
	if (redir[1].type)
	{
		free(command->outfile);
		command->outfile = ft_strdup(command->args[redir[1].index + 1]);
		if (!command->outfile)
			return (false);
		command->append = (redir[1].type == APPEND);
	}
	return (redir[0].type || redir[1].type);
}

t_command	init_command(void)
{
	t_command	res;

	res.path = NULL;
	res.args = NULL;
	res.pid = 0;
	res.infd = 0;
	res.outfd = 0;
	res.append = false;
	res.infile = ft_strdup("stdin");
	res.outfile = ft_strdup("stdout");
	return (res);
}

t_command	cmd_dup(t_command cmd)
{
	t_command	res;

	res = init_command();
	command_exec_set(&res, cmd.args, args_len(cmd));
	free(res.infile);
	free(res.outfile);
	res.infile = ft_strdup(cmd.infile);
	res.outfile = ft_strdup(cmd.outfile);
	res.append = cmd.append;
	if (!res.infile || !res.outfile)
	{
		free_command(&res);
		return ((t_command){});
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
