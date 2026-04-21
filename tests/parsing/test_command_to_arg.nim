discard """
  action: "compile"
  exitcode: 0
  output: '''
|ls|
|-l|
|echo|
|$PWD|
|awk|
|'$1=="cpu"{id=$5+$6;t=0;for(i=2;i<=NF;i++)t+=$i;s++;if(s==1){p_id=id;p_t=t;next};printf"%.2f%%
",100*(1-((id-p_id)/(t-p_t)))}'|
'''

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

import ../minishell

let tests = [
    command_to_args("ls -l").cstringArrayToSeq(),
    command_to_args("echo $PWD").cstringArrayToSeq(),
    command_to_args("awk '$1==\"cpu\"{id=$5+$6;t=0;for(i=2;i<=NF;i++)t+=$i;s++;if(s==1){p_id=id;p_t=t;next};printf\"%.2f%%\n\",100*(1-((id-p_id)/(t-p_t)))}'").cstringArrayToSeq()
]

var
    i: uint64

for cmd in tests:
    for b in cmd:
        echo "|", b, "|"
        inc(i)
