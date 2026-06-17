discard """
  action: "run"
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

import
    ../[libft, minishell],
    std/[envvars, strutils]


let
  tests: seq[(cint, bool)] = @[
    (1, false),
    (1, true)
  ]


var
  tmp: seq[string] = newSeq[string]()
  cenv: cstringArray
  marked_as_export_env: cstringArray


for k, v in envPairs():
  tmp.add(k & "=" & v)
cenv = allocCStringArray(tmp)

marked_as_export_env = allocCStringArray(readFile("tests/builtins/test_marked_as_export").split(":"))


{.compile("src/exec/builtins/env.c", "-Iincludes -Og -g3").}
proc builtin_env(ac: cint, ev: cstringArray) {.importc:"env".}


for test in tests:
    if not test[1]:
        builtin_env(test[0], cenv)
    else:
        builtin_env(test[0], marked_as_export_env)

deallocCStringArray(cenv)
deallocCStringArray(marked_as_export_env)
