#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "LineParser.h"
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
#define MAXINPUT 2048
#define WNOHANG		1
#define WCONTINUED 8

int debug=0;
int cd=0;
char in[MAX_INPUT];
int upper=0;
int index1=0;


    void debug_print(cmdLine *cmd, pid_t pid){
        if(pid!=0){
        fprintf(stderr,"%s" "%d\n", "return value is ", pid);    
        fprintf(stderr,"%s" "%s\n", "The command is ", cmd->arguments[0]);
        }
    }


typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void updateProcessStatus(process* process_list, int pid, int status);

//parts of this code taking from man guide https://linux.die.net/man/2/waitpid
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *add=calloc(1,sizeof(struct process));
    add->cmd=cmd;
    add->pid=pid;
    int status1, current;
    status1=waitpid(pid, &current, WNOHANG);
    if(status1==0)  // his status didnt change- he is still runing
        add->status=RUNNING;
    else { //his status changed so we need to check what his status now
        for(int i=0;i<index1;i++){
            if(process_list[i]!=NULL)
                 updateProcessStatus(process_list[i],status1,current);
        }
   
    }
    for(int i=index1-1;i>0 ;i--){
        if(process_list[i]!=NULL){
            process_list[i]->next=add;
            break;
        }
    }       
    process_list[index1]=add;
    index1++;
}

//parts of this code taking from man guide https://linux.die.net/man/2/waitpid
void updateProcessList(process **process_list){
    for(int i=0;i<index1; i++){
        if(process_list[i]!=NULL){
        int current;
        int status=waitpid(process_list[i]->pid, &current, WNOHANG | WUNTRACED | WCONTINUED);
    if(status == TERMINATED)
        process_list[i]->status = TERMINATED;
    
    else if (status !=0) { //his status changed so we need to check what his status now
        if (WIFCONTINUED(current)) {
            process_list[i]->status=RUNNING;
        }
        else if (WIFSTOPPED(current)) { 
                process_list[i]->status=SUSPENDED;
       } 
        else if(WIFEXITED(current) ){
            process_list[i]->status=TERMINATED;
        }
        else if (WIFSIGNALED(current)){
            process_list[i]->status=TERMINATED;
        }
    }     
    } 
    }
}

//parts of this code taking from man guide https://linux.die.net/man/2/waitpid 
void updateProcessStatus(process* process_list, int pid, int status){
  
    if(pid==0) //the status didnt change, we dont need to update it
        return;

    else if(pid!=0){  //the status did change   

    if (WIFCONTINUED(status)) 
            process_list->status=RUNNING;
    else if (WIFSTOPPED(status)) 
                process_list->status=SUSPENDED;        
    else if(WIFEXITED(status))
            process_list->status=TERMINATED;
    else if(WIFSIGNALED(status))
            process_list->status=TERMINATED;
    }              
}
void cmd(cmdLine *cmdLine){
        int result =chdir(cmdLine->arguments[1]);
        if(result!=0){
            perror("no such directory");
          }
}

pid_t execute(struct cmdLine *pCmdLine) {

        if (strcmp(pCmdLine->arguments[0], "cd") == 0) {  
        cmd(pCmdLine);
        return 0;
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
        //    printf("Return not expected. Must be an execv error.n\n");
            exit(1);
        }
        if(pCmdLine->blocking){
            int child_status;            
            waitpid(pid, &child_status, WUNTRACED);
            if(child_status==-1)
               perror("There was a problem with child proccess");
        }
        printf("%s", "waiting....\n");
        return pid;
        }
}

void delete_terminate(process **process_list){
   
   for(int i=index1; i>=0; i--){
       if(process_list[i]!=NULL && process_list[i]->status==TERMINATED){
           if(i>0)
           {
               int j=i-1;
           while (process_list[j]==NULL)
           {
               j--;
           }
           if(j>=0&& process_list[j]!=NULL){
           process_list[j]->next=process_list[i]->next;                     
           }  
           }           
           freeCmdLines(process_list[i]->cmd); 
           free(process_list[i]->next); 
           process_list[i]=NULL; 
       }
   }
}

void printProcessList(process** process_list){

    updateProcessList(process_list);

    if(index1>0){
       printf("INDEX \t\t PID \t\t Command \t STATUS \t\t \n");
    }
    for( int i=0;i<index1; i++){
    if(process_list[i]!=NULL){    
    char *status;
    if(process_list[i]->status==RUNNING)
        status="RUNNING";
    else if(process_list[i]->status==SUSPENDED)
        status="SUSPENDED";
    else if(process_list[i]->status==TERMINATED){
        status="TERMINATED";
    }
    
    
    printf("%d\t\t %d\t\t %s\t\t %s\t\t \n",i, process_list[i]->pid,process_list[i]->cmd->arguments[0], status);
      }
    }
    delete_terminate(process_list);

}

void freeProcessList(process* process_list){

    process* head = process_list;
    for(int i=0;i<index1; i++){
        if(process_list!=NULL){
            freeCmdLines(process_list->cmd); 
            process_list=process_list->next; 
        }
    }
    free(head);
}

void procs(process** process_list){
    printProcessList(process_list);
}

void cmdprocess(process** process_list, int pid, int call){
    int result=kill(pid,call);
    if(result==0){
    printf("Looper handling %s\n",strsignal(call));
    }
    else{
        perror("coludn't handle call");
    } 
}

int main(int argc, char **argv){
    int i=0,enter=1;
    process *process[255];
  //  char *buf;
    char input[MAXINPUT];
    cmdLine *cmd;

  //  int size = pathconf(".", _PC_PATH_MAX);
  /*  char *ptr;

    if ((buf = (char *)malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t)size);
    } */
    while (i<argc) {
    if (strcmp(argv[i], "-D") == 0) {
        printf("Debug mode\n");
        debug=1;
    }
    i++;
    }
        
    while(1) { 
    enter=1;
    fgets (input, MAX_INPUT, stdin); 
    if(feof(stdin)| (strncmp (input,"quit",4)==0)){
        freeProcessList(process[0]); 
     //   printf("goodbyeeeeee\n");
        exit(0);
    }
    if(strncmp(input,"procs",4)==0){
        procs(process);
        enter=0;
    }
    if(strncmp(input,"suspend",7)==0){
         int id=atoi(input+strlen("suspend"));
         cmdprocess(process,id,SIGTSTP);
         enter=0;
    }
    if(strncmp(input,"kill",4)==0){
         int id=atoi(input+strlen("kill"));
         cmdprocess(process,id,SIGINT);
         enter=0;
    }
    if(strncmp(input,"wake",4)==0){
         int id=atoi(input+strlen("wake"));
         cmdprocess(process,id,SIGCONT);
         enter=0;
    }
    else if(enter){
    cmd= parseCmdLines(input); 
    pid_t pid=execute(cmd);
    addProcess(process,cmd,pid);
    procs(process); 
    }  
    
    }
}
