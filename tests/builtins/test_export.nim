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


var
  tmp: seq[string] = newSeq[string]()
  marked_as_export_env: cstringArray
  env: cstringArray


for k, v in envPairs():
  tmp.add(k & "=" & v)
env = allocCStringArray(tmp)


marked_as_export_env = allocCStringArray(readFile("tests/builtins/test_marked_as_export").split(":"))


let
  tests: seq[(cint, cstringArray, bool)] = @[
    (1, allocCStringArray(["export"]), false),
    (1, allocCStringArray(["export"]), true)
  ]


{.compile("src/exec/builtins/export.c", "-Iincludes").}
proc builtin_export(ac: cint, av: cstringArray, env: var cstringArray) {.importc:"export".}

for test in tests:
  echo "TEST NUMBER " & $ tests.find(test)
  if not test[2]:
    builtin_export(test[0], test[1], env)
  else:
    builtin_export(test[0], test[1], marked_as_export_env)

deallocCStringArray(env)
deallocCStringArray(marked_as_export_env)
