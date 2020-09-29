    #include <linux/limits.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include "LineParser.h"
    #include <sys/types.h>
    #include <wait.h>
    # include <string.h> 
    #define MAXINPUT 2048

    typedef struct list
     {
    char * name; 
    char * value;				
    struct list *next;	
    } list;

    int debug=0;
    int cd=0;
    char in[MAX_INPUT];
    int upper=0;
    list *first;

    void debug_print(cmdLine *cmd, pid_t pid){
        if(pid!=0){
        fprintf(stderr,"%s" "%d\n", "return value is ", pid);    
        fprintf(stderr,"%s" "%s\n", "The command is ", cmd->arguments[0]);
        }
    }

    void cmd(cmdLine *cmdLine){
        if(strncmp(cmdLine->arguments[1],"~",1)==0){
            chdir(getenv("HOME"));
        }
        else{
        int result =chdir(cmdLine->arguments[1]);
        if(result!=0){
            perror("no such directory");
          }
        }
    }

    void set(cmdLine *cmdLine){
       int replace=0;
       if(first==NULL){
           list *newfirst=calloc(1,sizeof(list));
           newfirst->name=cmdLine->arguments[1];
           newfirst->value=cmdLine->arguments[2];
           first=newfirst;
           replace=1;
       }
       else{
           list *temp=first;
       while (temp->next!=NULL)
       {
           if(strcmp(temp->name,cmdLine->arguments[1])==0){
               temp->value=cmdLine->arguments[2];
               replace=1;
               break;
           }
           temp=temp->next;
       }
       }
       if(!replace){
           list *temp=first;
           while (temp->next!=NULL)
           {
               temp=temp->next;
           }
           list *add=calloc(1,sizeof(struct list));
           add->name=cmdLine->arguments[1];
           add->value=cmdLine->arguments[2];
           temp->next=add;
       }       
    }

    void var(){
         list *temp=first;
        while (temp!=NULL)
        {
            printf("Name = %s \t\t Value = %s\n", temp->name , temp->value);
            temp=temp->next;
        }
        
    }

    char* find_value(char* name){
        list *temp=first;
        while (temp!=NULL)
        {
            if(strcmp(name,temp->name)==0)
               return temp->value;
            temp=temp->next;
        }
        perror("value was not found");
        exit (1);
    }
  
    void execute(struct cmdLine *pCmdLine) {

        for(int i=0;i<pCmdLine->argCount;i++){
            if(strncmp(pCmdLine->arguments[i],"$",1)==0){
            char* new=find_value(pCmdLine->arguments[i]+1);
            replaceCmdArg(pCmdLine,i,new);
            }
        }
        
        if (strcmp(pCmdLine->arguments[0], "cd") == 0)   
           cmd(pCmdLine); 

        else if (strncmp(pCmdLine->arguments[0], "set",3) == 0) {  
           set(pCmdLine);
        }

        else if (strncmp(pCmdLine->arguments[0], "var",3) == 0) {  
           var();
        }
        
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

        while(1) {
        fgets (in, MAX_INPUT, stdin);   
        if(feof(stdin)|| memcmp(in,"quit",4)==0)
            exit(0);
        cmd= parseCmdLines(in); 
        execute(cmd);        
        }
        freeCmdLines(cmd); 
    }

                
