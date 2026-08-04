/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   exec_int.h                                          ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/05/19 17:30:32 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/08/04 22:27:58 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_INT_H
# define EXEC_INT_H
# include "libft.h"

typedef struct s_command	t_command;

// @doc t_quotetype
// @kind type
// @desc Enum that represents the type of the currently applied quote.
// @field NOT: 0, No quote is in action.
// @field DBL: ", A double quote is in action.
// @field SGL: ', A single quote is in action.
typedef enum e_quotetype
{
	NOT = 0,
	DBL = '"',
	SGL = '\''
}	t_quotetype;
typedef int (*t_cmd_fun)(t_command, char ***);

int8_t	entrypoint(t_command **cmds, char ***env, bool early_stop);
int8_t	exec_single(t_command *command, char ***env);
int8_t	execute(t_command cmd, t_ffile out, t_ffile in, char **env);

/* ************ */
/*   BUILTINS   */
/* ************ */
int		cd(t_command cmd, char ***env);
int		echo(t_command cmd, char ***env);
int		minishell_env(t_command cmd, char ***env);
int		minishell_exit(t_command cmd, char ***env);
int		export(t_command cmd, char ***env);
int		pwd(t_command cmd, char ***env);
int		unset(t_command cmd, char ***env);
#endif // EXEC_INT_H
