#include "minishell.h"
#include "parser.h"

static int32_t	tkn_len_since(t_token *tkn, intmax_t start)
{
	intmax_t	i;

	i = start;
	while (tkn[i].type && tkn[i].type != PIPE)
		i++;
	if (!(i - start))
		i++;
	return (i - start);
}

static uint64_t	command_len(t_token *tkns)
{
	uint64_t	res;
	uint64_t	i;

	res = 0;
	i = 0;
	while (tkns[i].content)
	{
		res++;
		while (tkns[i++].type == PIPE)
			res++;
		while (tkns[i].content && tkns[i].type != PIPE)
			i++;
	}
	return (res);
}

static t_command	*command_gen_error(t_command *res, char **tmp,
		intmax_t len, char ***env)
{
	free_command(res);
	if (tmp)
		ft_free_nt_tab(tmp, len);
	return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
}

// @doc command_gen
// @kind func
// @desc Generate a [[t_command]] array from the token array.
// @param input: [[t_token]], Tokenized user input.
// @return [[t_commnad]], A command array to be organized.

t_command	*command_gen(t_token **tkns, char ***env)
{
	intmax_t	i[2];
	intmax_t	len;
	t_command	*res;
	char		**tmp;

	if (!quote_join(tkns, env))
		return (NULL);
	res = ft_calloc(command_len(*tkns) + 1, sizeof(t_command));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	ft_bzero(i, sizeof(i));
	while ((*tkns)[i[0]].content)
	{
		len = tkn_len_since(*tkns, i[0]);
		res[i[1]] = init_command(env);
		tmp = tkn_to_tab(*tkns, i[0], len, env);
		if (!tmp)
			return (command_gen_error(res, NULL, 0, env));
		if (!command_exec_set(&res[i[1]], tmp, len))
			return (command_gen_error(res, tmp, len, env));
		ft_free_nt_tab(tmp, len);
		i[0] += len;
		i[1]++;
	}
	return (res);
}
