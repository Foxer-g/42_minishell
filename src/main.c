/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   main.c                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: rboutelo  rboutelo@student.42angouleme.fr       ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/08/07 23:46:31 by rboutelo           ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/12 01:38:44 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_handle = 0;

void	sig_init(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(action));
	sigemptyset(&action.sa_mask);
	action.sa_sigaction = sig_handle;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &action, NULL);
	action.sa_handler = SIG_IGN;
	action.sa_flags = 0;
	sigaction(SIGQUIT, &action, NULL);
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
		return (2);
	}
	res = entrypoint(parsed_input, env);
	free_command(parsed_input);
	free(input);
	return (res);
}

static int32_t	main_loop(char *input, char ***env, int32_t res)
{
	if (g_sig_handle == SIGINT)
	{
		free(input);
		return (130);
	}
	if (input[0])
	{
		add_history(input);
		res = minishell_action(input, env);
	}
	else
		free(input);
	return (res);
}

static void main_cleanup(char **env)
{
	rl_clear_history();
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	ft_free_nt_tab(env, ft_nt_tablen((void *)env));
}

static int32_t	main_exit(char **env, int32_t res)
{
	char	*input;

	input = prompt(env);
	while (input && res >= 0)
	{
		if (g_sig_handle == SIGINT)
			res = 130;
		g_sig_handle = 0;
		res = main_loop(input, &env, res);
		if (res < 0)
			break ;
		input = prompt(env);
	}
	if (g_sig_handle == SIGINT)
		res = 130;
	main_cleanup(env);
	if (res < 0)
		return (-(res + 1));
	return (res);
}

int32_t	main(int32_t ac, char **av, const char **aenv)
{
	char	**env;
	char	pathbuf[PATH_MAX];

	sig_init();
	env = ft_copy_env(aenv);
	ft_set_exit_code(0, &env);
	if (!ft_get_env("USER", env))
		ft_set_env("USER", &env, ft_strdup(""));
	if (!ft_get_env("PWD", env))
	{
		getcwd(pathbuf, PATH_MAX);
		ft_set_env("PWD", &env, ft_strdup(""));
	}
	rl_outstream = stderr;
	if (!isatty(STDIN_FILENO))
		rl_prep_term_function = 0;
	if (ac != 1)
	{
		minishell_action(join_tab(++av), &env);
		ft_free_nt_tab(env, ft_nt_tablen((void *)env));
		return (0);
	}
	return (main_exit(env, 0));
}
