#include "minishell.h"
#include "parser.h"

char	**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env)
{
	intmax_t	k;
	intmax_t	end;
	char		**res;

	k = 0;
	end = i + len;
	res = ft_calloc(len + 1, sizeof(char *));
	if (!res)
		return ((void *)((uintptr_t)parsing_error(MALLOC, "", env)));
	while (i < end)
	{
		res[k] = ft_strdup(tkns[i++].content);
		if (!res[k])
			return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
		k++;
	}
	return (res);
}

intmax_t	count_space(char *str)
{
	intmax_t	i;
	intmax_t	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			res++;
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
		return (!parsing_error(MALLOC, "", env));
	if (o[i[0]].type == DQUOTE || o[i[0]].type == QUOTE)
	{
		i[2] = o[i[0]].type;
		i[0]++;
		while (((intmax_t) o[i[0]].type) != i[2])
			tmp = ft_extend(tmp, o[i[0]++].content);
		if (!tmp)
			return (!parsing_error(MALLOC, "", env));
		tmp = ft_extend(tmp, &o[i[0]].content[0]);
		if (!tmp)
			return (!parsing_error(MALLOC, "", env));
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
		return (!parsing_error(MALLOC, "", env));
	while ((*tkns)[i[0]].content)
	{
		res = ft_recalloc(res, (i[1] + 1) * sizeof(t_token), i[1] + 2, sizeof(t_token));
		if (!res)
			return (!parsing_error(MALLOC, "", env));
		if (!cmpnd_qt_cntnt(i, *tkns, res, env))
		{
			free_token(res);
			return (false);
		}
		i[0]++;
	}
	free_token(*tkns);
	*tkns = res;
	return (true);
}
