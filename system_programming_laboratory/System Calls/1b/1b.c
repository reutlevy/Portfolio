#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define STDOUT 1
#define O_RDRW 0x002
#define S_IRWXU 0777
#define CREATE 64
#define WRITE 2
#define SEEK_SET 0
#define SYS_CLOSE 6
#define SYS_READ 3
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define O_RDONLY 0x000
#include "util.h"
#define NULL 0
#define O_WRONLY 0x002

int main (int argc , char* argv[], char* envp[])
{
    int y='a'-'A';
    char c;
    int x;
    int numbofargin=0;
    int numofargout=0;
    int debug=0;
    int i=0;


    while (i<argc) {
        if (strcmp(argv[i], "-D") == 0) {
            debug=1;
        }
        i++;
    }

    i=0;
    while (i<argc)
    {
        if(strncmp(argv[i],"-o",2)==0){
            numofargout=i;
        }
        if(strncmp(argv[i],"-i",2)==0){
            numbofargin=i;
        }
        i++;
    }

    int In=0;
    int OutFile=1;
    if(numofargout!=0){
        OutFile= system_call(SYS_OPEN, argv[numofargout]+2 ,CREATE|WRITE, S_IRWXU);
        if(debug){
        system_call(SYS_WRITE, OutFile ,argv[numofargout]+2, strlen(argv[numofargout]+2));
        system_call(SYS_WRITE, OutFile, "\n", 1);
        }

    }
    else if((numofargout==0) && debug){
        system_call(SYS_WRITE, OutFile ,"STDOUT", 6);
        system_call(SYS_WRITE, OutFile, "\n", 1); 
    }
    
    if(numbofargin!=0){
        In= system_call(SYS_OPEN, argv[numbofargin]+2 ,O_RDONLY, S_IRWXU);
        if(debug){
        system_call(SYS_WRITE, OutFile ,argv[numbofargin]+2, strlen(argv[numbofargin]+2));
        system_call(SYS_WRITE, OutFile, "\n", 1);
        }
    }

    else if((numbofargin==0) && debug){
        system_call(SYS_WRITE, OutFile ,"STDIN", 5);
        system_call(SYS_WRITE, OutFile, "\n", 1); 
    }


    char *out="You must enter file name";
        if(numbofargin!=0 && (strlen(argv[numbofargin]))==2){
            system_call(SYS_WRITE, STDOUT, out, strlen(out));
                return 0;
        }

        if(numofargout!=0 && (strlen(argv[numofargout]))==2){
            system_call(SYS_WRITE, STDOUT, out, strlen(out));
            return 0;
    }

    char *error="couldn't open file";
    if((In<0)){
        system_call(SYS_WRITE, STDOUT, error, strlen(error));  
        system_call(SYS_WRITE, STDOUT, "\n", 1);
        return 0;
    }


    if(debug){
        OutFile=2;
    }

    void debug1(int read1, int write){
             system_call(SYS_WRITE,STDERR,itoa(SYS_READ),strlen(itoa(SYS_READ)));
             system_call(SYS_WRITE, STDERR, "\t",1);
             system_call(SYS_WRITE,STDERR, itoa(read1), strlen(itoa(read1)));
            system_call(SYS_WRITE, STDERR, "\n",1);
             system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
             system_call(SYS_WRITE, STDERR, "\t",1);
             system_call(SYS_WRITE,STDERR, itoa(write), strlen(itoa(write)));
            system_call(SYS_WRITE, STDERR, "\n",1);
    }   
      
    int read= system_call(SYS_READ, In, &c, 1);
    int write=0;
    
    while (read>0) {
            x = c;
            if ((c >= 'a') & (c <= 'z')){
                x = c - y;
                write=system_call(SYS_WRITE, OutFile, &x, 1);
            }
            else{
                write=system_call(SYS_WRITE, OutFile, &x, 1);
            }                  
            read= system_call(SYS_READ, In, &c, 1);
            if(debug)
           {
            debug1(read,write);
               
            }
    }
   return 0;
}
        
