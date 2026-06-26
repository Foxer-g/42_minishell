#include "parser.h"
#include "minishell.h"

void	command_exec_set(t_command *command, char **cmd, uint64_t len)
{
	int64_t	i;

	if (cmd)
	{
		free(command->path);
		command->path = ft_stdup(cmd[0]);
		if (command->arguments)
		{
			i = 0;
			while (command->arguments[i])
				free(command->arguments[i++]);
			free(command->arguments);
		}
		command->arguments = ft_calloc((len + 1), sizeof(char *));
		if (!command->arguments)
			return ;
		i = 0;
		while (i < len)
		{
			command->arguments[i] = ft_strdup(cmd[i]);
			i++;
		}
	}
}

void	command_file_set(t_command *command, char *in, char *out, bool app)
{
	if (in)
	{
		free(*command.infile);
		*command.infile = ft_strdup(in);
	}
	if (out)
	{
		free(*command.outfile);
		*command.outfile = ft_strdup(out);
	}
	*command.append = app;
}

t_command	init_command(void)
{
	t_command	res;

	res.path = NULL;
	res.arguments = NULL;
	res.pid = 0;
	res.infd = 0;
	res.outfd = 0;
	res.append = false;
	res.infile = ft_strdup("stdin");
	res.outfile = ft_strdup("stdout");
	return (res);
}

void	free_command(t_command *cmds)
{
	uint64_t	i;
	uint64_t	j;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i].path || cmds[i].arguments)
	{
		free(cmds[i].path);
		if (cmds[i].arguments)
		{
			j = 0;
			while (cmds[i].arguments[j])
			{
				free(cmds[i].arguments[j]);
				j++;
			}
			free(cmds[i].arguments);
		}
		free(cmds[i].infile);
		free(cmds[i].outfile);
		i++;
	}
	free(cmds);
}
