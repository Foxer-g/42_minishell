#include "minishell.h"
#include "parser.h"

t_command	cmd_dup(t_command cmd, char ***env)
{
	t_command	res;

	res = init_command(env);
	command_exec_set(&res, cmd.args, args_len(cmd));
	free(res.infile);
	free(res.outfile);
	res.infile = ft_strdup(cmd.infile);
	if (!res.infile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.outfile = ft_strdup(cmd.outfile);
	if (!res.outfile)
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	res.append = cmd.append;
	if (!res.infile || !res.outfile)
	{
		free_command(&res);
		return ((t_command){.append = !parsing_error(MALLOC, "", env)});
	}
	return (res);
}

t_command	*full_cmd_dup(t_command *src, char ***env)
{
	intmax_t	i;
	t_command	*res;

	res = ft_calloc(cmd_len(src) + 1, sizeof(t_command));
	if (!res)
	{
		parsing_error(MALLOC, "", env);
		return (NULL);
	}
	i = 0;
	while (src[i].infile)
	{
		res[i] = cmd_dup(src[i], env);
		if (!res[i].infile)
		{
			free_command(res);
			parsing_error(MALLOC, "", env);
			return (NULL);
		}
		i++;
	}
	return (res);
}

char	**cmddup_without_redir(t_command cmd, t_redir *redir, char ***env)
{
	char		**res;
	intmax_t	i;
	intmax_t	j;

	i = -1;
	j = 0;
	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	while (cmd.args[++i])
	{
		if (!((redir[0].index && (i == redir[0].index
						|| i == redir[0].index + 1)) || ((redir[1].index
						&& (i == redir[1].index || i == redir[1].index + 1)))))
		{
			res[j] = ft_strdup(cmd.args[i]);
			if (!res[j++])
			{
				ft_free_nt_tab(res, j);
				return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
			}
		}
	}
	return (res);
}
