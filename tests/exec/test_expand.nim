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
  valgrind: false

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
  Pt_command = ref t_command

{.link:"build/exec/entrypoint.o", link:"build/exec/cmd_handler.o", passL:"-L libft -lft".}
proc expand(cmd: Pt_command, env: cstringArray): bool {.importc.}

let
  commands: seq[Pt_command] = @[
    Pt_command(
      arguments: allocCStringArray([$ ft_strdup("echo"), $ ft_strdup("hello $FOO")])
    )
  ]
  env: cstringArray = allocCStringArray([$(cstring "FOO=me")])

discard expand(commands[0], env)
echo t_command
