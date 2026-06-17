import
  libft, file

const
  WE* = 1
  RE* = 0

type
  pid_t = cint
  t_command* {.bycopy.} = object
    path*: cstring
    arguments*: cstringArray
    pid*: pid_t
    infd*: t_ffile
    outfd*: t_ffile
    infile*: cstring
    outfile*: cstring

{.passL: "wbuild/src/libminish.a libft/libft.a".}
proc execute*(cmd: t_command; `out`: t_ffile; `in`: t_ffile; env: cstringArray): cchar {.importc.}
proc command_to_args*(command: cstring): cstringArray {.importc.}
proc entrypoint*(cmds: ptr ptr t_command; env: cstringArray) {.importc.}
proc exec_single*(command: ptr t_command; env: cstringArray): cchar {.importc.}

proc free*(pt: pointer) {.importc.}
