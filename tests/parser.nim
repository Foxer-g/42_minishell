
import
  libft

const
  MALLOC* = "e[31merror : Malloc errore[0m"
  SYNTAX* = "e[31merror : Syntax error near : e[0m"
  PARSING* = "e[31merror : Parsing error near : e[0m"
  INV_CHAR* = "e[31merror : Invalid character : e[0m"
  INV_PIPES* = "e[31merror : Invalid pipe near : e[0m"
  INV_HEREDOC* = "e[31merror : Invalid heredoc near : e[0m"

type
  t_token_type* = enum
    O_PAR, C_PAR, QUOTE, DQUOTE, COMMAND, ENV_DESC, DPIPE, PIPE, DAND, AMP_SAND, SEMI,
    NLINE, I_REDIR, O_REDIR, APPEND, HEREDOC, WILDCARD
  intmax_t = clong
  t_ffile = cint
  uintmax_t = culong
  uint64_t = culong


##  @doc t_token
##  @kind type
##  @desc Token type.
##  @field content: char *, Content of the token.
##  @field type: int32_t, Type of the token.

type
  t_token* {.bycopy.} = object
    content*: cstring
    `type`*: int32_t

  t_redir* {.bycopy.} = object
    `type`*: int32_t
    index*: intmax_t
    file*: cstring

  t_command {.bycopy.} = object
    path*: cstring
    args*: cstringArray
    pid*: cint
    infd*: t_ffile
    outfd*: t_ffile
    append*: bool
    infile*: cstring
    outfile*: cstring

proc parser*(input: cstring; env: ptr cstringArray): ptr t_command {.importc.}
proc check_invalid_paterns*(token_lst: ptr t_token; env: ptr cstringArray): bool {.importc.}
proc parsing_error*(message: cstring; token: cstring; env: ptr cstringArray): bool {.importc.}
proc tokenizer*(input: cstring; env: ptr cstringArray): ptr t_token {.importc.}
proc ft_iscmd_chr*(c: char): bool {.importc.}
proc short_type*(token: cstring): int32_t {.importc.}
proc composed_type*(token: cstring): int32_t {.importc.}
proc free_token*(token_lst: ptr t_token) {.importc.}
proc command_gen*(tkns: ptr t_token; env: ptr cstringArray): ptr t_command {.importc.}
proc tkn_to_tab*(tkns: ptr t_token; i: intmax_t; len: intmax_t; env: ptr cstringArray): cstringArray {.importc.}
proc quote_join*(tkns: ptr ptr t_token; env: ptr cstringArray): bool {.importc.}
proc free_command*(cmds: ptr t_command) {.importc.}
proc init_command*(): t_command {.importc.}
proc cmd_dup*(cmd: t_command): t_command {.importc.}
proc command_exec_set*(command: ptr t_command; cmd: cstringArray; len: uint64_t) {.importc.}
proc command_redir_set*(command: ptr t_command; redir: ptr t_redir): bool {.importc.}
proc scheduler*(raw_command: ptr t_command; env: ptr cstringArray): ptr t_command {.importc.}
proc strtrim_cmd_end*(cmd: ptr t_command; c: char): bool {.importc.}
proc args_len*(cmd: t_command): intmax_t {.importc.}
proc cmd_set_hd*(cmd: ptr t_command; index: intmax_t; env: ptr cstringArray): bool {.importc.}
proc find_heredoc*(cmd: ptr t_command; env: ptr cstringArray): ptr intmax_t {.importc.}
proc is_error*(cmd: ptr t_command; len: intmax_t; env: ptr cstringArray): ptr t_command {.importc.}
proc find_redir*(cmd: t_command; env: ptr cstringArray): ptr t_redir {.importc.}
proc cmddup_without_redir*(cmd: t_command; redir: ptr t_redir; env: ptr cstringArray): cstringArray {.importc.}
proc find_pipe*(cmd: ptr t_command; env: ptr cstringArray): ptr intmax_t {.importc.}
proc is_valid_pipes*(pipes: ptr intmax_t; env: ptr cstringArray): bool {.importc.}
proc cmd_len*(cmd: ptr t_command): intmax_t {.importc.}
