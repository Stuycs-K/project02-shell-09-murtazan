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

// stuff to do:
// - reorganize the entire code to be in seperate files (one file for parsing? one for all the commands? you can figure it out)
// - extension of above, remake parsing in order to be able to parse multi-commands and multi-arguments better.
// - fork and exec [higher priority?]
// - >, < and | (pipes)
// - you should be done at this point. have fun trying other commands (cat? touch? pwd? doesn't have to be extremely difficult ones)

// Error Messages
int err(char * input){;
    printf("%s: %s\n", input, strerror(errno));
}

void cdCmd(char * cmdInput){
  char input[100];
  int inputLen;
  if (cmdInput == NULL || strncmp(cmdInput, "~", 1) == 0){
    strcpy(input, getenv("HOME"));
  }else{
    inputLen = strlen(cmdInput);
    strncpy(input, cmdInput, inputLen - 1);
    input[inputLen - 1] = '\0';
  }
  char * PATH = input;
  if (chdir(PATH) == -1) err(input);
  // char cwd_buff[100];
  // getcwd(cwd_buff, 100);
  // printf("current dir: %s\n", cwd_buff);
}

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
      int TOOMANYARGS = 0;
      if (strcmp(args[0], "cd") == 0){
        // if (args[2] != NULL && args[2] != ""){
        //   printf("cd: too many arguments! args[2]: %s\n", args[2]);
        //   TOOMANYARGS = 1;
        // }else{
          cdCmd(args[1]);
          break;
        // }
      }
      if (TOOMANYARGS != 1){
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

int main(){
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
