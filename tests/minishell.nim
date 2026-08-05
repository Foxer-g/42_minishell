##  **************************************************************************
##
##                                                         :::      ::::::::
#    minishell.nim                                       ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀     #
##                                                     +:+ +:+         +:+
##    By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+
##                                                 +#+#+#+#+#+   +#+
##    Created: 2026/04/24 01:44:34 by f0xer             #+#    #+#
#    Updated: 2026/08/04 00:59:38 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟            #
##
##  **************************************************************************

import
  libft, file, parser, exec
export libft
export exec

const
  WE* = 1
  RE* = 0
  PROMPT_A* = "\e[22;36;45m"
  PROMPT_B* = "\e[0m @ \e[32;45m"
  PROMPT_C* = "\e[0m $ "

type
  e_builtin* = enum
    CD = '\x00', PWD = 'd', ECHO = 'h', EXIT = 'i', EXPORT = 'p', UNSET = 's', ENV = 'v'


var g_sig_handle*: cint

{.passL: "wbuild/src/libminish.a".}
proc command_to_args*(command: cstring): cstringArray {.importc.}
proc join_tab*(tab: cstringArray): cstring {.importc.}
proc prompt*(aenv: cstringArray): cstring {.importc.}
proc parser*(input: cstring; env: ptr cstringArray): ptr t_command {.importc.}
proc sig_handle*(signal: int32_t; info: pointer; context: pointer) {.importc.}
proc error*(err_message: cstring) {.importc.}
proc expand*(cmd: ptr t_command; env: ptr cstringArray): bool {.importc.}
proc test_print*(input: ptr t_command) {.importc.}
