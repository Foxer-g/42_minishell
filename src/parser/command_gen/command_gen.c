
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

static bool	cmpnd_qt_cntnt(intmax_t i[3], t_token *o, t_token *res, char ***env)
{
	char			*tmp;
	t_token_type	res_type;

	tmp = ft_strdup(o[i[0]].content);
	if (!tmp)
		return (!parsing_error(MALLOC, " : command_gen.c : cmpnd_qt_cntnt : tmp", env));
	if (o[i[0]].type == DQUOTE || o[i[0]].type == QUOTE)
	{
		i[2] = o[i[0]].type;
		i[0]++;
		while (((intmax_t) o[i[0]].type) != i[2])
			tmp = ft_extend(tmp, o[i[0]++].content);
		if (!tmp)
			return (!parsing_error(MALLOC, " : command_gen.c : cmpnd_qt_cntnt : tmp 2", env));
		tmp = ft_extend(tmp, &o[i[0]].content[0]);
		if (!tmp)
			return (!parsing_error(MALLOC, " : command_gen.c : cmpnd_qt_cntnt : tmp 3", env));
		res_type = COMMAND;
	}
	else
		res_type = o[i[0]].type;
	res[i[1]++] = (t_token){tmp, res_type};
	return (true);
}

bool	quote_join(t_token **tkns, char ***env)
{
	t_token		*res;
	intmax_t	i[3];

	ft_bzero(i, sizeof(i));
	res = ft_calloc(1, sizeof(t_token));
	if (!res)
		return (!parsing_error(MALLOC, " : command_gen.c : quote_join : res", env));
	while ((*tkns)[i[0]].content)
	{
		res = ft_recalloc(res, (i[1] + 1) * sizeof(t_token), i[1] + 2, sizeof(t_token));
		if (!res)
			return (!parsing_error(MALLOC, " : command_gen.c : quote_join : res 2", env));
		if (!cmpnd_qt_cntnt(i, *tkns, res, env))
		{
			free_token(res);
			return (false);
		}
		i[0]++;
	}
	free(*tkns);
	*tkns = res;
	return (true);
}

// @doc command_gen
// @kind func
// @desc Generate a [[t_command]] array from the token array.
// @param input: [[t_token]], Tokenized user input.
// @return [[t_commnad]], A command array to be organized.

t_command	*command_gen(t_token **tkns, char ***env)
{
	intmax_t	i[2];
	t_command	*res;
	char		**tmp;

	if (!quote_join(tkns, env))
		return (NULL);
	res = ft_calloc(command_len(*tkns) + 1, sizeof(t_command));
	if (!res)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen.c : command_gen : res", env)));
	ft_bzero(i, sizeof(i));
	while ((*tkns)[i[0]].content)
	{
		res[i[1]] = init_command(env);
		tmp = tkn_to_tab(*tkns, i[0], tkn_len_since(*tkns, i[0]), env);
		if (!tmp || !res[i[1]].infile)
		{
			free_command(res);
			if (tmp)
				ft_free_nt_tab(tmp, tkn_len_since(*tkns, i[0]));
			return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen.c : command_gen : tmp || res[j]", env)));
		}
		command_exec_set(&res[i[1]], tmp, tkn_len_since(*tkns, i[0]));
		ft_free_nt_tab(tmp, tkn_len_since(*tkns, i[0]));
		if (!res[i[1]++].infile)
		{
			free_command(res);
			return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen.c : command_gen : res[j] by command_exec_set", env)));
		}
		i[0] += tkn_len_since(*tkns, i[0]);
	}
	return (res);
}
