#include "minishell.h"
#include "parser.h"

// @doc cmd_dup
// @kind func
// @desc Duplicate an inputed [[t_command]].
// @param cmd: [[t_command]], Command to duplicate.
// @param env: char ***, Environement variables.
// @return [[t_command]], Duplicated command.

static bool	cmd_dup_info(t_command *res, t_command cmd)
{
	res->path = NULL;
	if (cmd.path)
		res->path = ft_strdup(cmd.path);
	res->infd = cmd.infd;
	res->outfd = cmd.outfd;
	res->outpipe_end = cmd.outpipe_end;
	res->hd_pipe = cmd.hd_pipe;
	res->append = cmd.append;
	res->builtin = cmd.builtin;
	free(res->infile);
	res->infile = NULL;
	if (cmd.infile)
		res->infile = ft_strdup(cmd.infile);
	free(res->outfile);
	res->outfile = NULL;
	if (cmd.outfile)
		res->outfile = ft_strdup(cmd.outfile);
	if ((cmd.path && !res->path) || (cmd.infile && !res->infile))
		return (false);
	if (cmd.outfile && !res->outfile)
		return (false);
	return (true);
}

t_command	cmd_dup(t_command cmd, char ***env)
{
	t_command	res;
	uint64_t	i;

	res = init_command(env);
	if (!res.infile || !cmd_dup_info(&res, cmd))
		return ((t_command){0});
	if (!cmd.args)
		return (res);
	res.args = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res.args)
		return ((t_command){0});
	i = 0;
	while (cmd.args[i])
	{
		res.args[i] = ft_strdup(cmd.args[i]);
		if (!res.args[i++])
			return ((t_command){0});
	}
	return (res);
}

// @doc full_cmd_dup
// @kind func
// @desc Duplicate an inputed [[t_command]] array.
// @param cmd: [[t_command]] *, Command array to duplicate.
// @param env: char ***, Environement variables.
// @return [[t_command]] *, Duplicated command array.

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

// @doc cmd_dup_witout_redir
// @kind func
// @desc Duplicate an inputed [[t_command]] array without the redirs.
// @param cmd: [[t_command]] *, Command array to duplicate.
// @param redir: [[t_redir]] *, Redirs to remove.
// @param env: char ***, Environement variables.
// @return [[t_command]] *, Duplicated commands.

char	**cmddup_without_redir(t_command cmd)
{
	char		**res;
	intmax_t	i[2];

	res = ft_calloc(args_len(cmd) + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	while (cmd.args[i[0]])
	{
		if (!cmd.args[i[0]][0])
		{
			i[0]++;
			continue ;
		}
		if (is_redir(cmd.args[i[0]][0], cmd.args[i[0]][1]))
		{
			i[0] += 2;
			continue ;
		}
		res[i[1]++] = ft_strdup(cmd.args[i[0]++]);
		if (!res[i[1] - 1])
			return (ft_free_nt_tab(res, i[1]));
	}
	return (res);
}

bool	cmddup_without_empty(t_command *cmd, char ***env)
{
	char		**res;
	intmax_t	i;
	intmax_t	j;

	if (!cmd->args)
		return (true);
	res = ft_calloc(args_len(*cmd) + 1, sizeof(char *));
	if (!res)
		return (!parsing_error(MALLOC, "", env));
	i = -1;
	j = 0;
	while (cmd->args[++i])
	{
		if (!cmd->args[i][0])
			continue ;
		res[j] = ft_strdup(cmd->args[i]);
		if (!res[j++])
		{
			ft_free_nt_tab(res, j);
			return (!parsing_error(MALLOC, "", env));
		}
	}
	if (!command_exec_set(cmd, res, j))
		return (ft_free_nt_tab(res, j));
	return (!ft_free_nt_tab(res, j));
}
