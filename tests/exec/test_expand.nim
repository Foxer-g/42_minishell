discard """
  action: "compile"
  exitcode: 0

  # On Linux 64-bit machines, whether to use Valgrind to check for bad memory
  # accesses or memory leaks. On other architectures, the test will be run
  # as-is, without Valgrind.
  # Options:
  #   true: run the test with Valgrind
  #   false: run the without Valgrind
  #   "leaks": run the test with Valgrind, but do not check for memory leaks
  valgrind: true

  # Targets to run the test into (c, cpp, objc, js). Defaults to c.
  targets: "c"

  # Conditions that will skip this test. Use of multiple "disabled" clauses
  # is permitted.
  disabled: "win"
"""

import ../libft

type
  Pid {.importc: "pid_t", header:"<sys/types.h>".} = object
  t_command = object
    path: cstring
    arguments: cstringArray
    pid: Pid
    infd: cint
    outfd: cint
    append: bool
    infile: cstring
    outfile: cstring
  Pt_command = ptr t_command

{.compile("src/exec/entrypoint.c", "-Iincludes"), compile("src/exec/cmd_handler.c", "-Iincludes"), passL:"-L libft -lft".}
proc expand(cmd: Pt_command, env: cstringArray): bool {.importc.}
proc free(tgt: cstring) {.importc:"free", header:"stdlib".}

let
  env: cstringArray = allocCStringArray([$(cstring "FOO=me")])

var
  commands: seq[t_command] = @[]
  args: seq[string] = @[]

args.add("echo")
args.add("hello $FOO")

commands.add(t_command(
  arguments: allocCStringArray(args.toOpenArray[:string](0, args.len - 1))
))


for command in commands:
    discard expand(addr command, env)
    deallocCStringArray(command.arguments)
    deallocCStringArray(env)
