import
  libft

type
  t_ffile = cint
  int8_t = cchar
  t_command* {.bycopy.} = object
    path*: cstring
    args*: cstringArray
    pid*: cint
    infd*: t_ffile
    outfd*: t_ffile
    append*: bool
    infile*: cstring
    outfile*: cstring

##  @doc t_quotetype
##  @kind type
##  @desc Enum that represents the type of the currently applied quote.
##  @field NOT: 0, No quote is in action.
##  @field DBL: ", A double quote is in action.
##  @field SGL: ', A single quote is in action.

type
  t_quotetype* = enum
    NOT = 0, DBL = '\"', SGL = '\''
  t_cmd_fun* = proc (a1: t_command; a2: ptr cstringArray): cint


proc entrypoint*(cmds: ptr ptr t_command; env: ptr cstringArray; early_stop: bool): int8_t {.importc.}
proc exec_single*(command: ptr t_command; env: ptr cstringArray): int8_t {.importc.}
proc execute*(cmd: t_command; `out`: cint; `in`: cint; env: cstringArray): int8_t {.importc.}
##  ************
##    BUILTINS
##  ************

proc cd*(path: cstring; env: cstringArray): cint {.importc.}
proc echo*(ac: cint; av: cstringArray; `out`: t_ffile): cint {.importc.}
proc env*(ac: cint; ev: cstringArray): cint {.importc.}
proc minishell_exit*(ac: cint; av: cstringArray): cint {.importc.}
proc `export`*(ac: cint; av: cstringArray; env: ptr cstringArray): cint {.importc.}
proc pwd*(ac: cint): cint {.importc.}
proc unset*(vars: cstringArray; env: ptr cstringArray): cint {.importc.}
