#include "minishell.h"
#include "parser.h"

bool	is_valid_heredocs(const intmax_t *heredocs, t_command *cmd)
{
	intmax_t	i;
	intmax_t	j;

	i = 0;
	while (cmd[i].infile)
	{
		j = heredocs[i];
		if (j != -1 && (!cmd[i].args[j + 1]
				|| is_redir(cmd[i].args[j + 1][0],
				cmd[i].args[j + 1][1])))
			return (false);
		i++;
	}
	return (true);
}

static bool	fill_args_without_hd(char **res, t_command cmd,
		intmax_t index, char ***env)
{
	intmax_t	i;
	intmax_t	j;

	i = 0;
	j = 0;
	while (cmd.args[i])
	{
		if (i != index && i != index + 1)
		{
			res[j] = ft_strdup(cmd.args[i]);
			if (!res[j++])
			{
				parsing_error(MALLOC, "", env);
				return (false);
			}
		}
		i++;
	}
	return (true);
}

static char	**args_to_tab_without_hd(t_command cmd, intmax_t index, char ***env)
{
	char	**res;

	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res)
	{
		parsing_error(MALLOC, "", env);
		return (NULL);
	}
	if (!fill_args_without_hd(res, cmd, index, env))
	{
		ft_free_nt_tab(res, args_len(cmd));
		return (NULL);
	}
	return (res);
}

bool	cmd_set_hd(t_command *cmd, intmax_t index, char ***env)
{
	char	**tmp;

	tmp = args_to_tab_without_hd(*cmd, index, env);
	if (!tmp)
		return (false);
	if (!tmp[0])
	{
		free(cmd->path);
		cmd->path = NULL;
		free(cmd->infile);
		cmd->infile = ft_strdup("<<");
		ft_free_nt_tab(tmp, args_len(*cmd));
		return (cmd->infile != NULL);
	}
	if (!command_exec_set(cmd, tmp, args_len(*cmd) - 2))
	{
		ft_free_nt_tab(tmp, args_len(*cmd));
		return (false);
	}
	ft_free_nt_tab(tmp, args_len(*cmd));
	free(cmd->infile);
	cmd->infile = ft_strdup("<<");
	return (cmd->infile != NULL);
}

static intmax_t	*heredoc_error(intmax_t *res, char *arg, char ***env)
{
	free(res);
	parsing_error(PARSING, arg, env);
	return (NULL);
}

intmax_t	*find_heredoc(t_command *c, char ***env)
{
	intmax_t	*res;
	intmax_t	i[2];

	res = ft_calloc(cmd_len(c) + 1, sizeof(intmax_t));
	if (!res)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, "", env)));
	i[0] = -1;
	while (c[++i[0]].infile)
	{
		res[i[0]] = -1;
		if (!c[i[0]].args)
			continue ;
		i[1] = -1;
		while (c[i[0]].args[++i[1]])
		{
			if (c[i[0]].args[i[1]][0] == '<'
				&& c[i[0]].args[i[1]][1] == '<')
			{
				if (res[i[0]] != -1)
					return (heredoc_error(res,
							c[i[0]].args[i[1]], env));
				res[i[0]] = i[1];
			}
		}
	}
	return (res);
}
