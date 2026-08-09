#include "minishell.h"
#include "parser.h"

static bool	fill_args_without_hd(char **res, t_command cmd,
	intmax_t index, char ***env)
{
	intmax_t	i;
	intmax_t	j;

	res[0] = ft_strdup(cmd.args[index + 1]);
	if (!res[0])
	{
		parsing_error(MALLOC, "", env);
		return (false);
	}
	i = -1;
	j = 1;
	while (cmd.args[++i])
	{
		if (i != index && i != index + 1)
		{
			res[j] = ft_strdup(cmd.args[i]);
			if (!res[j])
			{
				parsing_error(MALLOC, "", env);
				return (false);
			}
			j++;
		}
	}
	return (true);
}

static char	**args_to_tab_without_hd(t_command cmd, intmax_t index,
	char ***env)
{
	char	**res;

	res = ft_calloc(args_len(cmd), sizeof(char *));
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
	if (!command_exec_set(cmd, tmp, args_len(*cmd) - 1))
	{
		ft_free_nt_tab(tmp, args_len(*cmd));
		return (false);
	}
	ft_free_nt_tab(tmp, args_len(*cmd));
	free((*cmd).path);
	free((*cmd).infile);
	(*cmd).path = ft_strdup((*cmd).args[1]);
	(*cmd).infile = ft_strdup("<<");
	if (!(*cmd).path || !(*cmd).infile)
		return (false);
	return (true);
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
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	i[0] = -1;
	while (c[++i[0]].infile)
	{
		res[i[0]] = -1;
		i[1] = -1;
		while (c[i[0]].args[++i[1]])
		{
			if (c[i[0]].args[i[1]][0] == '<'
				&& c[i[0]].args[i[1]][1] == '<')
			{
				if (res[i[0]] != -1)
					return (heredoc_error(res, c[i[0]].args[i[1]], env));
				res[i[0]] = i[1];
			}
		}
	}
	return (res);
}
