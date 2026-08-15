/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 23:36:34 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/15 06:15:05 by rboutelo         ###   ########.fr       */
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
