*This project has been created as part of the 42 curriculum by toespino, rboutelo*

# MINISHELL

## As beautiful as a shell

Minishell is a project from 42's Common Core in the third circle

# Description :
Minishell is a minimalist Unix shell

As mainly based on bash this teached us :
> - Tokenization
> - Advanced parsing
> - Scheduling
> - Execution
> - Environement management

And all on to have a clearer idea of what really is the shell that we may daily drive.
The main challenge was to keep readability as much as possible all along the project.

# Instruction :

## Compilation
This project build with :
> - Make
> - Waf build

The project being a part of 42's CC, it must be built using make, however, I also included the waf build system (waf and wscript files) because we find it practical and interesting to use.

To build it you need to execute `make` (or `./waf configure build`, configure needs to be ran at least once but is not necessary for subsequent runs).

## Usage
Once compiled, you can execute it with `./minishell`.

Minishell can be used with or without arguments

If any arguments is given, the program will treat those as a regular input.

*Exemple :*
![]()

If no arguments has been given the program will act as a regular shell by displaying a prompt waiting for instruction.

*Exemple :*
![]()

## Builtin
Minishell have some integrated builtins which are :
 - `echo` : Display the input.
 >  `-n` : this option remove the '\\n' at the end of the output.
 - `cd` : Change the working directory.
 >  Take only relative or absolute path.
 - `pwd` : Print the working directory.
 - `export` : Update an environement variable.
 - `unset` : Remove an environement variable.
 - `env` : Display the content of an environement variable.
 - `exit` : Exit the program with a designated exit code.

 # Ressources :
> - **Bash man** : [manual](https://www.gnu.org/software/bash/manual/bash.html)
> - **Gif creation made with** : [VHS by Charm team](https://github.com/charmbracelet/vhs)

In this project AI has been used to find leaks. 

*special thanks to Dragork for beeing here as an emotional and ruber duck debugging support*
