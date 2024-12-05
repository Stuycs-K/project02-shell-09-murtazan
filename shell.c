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
// Error Messages
int err(char * input){;
    printf("%s: %s\n", input, strerror(errno));
}

void lsCmd(char * cmdInput){ //from lab08
  char input[100];
  int inputLen;
  if (cmdInput == NULL){
    strcpy(input, ".");
  }else{
    inputLen = strlen(cmdInput);
    strncpy(input, cmdInput, inputLen - 1);
  }
  //null terminate the new copy
  input[inputLen - 1] = '\0';
  //set dir
  DIR * d;
  DIR * d2;
  DIR * d3;
  char * PATH = input;
  d = opendir(PATH);
  d2 = opendir(PATH);
  d3 = opendir(PATH);
  if (d == NULL){
    err(input);
    return;
  }
  if (d2 == NULL) err(input);
  if (d3 == NULL) err(input);
  printf("Statistics for directory: %s\n", input);
  struct dirent *dirEntry;
  int numBytes = 0;
  int currFile;
  char buff[129];
  buff[128] = 0;
  while ( (dirEntry = readdir(d3)) ){
    //gather length of full curr PATH
    char * currPath = malloc(strlen(PATH) + strlen(dirEntry->d_name) + 1);
    if (strcmp(PATH, ".") != 0){
      currPath = strcpy(currPath, PATH);
      currPath = strcat(currPath, "/");
      currPath = strcat(currPath, dirEntry->d_name);
    }else{
      currPath = dirEntry->d_name;
    }
    if (dirEntry->d_type == DT_REG){
      currFile = open(currPath, O_RDONLY, 0);
      if (currFile == -1) err(currPath);
      int bytes;
      while ( (bytes = read(currFile, buff, 128)) ){
        if (bytes == -1) err(currPath);
        numBytes += bytes;
      }
    }
  }
  printf("Total Directory Size: %u Bytes\n", numBytes);
  printf("Directories:\n");
  while( (dirEntry = readdir(d)) ){
    if (dirEntry->d_type == DT_DIR){
      printf("  %s\n", dirEntry->d_name);
    }
  }
  printf("Regular files:\n");
  while( (dirEntry = readdir(d2)) ){
    if (dirEntry->d_type == DT_REG){
      printf("  %s\n", dirEntry->d_name);
    }
  }
  closedir(d);
}

void cdCmd(char * cmdInput){
  char input[100];
  int inputLen;
  if (cmdInput == NULL){
    strcpy(input, getenv("HOME"));
  }else{
    inputLen = strlen(cmdInput);
    strncpy(input, cmdInput, inputLen - 1);
  }
  //null terminate the new copy
  input[inputLen - 1] = '\0';
  char * PATH = input;
  if (chdir(PATH) == -1) err(input);
  // char cwd_buff[100];
  // getcwd(cwd_buff, 100);
  // printf("current dir: %s\n", cwd_buff);
}

void processInput(char * input_buff){
    char * src = input_buff;
    char * cmd;
    char * arg1;
    cmd = strsep(&src, " ");
    arg1 = strsep(&src, " ");
    if (strncmp("ls", cmd, 2) == 0){
      lsCmd(arg1);
    }
    if (strncmp("cd", cmd, 2) == 0){
      cdCmd(arg1);
    }
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
