/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                              ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:09:10 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/08 04:21:14 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
# include "types.h"

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

/* *************** */
/*      OPEN       */
/* *************** */
t_ffile		ft_ffopen(char *filename, const char *mode);
t_ffile		ft_to_ffile(t_ffile fd);

/* *************** */
/*      READ       */
/* *************** */
intmax_t	ft_ffread(t_ffile file, void *buf, intmax_t nbyte);
char		*get_next_line(int fd);
intmax_t	ft_ffreadall(t_ffile file, void **buf);

/* *************** */
/*      WRITE      */
/* *************** */
intmax_t	ft_ffwrite(t_ffile file, const void *buf, intmax_t nbyte);

/* *************** */
/*      CLOSE      */
/* *************** */
void		ft_ffclose(t_ffile *file);
void		ft_close_pipe(t_ffile fds[2]);

/* *************** */
/*     CLEANUP     */
/* *************** */
void		ft_clear_filelist(void);

#endif //FILE_H
