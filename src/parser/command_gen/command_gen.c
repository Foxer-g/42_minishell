#include "minishell.h"
#include "parser.h"

static int32_t	tkn_cat(t_token tkn)
{
	if ((tkn.type >= S_QUOTE && tkn.type <= ENV_DESC) || tkn.type == HEREDOC)
		return (3);
	return (tkn.type);
}

static uint64_t	command_len(t_token *tkns)
{
	uint64_t	res;
	uint64_t	i;

	res = 0;
	i = 0;
	while (tkns[i])
	{
		res++;
		while (tkns[i + 1] && tkn_cat(tkns[i]) == tkn_cat(tkns[i + 1]))
			i++;
		i++;
	}
	return (res);
}

// @doc command_gen
// @kind func
// @desc Generate a [[t_command]] array from the token array.
// @param input: [[t_token]], Tokenized user input.
// @return [[t_commnad]], A command array to be organized.

t_command	*command_gen(t_token *tkns)
{
	uint64_t	i;
	uint64_t	j;
	uint64_t	k;
	t_command	*res;

	i = 0;
	k = 0;
	res = ft_calloc((command_len(tkns) + 1), sizeof(t_command));
	if (!res)
		return (NULL);
	while (tkns[i])
	{
		j = i + 1;
		while (tkns[j] && tkn_cat(tkns[i]) == tkn_cat(tkns[j]))
			j++;
		res[k] = init_command();
		command_exec_set(&res[k], tkns[i], j - i);
		k++;
		i = j;
	}
	return (res);
}
