import
  libft

type
  t_command* {.bycopy.} = object
    path*: cstring
    arguments*: cstringArray
    pid*: __pid_t


proc execute*(cmd: t_command; outfd: t_ffile; infd: t_ffile; env: cstringArray): cchar {.importc.}
