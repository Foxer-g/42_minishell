##  **************************************************************************
##
##                                                         :::      ::::::::
##    file.h                                             :+:      :+:    :+:
##                                                     +:+ +:+         +:+
##    By: rboutelo <rboutelo@student.42angouleme.f>  +#+  +:+       +#+
##                                                 +#+#+#+#+#+   +#+
##    Created: 2026/02/17 22:09:10 by rboutelo          #+#    #+#
##    Updated: 2026/03/17 05:44:34 by rboutelo         ###   ########.fr
##
##  **************************************************************************

import
  libft

when defined(TRUE_FILE):
  ##  Values used in musl
  when not defined(UNGET):
    const
      UNGET* = 8
  when not defined(BUFSIZE):
    const
      BUFSIZE* = 1024
  when not defined(EOF):
    const
      EOF* = -1
  when not defined(F_NORD):
    const
      F_NORD* = 4
  when not defined(F_NOWR):
    const
      F_NOWR* = 8
  when not defined(F_EOF):
    const
      F_EOF* = 16
  when not defined(F_ERR):
    const
      F_ERR* = 32
  when not defined(F_APP):
    const
      F_APP* = 128
const
  WE* = 1
  RE* = 0

type
  t_ffile* = cint

proc clear_filelist*() {.importc.}
proc ft_ffclose*(file: t_ffile) {.importc.}
proc close_pipe*(fds: ptr cint) {.importc.}
proc ft_ffopen*(filename: cstring; mode: cstring): t_ffile {.importc.}
proc to_ffile*(fd: t_ffile): t_ffile {.importc.}
