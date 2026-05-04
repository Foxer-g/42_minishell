/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cli_to_args.c                                              _             */
/*                                                            _ \'-_,#        */
/*   By: neumann </var/spool/mail/neumann>                   _\'--','`|       */
/*                                                           \`---`  /        */
/*   Created: 2026/05/04 05:00:15 by neumann                  `----'`         */
/*   Updated: 2026/05/04 05:00:42 by neumann                                  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint32_t	get_tokens(char *command)
{
	uint8_t		str_context;
	uint32_t	tokens;

	tokens = 0;
	str_context = 0;
	while (command && *command)
	{
		while (*command && (!ft_isspace(*command) || str_context != 0))
		{
			if (*command == '"' || *command == '\'')
			{
				if (*command == str_context)
					str_context = 0;
				else
					str_context = *command;
			}
			command++;
		}
		while (ft_isspace(*command))
			command++;
		tokens++;
	}
	return (tokens);
}

static void	fill_args(char **args, char *command)
{
	char	*last_token;
	uint8_t	str_context;

	last_token = command;
	str_context = 0;
	while (command && *command)
	{
		while (*command && (!ft_isspace(*command) || str_context != 0))
		{
			if (*command == '"' || *command == '\'')
			{
				if (*command == str_context)
					str_context = 0;
				else
					str_context = *command;
			}
			command++;
		}
		*args = ft_strndup(last_token, command - last_token);
		args++;
		while (ft_isspace(*command))
			command++;
		last_token = command;
	}
}

char	**command_to_args(char *command)
{
	char	**result;

	result = ft_calloc(get_tokens(command) + 1, sizeof(char *));
	fill_args(result, command);
	return (result);
}

/*char	**line_to_commands(char *line)
{
	
}*/
