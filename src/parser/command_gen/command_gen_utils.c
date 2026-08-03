#include "minishell.h"
#include "parser.h"

char	**tkn_to_tab(t_token *tkns, intmax_t i, intmax_t len, char ***env)
{
	intmax_t	k;
	char		**res;

	k = 0;
	res = ft_calloc(len + 1, sizeof(char *));
	if (!res)
		return ((void *)((uintptr_t)parsing_error(MALLOC, " : command_gen_utils.c : tkn_to_tab : res", env)));
	while (i < len)
	{
		res[k] = ft_strdup(tkns[i++].content);
		if (!res[k])
			return ((void *)((uintptr_t)!parsing_error(MALLOC, " : command_gen_utils.c : tkn_to_tab : res[k]", env)));
		k++;
	}
	return (res);
}
