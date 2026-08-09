#include "minishell.h"
#include "parser.h"

void	free_redir(t_redir *redir)
{
	free(redir[0].index);
	free(redir[1].index);
	free(redir[0].file);
	free(redir[1].file);
	free(redir);
}

t_redir	*redir_calloc(intmax_t n, char ***env)
{
	t_redir	*res;

	res = ft_calloc(2, sizeof(t_redir));
	if (!res)
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	res[0].index = ft_calloc(n, sizeof(intmax_t));
	res[1].index = ft_calloc(n, sizeof(intmax_t));
	res[0].file = ft_calloc(n, sizeof(char *));
	res[1].file = ft_calloc(n, sizeof(char *));
	if (!res[0].index || !res[0].file || !res[1].index || !res[1].file)
	{
		free_redir(res);
		return ((void *)((uintptr_t) !parsing_error(MALLOC, "", env)));
	}
	res[0].index[0] = -1;
	res[1].index[0] = -1;
	return (res);
}
