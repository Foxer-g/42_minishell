#include "minishell.h"
#include "parser.h"

static int32_t	is_redir(char c1, char c2)
{
	if (c1 == '>' && c2 == '>')
		return (APPEND);
	if (c1 == '<')
		return (I_REDIR);
	if (c1 == '>')
		return (O_REDIR);
	return (0);
}

char	**cmddup_without_redir(t_command cmd, t_redir *redir, char ***env)
{
	intmax_t	i;
	intmax_t	j;
	char		**res;

	i = 0;
	j = 0;
	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	while (cmd.args[i])
	{
		if (i != redir[0].index || i != redir[0].index + 1
			|| i != redir[1].index || i != redir[1].index)
		{
			res[j] = ft_strdup(cmd.args[i]);
			if (!res)
			{
				res = ft_free_nt_tab(res, args_len(cmd) - 2);
				return ((void *) ((uintptr_t)!parsing_error(MALLOC, "", env)));
			}
			j++;
		}
		i++;
	}
	return (res);
}

t_redir	*find_redir(t_command cmd, char ***env)
{
	t_redir		*redir;
	intmax_t	i;
	int32_t		type;

	redir = ft_calloc(2, sizeof(t_redir));
	if (!redir)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, "", env)));
	i = 0;
	while (cmd.args[i++])
	{
		type = is_redir(cmd.args[i - 1][0], cmd.args[i - 1][1]);
		if (type)
		{
			if (i - 1 == 0 || !cmd.args[i - 1] || redir[type != I_REDIR].type)
			{
				free(redir);
				parsing_error(PARSING, cmd.args[i - 1], env);
				return (false);
			}
			redir[type != I_REDIR].type = type;
			redir[type != I_REDIR].index = (++i) - 2;
			redir[type != I_REDIR].file = cmd.args[i - 1];
		}
	}
	return (redir);
}
