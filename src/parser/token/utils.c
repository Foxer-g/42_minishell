#include "minishell.h"
#include "parser.h"

bool	ft_iscmd_chr(char c)
{
	if (ft_strnstr("$*'\"()|&;\n<>", &c, 12))
		return (false);
	return (true);
}
