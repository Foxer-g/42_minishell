#include "minishell.h"
#include "parser.h"

char	**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env)
{
	intmax_t	j;
	intmax_t	k;
	char		**res;

	j = i;
	k = 0;
	res = ft_calloc(len + 1, sizeof(char *));
	if (!res)
		return (parsing_error(MALLOC, "", env));
	while (j < len)
	{
		res[k] = ft_strdup(tkns[j++].content);
		if (!res[k])
			return (!parsing_error(MALLOC, "" , env));
	}
	return (res);
}
