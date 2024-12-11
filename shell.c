#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>

// int err(char * input)
// used primarily for errors found in 'void cdCmd(char * cmdInput)'
// 'char * input' takes in stdin input from 'void cdCmd(char * cmdInput)' in order to display in a user-friendly way an error message
// returns nothing, because it simply prints an error.
void err(char * input){;
    printf("%s: %s\n", input, strerror(errno));
}

// void cdCmd(char * cmdInput)
// the 'cd' command of the shell, used to change directories, if said directory exists.
// returns nothing, because this command is used to change directories.
// 'char * cmdInput' takes in an input from 'void processInput(char * input_buff)'; this determines the directory that 'void cdCmd(char * cmdInput)' will change into.
void cdCmd(char * cmdInput){
  char input[100];
  int inputLen;
  if (cmdInput == NULL || strncmp(cmdInput, "~", 1) == 0){
    strcpy(input, getenv("HOME"));
  }else{
    inputLen = strlen(cmdInput);
    strcpy(input, cmdInput);
  }
  char * PATH = input;
  if (chdir(PATH) == -1) err(input);
  // char cwd_buff[100];
  // getcwd(cwd_buff, 100);
  // printf("current dir: %s\n", cwd_buff);
}

// void processInput(char * input_buff)
// parses stdin into multiple commands and arguments for easy execvp'ing and usage of 'void cdCmd(char * cmdInput)'.
// this function also does the execvp'ing itself.
// returns nothing, because this command runs other commands within it.
// 'char * input_buff' receives the input from stdin in 'int main()'.
void processInput(char * input_buff){
    char * cmds[100];
    char * src = input_buff;
    src[strcspn(src, "\n")] = 0;
    char * cmd;
    cmd = strsep(&src, ";");
    int cmdsIndex = 0;
    while (cmd != NULL){ //list commands in an array. [;]
        cmds[cmdsIndex] = cmd;
        cmdsIndex += 1;
        cmd = strsep(&src, ";");
    }
    // process each command after parsing arguments
    int cmdsIter = 0;
    while (cmdsIter < cmdsIndex){
      char * args[100];
      char * arg;
      arg = strsep(&cmds[cmdsIter], " ");
      int argsIndex = 0;
      while (arg != NULL){
        args[argsIndex] = arg;
        argsIndex += 1;
        arg = strsep(&cmds[cmdsIter], " ");
      }
      int EXECFORK = 0;
      if (strcmp(args[0], "cd") == 0){
        // if (args[2] != NULL && args[2] != ""){
        //   printf("cd: too many arguments! args[2]: %s\n", args[2]);
        //   TOOMANYARGS = 1;
        // }else{
          cdCmd(args[1]);
          EXECFORK = 1;
        // }
      }
      if (EXECFORK != 1){
        pid_t pid;
        pid = fork();
        if (pid <= -1){
          perror("fork fail\n");
          exit(1);
        }else if (pid == 0){
          execvp(args[0], args);
          perror("execvp fail\n");
          exit(1);
        }else{
          wait(NULL);
          cmdsIter += 1;
        }
      }else{
        cmdsIter += 1;
      }
    }
    // cmd = strsep(&src, " ");
    // arg1 = strsep(&src, " ");
    // if (strncmp("ls", cmd, 2) == 0){
    //   lsCmd(arg1);
    // }
    // if (strncmp("cd", cmd, 2) == 0){
    //   cdCmd(arg1);
    // }
    // if (strncmp("exit", cmd, 4) == 0){
    //     exit(1);
    // }
}

// int main()
// the frontend: prints the current working directory and displays an input for users to provide.
// 'int argc' is not used for anything. 'char *argv[]' is used to determine if the shell was ran with a command to run prematurely.
// returns 0.
int main(int argc, char *argv[]){
    char input_buff[100];
    char cwd_buff[100];
    while (true){
        getcwd(cwd_buff, 100);
        printf("%s $ ", cwd_buff);
        fgets(input_buff,100,stdin);
        processInput(input_buff);
    }
    return 0;
}
