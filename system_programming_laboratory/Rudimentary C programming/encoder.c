
       
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main( int argc, char *argv[] )
{
    FILE *fp = stdin;
    FILE *out=stdout;
    FILE *output=stderr;
    int numoftask=1;
    int numbofarg=0;

   for(int i=1; i<argc; i++){
      if(strcmp(argv[i],"-D")==0){
         numoftask=2;
      }
       if(strncmp(argv[i],"+e",2)==0){
         numbofarg=i;
         numoftask=3;
      }
       if(strncmp(argv[i],"-e",2)==0){
         numbofarg=i;
         numoftask=4;
      }
   }
   if((numoftask==3)||(numoftask==4)){
       for(int i=0;i<argc;i++){
       if(strncmp(argv[i],"-o",2)==0){
        numbofarg=i;
         numoftask=5;
       }
        if(strncmp(argv[i],"-i",2)==0){
        numbofarg=i;
         numoftask=6;
       }
       }
       
   }

   if((numoftask==1)|(numoftask==2)){
        int y='a'-'A';
        char c;
        int x;

        if(numoftask==2){
           puts("-D");
        }

        if (fp == NULL)
            return 0;

        c = fgetc(fp);

        while (c != EOF) {
            x = c;
            if(c!='\n')  {
            if ((c >= 'a') & (c <= 'z')){
                x = c - y;
               }
            
            if(numoftask==1){
               printf("%c", x);
            }
            if(numoftask==2){
                 fprintf(output,"%d\t",c);
                 fprintf(output,"%d\n",x);
                 fprintf(out,"%c",x);
            }
            }
            c = fgetc(fp);
        }
   }

     if((numoftask==3)||(numoftask==4)){

        int count=2;
        fp = stdin;
        char c = fgetc(fp);
        int y;

        while (c != EOF)
        {
            int x = argv[numbofarg][count]-'0';
            if (!(c == '\n')) {
                //    printf("%c",c);
                if(numoftask==3){
                y=c+x;
                }
                if(numoftask==4){
                  y=c-x;
                }
                printf("%c", y);
            }

            if ((count==strlen(argv[numbofarg])-1)|(c=='\n')) {
                count=2;
            } else {
                count++;
            }
            c = fgetc(fp);
        }
}

     if((numoftask==5)||(numoftask==6)){
         int num=0;
     if((strlen(argv[numbofarg]))==2){
          puts("You must enter the file name");
          return 0;
        }

     for(int i=1; i<argc; i++){
       if(strncmp(argv[i],"-e",2)==0){
         num=i;
      }
        if(strncmp(argv[i],"+e",2)==0){
         num=i;
      }
     }

        int k = 0;
        char *name = (char*)malloc(strlen(argv[numbofarg]));

        strcpy(name,argv[numbofarg]+2);

       
        if(numoftask==5){  
        out = fopen(name, "w");
        if (out == NULL) {
            puts("Couldn't open file");
          }
        }
        if(numoftask==6){
            fp = fopen(name, "r");
            if (fp == NULL) {
            fprintf(output,"%s","couldnt open file");
            return 0;
        }
        }

        char c;
        int y;
        int count = 2;

        c = fgetc(fp);

        if (argv[num][0] == '+') {

            while (c != EOF) {
                int x = argv[num][count] - '0';
                if (c != '\n') {
                    y = c + x;
                    fprintf(out,"%c", y);
                }

                if ((count == strlen(argv[num]) - 1)||(c=='\n')) {
                    count = 2;
                } else {
                    count++;
                }
                c = fgetc(fp);
            }

        }
        if (argv[num][0] == '-') {
            while (c != EOF) {
                int x = argv[num][count] - '0';
                if (c != '\n') {
                    y = c - x;
                    fprintf(out,"%c", y);
                }

                if ((count == strlen(argv[num]) - 1)||(c=='\n')) {
                    count = 2;
                } else {
                    count++;
                }
                c = fgetc(fp);
            }
        }
    }
   
   return 0;
}


