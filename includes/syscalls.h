/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.h                                          ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 23:36:34 by rboutelo          #+#    #+#             */
/*   Updated: 2026/07/28 19:52:01 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALLS_H
# define SYSCALLS_H
# include "int_file.h"
# include "types.h"

intmax_t	ft_readv(t_ffile fd, t_iovec *iov, int32_t cnt);
intmax_t	ft_writev(int fd, const t_iovec *iov, int32_t cnt);
int32_t		ft_fcntl(int fd, int cmd, int flags);

#endif //SYSCALLS_H
