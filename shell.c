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

void processInput(char * input_buff){
    printf("%s", input_buff);
}

int main(){
    char input_buff[100];
    char cwd_buff[100];
    getcwd(cwd_buff, 100);
    while (true){
        printf("%s $ ", cwd_buff);
        fgets(input_buff,100,stdin);
        processInput(input_buff);
    }
    return 0;
}
