/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   main.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo  rboutelo@student.42angouleme.fr       ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/07 23:46:31 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/08 06:55:40 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_sig_handle = 0;

void	sig_init(void)
{
	struct sigaction	sig_action;

	sig_action.sa_sigaction = sig_handle;
	sig_action.sa_flags = SA_SIGINFO;
	sigemptyset(&sig_action.sa_mask);
	sigaction(SIGINT, &sig_action, NULL);
	sigaction(SIGQUIT, &sig_action, NULL);
	rl_catch_signals = 0;
}

int32_t	minishell_action(char *input, char ***env)
{
	t_command	*parsed_input;
	int32_t		res;

	parsed_input = parser(input, env);
	if (!parsed_input)
	{
		free(input);
		return (-1);
	}
	res = entrypoint(parsed_input, env);
	test_print(parsed_input);
	free_command(parsed_input);
	free(input);
	return (res);
}

static int32_t	main_exit(char **env, int32_t res)
{
	rl_clear_history();
	ft_printf("exit\n");
	ft_free_nt_tab(env, ft_nt_tablen((void *)env));
	if (res < 0)
		return (-(res + 1));
	return (res);
}

int32_t	main(int32_t ac, char **av, const char **aenv)
{
	char	*input;
	char	**env;
	int32_t	res;

	sig_init();
	env = ft_copy_env(aenv);
	res = 0;
	if (ac != 1)
	{
		minishell_action(join_tab(++av), &env);
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		return (0);
	}
	input = prompt(env);
	while (input)
	{
		res = (g_sig_handle == SIGINT) * 130;
		if (input[0])
		{
			add_history(input);
			res = minishell_action(input, &env);
			if (res < 0)
				break ;
		}
		input = prompt(env);
	}
	return (main_exit(env, res));
}
