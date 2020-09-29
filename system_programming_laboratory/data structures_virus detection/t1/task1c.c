
#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char* sig;
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};

char *suspected;

void PrintHex(char *buffer, int bytes,FILE *file){
    for(int i=0;i<bytes;i++){
        fprintf(file,"%02.2hhX ", buffer[i]);
    }
}


void printVirus(virus* virus, FILE* output){

    char name[] = "Virus name: ";
    fwrite(name,1,sizeof(name),output);
    fwrite(virus->virusName,1,16,output);
    fwrite("\n", sizeof(char), 1, output);
    char size[] = "Virus size: ";
    fwrite(size,1,sizeof(size),output);
    fprintf(output,"%hd",virus->SigSize);
    fwrite("\n", sizeof(char), 1, output);
    char signature[] = "signature: ";
    fwrite("signature: ",sizeof(signature),1,output);
    fwrite("\n", sizeof(char), 1, output);
    PrintHex(virus->sig,virus->SigSize,output);
    fwrite("\n", sizeof(char), 1, output);
}

link* list_print(link *virus_list){
    link *head=virus_list;
    while (virus_list!=NULL){
        printVirus(virus_list->vir,stdout);
        virus_list=virus_list->nextVirus;
    }
    return head;
}

link* list_append(link* virus_list, virus* data)
{
    link *answer=virus_list;
    link *link=calloc(1,sizeof(struct link));
    if(virus_list==NULL) {
        link->nextVirus = NULL;
        link->vir=data;
        return link;

    } else{
        link->vir=data;
        while (virus_list->nextVirus!=NULL){
            virus_list=virus_list->nextVirus;
        }
        virus_list->nextVirus=link;
    }
    return answer;
}


void list_free(link *virus_list) {
    link *head=virus_list;
    while (virus_list!=NULL){
        head=virus_list;
        virus_list=virus_list->nextVirus;
        free(head->vir->sig);
        free(head->vir);
        free(head);
    }
    free(virus_list);
}

virus* readVirus(FILE* f){
    virus *virus1= calloc(1,sizeof(struct virus));
    fread(virus1,1,18,f);
    virus1->sig=calloc(sizeof(char ),virus1->SigSize);
    fread(virus1->sig,virus1->SigSize,1,f);
    return virus1;
}


void detect_virus(char *buffer, unsigned int size, link *virus_list){
    link * head=virus_list;
    while (head != NULL) {
        for(int index=0; (index <= size-head->vir->SigSize) ; index++) {
            if ((memcmp(buffer+index, head->vir->sig,head->vir->SigSize) == 0)) {
                printf("The starting byte of the suspect virus is : %d \n", index);
                printf("The virus name : %s \n", head->vir->virusName);
                printf("The size of the virus signature : %d \n", head->vir->SigSize);
                break;
            }
        }
        head = head->nextVirus;
    }
}



link* upload_virus(link *virus_list){
    struct link *head = virus_list;
    FILE *f=fopen(suspected,"r");
    if(f == NULL)
    {
        printf("File was not found");
        exit(0);
    }
    char buffer[10000];
    fread(buffer,sizeof(buffer),1,f);
    fseek(f, 0, SEEK_END); //find size of f
    unsigned long size = ftell(f);
    fseek(f, 0, SEEK_SET); // move back to the begining
    detect_virus(buffer,size,virus_list);
    fclose(f);
    return head;
}

struct fun_desc {
    char *name;
    link* (*fun)();
};

link* quit(link *virus_list){
    list_free(virus_list);
    exit(0);
}


link* list_signatures(link* virus_list){

    printf("%s\n","Hello, please enter the file name");
    char *name=calloc(256,1);
    scanf("%s",name);
    FILE *f = fopen(name, "r");
    if(f == NULL)
    {
        printf("File was not found");
        exit(0);
    }
    free(name);
    fseek(f, 0, SEEK_END); //find size of f
    long size = ftell(f);
    fseek(f, 0, SEEK_SET); // move back to the begining
    bool end=false;
    while (!end) {
        virus_list=list_append(virus_list,readVirus(f));
        long lOldPos = ftell(f);
        end= lOldPos == size;
    }
    fclose(f);
    return virus_list;
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize) {
    FILE *f = fopen(fileName, "r+");
    if(f == NULL)
    {
        printf("File was not found");
        exit(0);
    }
    char nops[signitureSize];
    int i;
    for (i = 0; i < signitureSize; i++) {
        nops[i] = 0x90;
    }
    fseek(f,signitureOffset,SEEK_SET);
    fwrite(nops,1,signitureSize,f);
    fclose(f);
}

link *fixfile(link* link1){
    printf("%s\n","Hello,please enter the starting byte of the suspected file");
    int start=0;
    scanf("%d",&start);
    printf("%s\n","Hello,please enter the size of the virus");
    int size=0;
    scanf("%d",&size);
    kill_virus(suspected,start,size);
    return link1;
}




int main(int argc, char **argv) {

    int limit=0;
    int option=-1;
    suspected=argv[1];
    link *link2=NULL;
    struct fun_desc menu[]  = { {"Load signatures", list_signatures }, { "Print signatures", list_print },{"Detect viruses",upload_virus},{"Fix file",fixfile},{"Quit",quit}, {NULL,NULL}
    };
    while(menu[limit].name!=NULL){
        limit++; }

    do {
        printf("Please choose a function:\n");
        for (int i = 0; i < limit; i++) {
            printf("%d) %s \n", i+1, menu[i].name);
        }
        int bounds=0;
        scanf("%d",&option);
        char c1=fgetc(stdin);
        printf("Option: %d\n",option);
        if ((option >= 0) & (option <= limit))
            bounds = 1;
        if (bounds) {
            printf("%s\n", "Within bounds");
        } else {
            printf("%s\n", "Not within bounds");
            exit(0);
        }
        link2 = (menu[option - 1].fun(link2));
        printf("%s\n", "DONE.");
        printf("\n");
    }
    while (1);
    free(menu);

}

