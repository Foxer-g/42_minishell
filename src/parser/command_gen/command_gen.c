
#include "minishell.h"
#include "parser.h"

static int32_t	tkn_cat(t_token tkn)
{
	if (tkn.type == PIPE)
		return (1);
	return (2);
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
		while (tkns[i + 1].content && tkn_cat(tkns[i]) == tkn_cat(tkns[i + 1]))
			i++;
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
	uintmax_t	i[3];
	t_command	*res;
	char		**tmp;

	ft_bzero(i, sizeof(i));
	if (!quote_join(tkns, env))
		return (NULL);
	res = ft_calloc(command_len(*tkns) + 1, sizeof(t_command));
	if (!res)
		return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen.c : command_gen : res", env)));
	while ((*tkns)[i[0]].content)
	{
		i[1] = i[0] + 1;
		while ((*tkns)[i[1]].content
			&& tkn_cat((*tkns)[i[0]]) == tkn_cat((*tkns)[i[1]]))
			i[1]++;
		res[i[2]] = init_command(env);
		tmp = tkn_to_tab(*tkns, i[0], i[1] - i[0], env);
		if (!tmp)
		{
			free_command(res);
			return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen.c : command_gen : tmp", env)));
		}
		command_exec_set(&res[i[2]++], tmp, i[1] - i[0]);
		i[0] = i[1];
	}
	return (res);
}
