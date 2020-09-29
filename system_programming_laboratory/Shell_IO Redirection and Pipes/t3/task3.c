 #include <linux/limits.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/types.h>
    #include <wait.h>

int debug=0;

void mypipeline(){
  int pipefd[2];
  int write=0;
  int read=0;
  char* tail="tail -n 2";
  char* ls="ls -l";
 // int child_a, child_b;
  pid_t child_a=fork();

  if(debug)
     fprintf(stderr,"%s\n","parent_process>forking…");
  if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
   }

  if(debug){
     fprintf(stderr,"%s %d\n","parent_process>created process with id:", child_a);
  }


     if (child_a == 0) {
      dup2(pipefd[1],write);
      if(debug){
        fprintf(stderr,"%s\n","child1>redirecting stdout to the write end of the pipe…");
        fprintf(stderr,"%s\n","child1>going to execute cmd: …");
      }
      close(pipefd[1]);
      execvp("ls",&ls);
    } 

    else {
      pid_t child_b= fork();
      if(child_b==0) {
      if(debug){
     fprintf(stderr,"%s %d\n","parent_process>created process with id:", child_b);
      }
         dup2(pipefd[0],read);
        if(debug){
            fprintf(stderr,"%s\n","child2>redirecting stdin to the write end of the pipe…");
            fprintf(stderr,"%s\n","child2>going to execute cmd: …");
      }
      close(pipefd[0]);
      execvp("tail",&tail);
    }
    else {  
        if(debug)
        fprintf(stderr,"%s\n","parent_process>waiting for child processes to terminate…");
        waitpid(child_a, NULL, 0);
        waitpid(child_b, NULL, 0);
        if(debug)
        fprintf(stderr,"%s\n","parent_process>closing the write end of the pipe…");
        close(pipefd[1]);
        if(debug)
        fprintf(stderr,"%s\n","parent_process>closing the read end of the pipe…");
        close(pipefd[0]);
        fprintf(stderr,"%s\n","parent_process>exiting…");
       }
    }
}



int main(int argc, char *argv[])
{
    for(int i=0;i<argc;i++){
      if ((strncmp(argv[i],"-d",2)==0))
      {
         debug=1;
      }
      
    }
    mypipeline();
}