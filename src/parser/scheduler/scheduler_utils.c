#include "minishell.h"
#include "parser.h"

intmax_t	args_len(t_command cmd)
{
	intmax_t	i;

	i = 0;
	while (cmd.args[i])
		i++;
	return (i);
}

bool	strtrim_cmd_end(t_command *cmd, char ***env)
{
	char		**tmp;
	intmax_t	i;

	tmp = ft_calloc(args_len(*cmd) + 1, sizeof(char *));
	if (!tmp)
		return (!parsing_error(MALLOC, " : scheduler_utils.c : strtrim_cmd_end : tmp\n", env));
	i = 0;
	while ((*cmd).args[i])
	{
		tmp[i] = ft_strtrim((*cmd).args[i], " ");
		if (!tmp[i])
		{
			ft_free_nt_tab(tmp, i);
			return (!parsing_error(MALLOC, " : scheduler_utils.c : strtrim_cmd_end : tmp[i]\n", env));
		}
		i++;
	}
	command_exec_set(cmd, tmp, args_len(*cmd));
	ft_free_nt_tab(tmp, args_len(*cmd));
	return (true);
}

bool	cmd_set_hd(t_command *cmd, intmax_t index, char ***env)
{
	char		**tmp;
	intmax_t	i[2];

	tmp = ft_calloc(args_len(*cmd) + 1, sizeof(char *));
	if (!((*cmd).args)[index + 1] || !tmp)
		return (!parsing_error(MALLOC, " : scheduler_utils.c : cmd_set_hd : tmp || no file\n", env));
	tmp[0] = ft_strdup((*cmd).args[index + 1]);
	ft_bzero(i, sizeof(i));
	while ((*cmd).args[i[0]++])
	{
		if (i[0] - 1 != index || i[0] != index)
			tmp[i[1]++] = ft_strdup((*cmd).args[i[0] - 1]);
		if (!tmp[i[1] - 1])
		{
			ft_free_nt_tab(tmp, args_len(*cmd) + 1);
			return ((void *)((uintptr_t)!parsing_error(MALLOC, " : scheduler_utils.c : cmd_set_hd : tmp element\n", env)));
		}
	}
	command_exec_set(cmd, tmp, args_len(*cmd));
	ft_free_nt_tab(tmp, args_len(*cmd) + 1);
	free((*cmd).path);
	(*cmd).path = ft_strdup((*cmd).args[1]);
	free((*cmd).infile);
	(*cmd).infile = ft_strdup("<<");
	return (true);
}

intmax_t	*find_heredoc(t_command *c, char ***env)
{
	intmax_t	*res;
	intmax_t	i[2];

	res = ft_calloc(cmd_len(c), sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, " : scheduler_utils.c : find_heredoc : res\n", env)));
	i[0] = -1;
	while (c[++i[0]].infile)
	{
		res[i[0]] = -1;
		i[1] = -1;
		while (c[i[0]].args[++i[1]])
		{
			if (c[i[0]].args[i[1]][0] == '<' && c[i[0]].args[i[1]][1] == '<')
			{
				if (res[i[0]] != -1)
				{
					free(res);
					parsing_error(PARSING, c[i[0]].args[i[1]], env);
					return (false);
				}
				res[i[0]] = i[1];
			}
		}
	}
	return (res);
}
