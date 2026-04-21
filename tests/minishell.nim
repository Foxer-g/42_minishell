import
  libft

type
  pid_t* {.importc: "pid_t", header: "<sys/types.h>".} = object
  t_command* {.bycopy.} = object
    path*: cstring
    arguments*: cstringArray
    pid*: pid_t

{.passL: "./minishell.a ./libft.a".}
proc execute*(cmd: t_command; `out`: t_ffile; `in`: t_ffile; env: cstringArray): cchar {.importc.}
proc command_to_args*(command: cstring): cstringArray {.importc.}
