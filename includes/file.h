/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:09:10 by rboutelo          #+#    #+#             */
/*   Updated: 2026/03/17 05:44:34 by rboutelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H
# include <fcntl.h>
# ifdef AUTHORIZED_READV
#  include <sys/uio.h>
# else //AUTHORIZED_READV
#  include "syscalls.h"
# endif //AUTHORIZED_READV
# include "libft.h"

# ifdef TRUE_FILE
// Values used in musl
#  ifndef UNGET
#   define UNGET 8
#  endif //UNGET
#  ifndef BUFSIZE
#   define BUFSIZE 1024
#  endif //BUFSIZE
#  ifndef EOF
#   define EOF -1
#  endif //EOF
// Permissions
#  ifndef F_NORD
#   define F_NORD 4
#  endif //F_NORD
#  ifndef F_NOWR
#   define F_NOWR 8
#  endif //F_NOWR
#  ifndef F_EOF
#   define F_EOF 16
#  endif //F_EOF
#  ifndef F_ERR
#   define F_ERR 32
#  endif //F_ERR
#  ifndef F_APP
#   define F_APP 128
#  endif //F_APP
# endif //TRUE_FILE

# define WE 1
# define RE 0

typedef int				t_ffile;
typedef struct s_iovec	t_iovec;

void		clear_filelist(void);
void		ft_ffclose(t_ffile file);
void		close_pipe(int32_t *fds);
t_ffile		ft_ffopen(char *filename, const char *mode);
intmax_t	ft_ffread(t_ffile file, void *buf, intmax_t nbyte);
intmax_t	ft_ffreadall(t_ffile file, void **buf);
intmax_t	ft_ffwrite(t_ffile file, const void *buf, intmax_t nbyte);
t_ffile		to_ffile(t_ffile fd);
#endif //FILE_H
