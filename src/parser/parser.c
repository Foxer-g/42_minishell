/* ************************************************************************** */
/*   parser.c                                            ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: f0xer <f0xer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 05:31:25 by f0xer             #+#    #+#             */
/*   Updated: 2026/05/21 03:32:16 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// @doc parser
// @kind func
// @desc Execute the whole parsing process.
// @param input: char *, User input to parse.
// @param env: char ***, Environement variables.
// @return [[t_command]], The parsed command array.

t_command	*parser(char *input, char ***env)
{
	char		*tmp;
	t_token		*token_lst;
	t_command	*raw_commands;
	t_command	*filtered_commands;

	tmp = ft_strtrim(input, " \t");
	if (!tmp)
		return (NULL);
	token_lst = tokenizer(tmp, env);
	free(tmp);
	if (!token_lst)
		return (NULL);
	if (check_invalid_paterns(token_lst, env))
		return (free_token(token_lst));
	raw_commands = command_gen(&token_lst, env);
	free_token(token_lst);
	if (!raw_commands)
		return (NULL);
	filtered_commands = scheduler(raw_commands, env);
	free_command(raw_commands);
	if (!filtered_commands)
		return (NULL);
	return (filtered_commands);
}
