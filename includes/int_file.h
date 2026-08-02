/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_file.h                                          ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 17:14:41 by rboutelo          #+#    #+#             */
/*   Updated: 2026/07/28 19:52:26 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef INT_FILE_H
# define INT_FILE_H
# include <fcntl.h>
# ifdef AUTHORIZED_READV
#  include <sys/uio.h>
# endif
# include "libft.h"
# include "syscalls.h"
# include "types.h"
// Values used in musl
# define UNGET 8
# define BUFSIZE 1024
# ifndef EOF
#  define EOF -1
# endif //EOF
// Permissions
# define F_PERM 1
# define F_NORD 4
# define F_NOWR 8
# define F_EOF 16
# define F_ERR 32
# define F_APP 128
# define WE 1
# define RE 0

void		append_file(t_ffile file);
void		remove_file(t_ffile file);
void		free_file(t_ffile file);
uintmax_t	get_total_iov_len(const t_iovec *iov, uintmax_t cnt);
#endif //INT_FILE_H
