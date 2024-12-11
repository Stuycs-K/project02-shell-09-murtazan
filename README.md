[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
## Group Member(s):
Naf (Nafiyu Murtaza)
## Creative Team Name:
The Damned
## Features (Implemented):
- any command in shell that comes with execvp
  It's essentially what it sounds like. Thanks to execvp, essentially use any shell command that is an actual program freely.
- cd
  Change the current directory you are in.
## Features (Attempted, Failed):
- Pipes / Redirection
  Kept seg-faulting, so removed it entirely.
- Reading lines from a file
  The whole commands.txt thing.
- Having arguments when running prematurely (i.e ./shell ls ../Directory)
  execvp would fail with a bad address error whenever I tried this. Even when debugging and nothing running it still did this. 
## Bugs:
- any command from 'execvp' works as intended, however error messages show. Please ignore them, as this had something to do with my parsing but attempts to fix it caused issues with 'cd'.
- no command can be used empty: i.e typing just "cd" into the command line will not send you to "HOME". It used to work before execvp, but something about the way I parsed execvp creates empty strings that, when trying to detect using strcmp, fail.
## Function Header(s):
### int err(char * input)
used primarily for errors found in 'void cdCmd(char * cmdInput)'
'char * input' takes in stdin input from 'void cdCmd(char * cmdInput)' in order to display in a user-friendly way an error message
returns nothing, because it simply prints an error.
### void cdCmd(char * cmdInput)
the 'cd' command of the shell, used to change directories, if said directory exists.
returns nothing, because this command is used to change directories.
'char * cmdInput' takes in an input from 'void processInput(char * input_buff)'; this determines the directory that 'void cdCmd(char * cmdInput)' will change into.
### void processInput(char * input_buff)
parses stdin into multiple commands and arguments for easy execvp'ing and usage of 'void cdCmd(char * cmdInput)'.
this function also does the execvp'ing itself.
returns nothing, because this command runs other commands within it.
'char * input_buff' receives the input from stdin in 'int main()'.
### int main()
the frontend: prints the current working directory and displays an input for users to provide.
'int argc' is not used for anything. 'char *argv[]' is used to determine if the shell was ran with a command to run prematurely.
returns 0.
