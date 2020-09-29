#include <stdio.h> 
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#define SIZE 5

void mypipe(){
    int pipefd[2];
    char *buf=calloc(SIZE,1);

    int pid=fork(); 
    int child_status;
    char* msg = "hello";

    if(pid==-1) {
        perror("Fork error");
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    if(pid==0){
    write(pipefd[1], msg, SIZE);
    }
 
    waitpid(pid, NULL, 0); 
    close(pipefd[1]);
    read(pipefd[0], &buf, SIZE);
    printf("%s\n",&buf); 
    exit(0);
    
}


int main(int argc, char *argv[])
{
    mypipe();
}