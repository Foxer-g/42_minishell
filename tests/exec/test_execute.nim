discard """
  action: "compile"
  exitcode: 0

  outputsub: '''
testresults
tests
'''

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

import
    ../[libft, minishell],
    std/envvars

let
  commands: array[1, t_command] = [
    t_command(
      path: "/usr/bin/ls",
      arguments: allocCStringArray(["ls", "-a"]),
      infile: "stdin",
      outfile: "stdout"
    )
  ]

var
  tmp: seq[string] = newSeq[string]()
  env: cstringArray

for k, v in envPairs():
  tmp.add(k & "=" & v)
env = allocCStringArray(tmp)

for c in commands:
  assert(0 == int execute(c, STDOUT_FILENO, STDIN_FILENO, env))
