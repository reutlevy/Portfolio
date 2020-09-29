    #include <linux/limits.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include "LineParser.h"
    #include <sys/types.h>
    #include <wait.h>

    #define MAXINPUT 2048

    int debug=0;
    int cd=0;
    char in[MAX_INPUT];
    int upper=0;

    void debug_print(cmdLine *cmd, pid_t pid){
        if(pid!=0){
        fprintf(stderr,"%s" "%d\n", "return value is ", pid);    
        fprintf(stderr,"%s" "%s\n", "The command is ", cmd->arguments[0]);
        }
    }

    void cmd(cmdLine *cmdLine){
        int result =chdir(cmdLine->arguments[1]);
        if(result!=0){
            perror("no such directory");
          }
    }


    void execute(struct cmdLine *pCmdLine) {

        if (strcmp(pCmdLine->arguments[0], "cd") == 0)   
           cmd(pCmdLine); 
        
        else{  
        pid_t pid=fork();        
        if(debug)
            debug_print(pCmdLine,pid);   
        if(pid==-1) {
            perror("Fork error");
        }
        else if(pid==0){
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            printf("Return not expected. Must be an execv error.n\n");
            exit(1);
        }
        if(pCmdLine->blocking){
            int child_status;            
            waitpid(pid, &child_status, 0);
            if(child_status==-1)
               perror("There was a problem with child proccess");
        }
        printf("%s", "Blocking mode....\n");
        }
    }


    int
    main(int argc, char *argv[])
    {
     //   char *buf;
        cmdLine *cmd;

        int i=0;
        while (i<argc) {
            if (strcmp(argv[i], "-D") == 0) {
                debug=1;
            }
            i++;
        }
      /*  int size = pathconf(".", _PC_PATH_MAX);
        char *ptr;

        if ((buf = (char *)malloc((size_t)size)) != NULL){
             ptr = getcwd(buf, (size_t)size);
        } */
        
        while(1) {
        fgets (in, MAX_INPUT, stdin);   
        if(feof(stdin)|| memcmp(in,"quit",4)==0)
            exit(0);
        cmd= parseCmdLines(in); 
        execute(cmd);        
        }
        freeCmdLines(cmd); 
    }

                
