//
// Created by reut on 01/06/2020.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int dflag=0;
int dswitch=0;

typedef struct {
    char debug_mode;
    char file_name[128];
    int unit_size;
    unsigned char mem_buf[10000];
    size_t mem_count;
} state;



struct fun_desc {
    char *name;
    void (*fun)(state*);
};

void debug(state * s){
    if(dflag){
        printf("Debug flag now off:\n");
        dflag=0;
        return;
    }
    else
    {
        printf("Debug flag now on:\n");
        dflag=1;
        return;
    }

}

void file_name(state * s){
    printf("Enter the file name:\n");
    char buffer[128];
    scanf("%s",buffer);
    strcpy(s->file_name, buffer);
    if(dflag){
        printf("Debug: file name set to  %s\n",buffer);
    }

}

void unit_size(state * s){
    int unit=0;
    printf("Enter the unit's number:\n");
    scanf("%d",&unit);
    printf("Unit = %d\n", unit);
    if((unit!=2) & (unit!=4) & (unit!=1)){
        perror("The number is not valid\n");
        return;
    }
    s->unit_size=unit;
    if(dflag){
        printf("Debug: set size to %d\n",unit);
    }

}

void load_memory(state *s){
    int fd;
    unsigned int location;
    int length=0;
    if(strcmp(s->file_name, "")==0){
        perror("The file name is empty");
        return;
    }
    fd = open(s->file_name, O_RDONLY);
    printf("Open file =%s:\n", s->file_name);
    if(fd==-1)
        perror("can't open file");
    printf("Enter the location and length:\n");
    scanf("%x" "%d", &location, &length);
    lseek(fd,location,SEEK_SET);
    s->mem_count=location;
    if(dflag){
        printf("FileName \t\t Location \t\t Length \t\n");
        printf("%s\t\t\t %d\t\t\t %d\t \n",s->file_name, location, length);
    }
    if(s->unit_size==0){
        perror("Unit were not defined");
    }
    read(fd, s->mem_buf, s->unit_size*length);
    close(fd);

}

void switch_hex(state *s){
    if(!dswitch){
        printf("Display flag now on, hexadecimal representation:\n");
        dswitch=1;
        return;
    }
    else
    {
        printf("Display flag now off, decimal representation:\n");
        dswitch=0;
        return;
    }
}

//This parts of the code are taken from the example units.c
char* unit_to_hex(int unit) {
    static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
    return formats[unit-1];
}

char* unit_to_dec(int unit) {
    static char* formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
    return formats[unit-1];
}

void print_units_dec(int unit_size, char* buffer, int count) {
    char* end = buffer + unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(stdout, unit_to_dec(unit_size), var);
        buffer += unit_size;
    }
}

void print_units_hex(int unit_size, char* buffer, int count) {
    char* end = buffer + unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(stdout, unit_to_hex(unit_size), var);
        buffer += unit_size;
    }
}

void memory_display(state *s){
    unsigned int address;
    int length1=0;
    printf("Enter the address and the length:\n");
    scanf("%x" "%d",&address,&length1);
    if(address==0){
        //    *((int*)( s->mem_buf)) += address;
        if(!dswitch){
            printf("Decimal:\n");
            print_units_dec(s->unit_size,(char*) s->mem_buf, length1);
        }
        else{
            printf("Hexadecimal:\n");
            print_units_hex(s->unit_size,(char*) s->mem_buf, length1);
        }
    }
    else{
        if(!dswitch){
            printf("Decimal:\n");
            print_units_dec(s->unit_size, (void*)address, length1);
        }
        else{
            printf("Hexadecimal:\n");
            print_units_hex(s->unit_size, (void*)address, length1);
        }
    }
}

void write_units (state *s){
    unsigned int address1;
    int target=0;
    int length2=0;
    printf("Enter the address the target and the length:\n");
    scanf("%x" "%x" "%d", &address1, &target, &length2);
    printf("The address id = %x, the target is = %x the length is =%d:\n", address1, target, length2);
    FILE *fd = fopen(s->file_name, "r+");
    if(fd==NULL){
        perror("Cant open file");
    }
    printf("Open file %s:\n", s->file_name);
    if(fd==NULL)
        perror("Cant open fie");
    fseek(fd,target,SEEK_SET);
    if(address1!=0){
        if (!address1)
        {
            fprintf(stderr, "Cant open file = %d\n", address1);
            return;
        }
        else
            write(address1,s->mem_buf, s->unit_size*length2);
    }
    else if(address1==0){
        printf("Modifying\n");
        fwrite(s->mem_buf, length2,s->unit_size,fd);
    }
    fclose(fd);
}

void memory_modify(state *s){
    unsigned int location=0;
    unsigned int val=0;
    printf("Enter the location and the val:\n");
    scanf("%x" "%x", &location, &val);
    char* end = location + s->mem_buf;
    if(dflag){
        printf("Location =%d , Val= %x:\n", location, val);
    }
   memcpy(end, &val, s->unit_size);
  // printf("%hhn\n",s->mem_buf);
}



void quit(state *s){
    exit (0);
}



int main(int argc, char **argv){

    int bounds=0;
    int option=0;
    state* s=calloc(sizeof(state), 1);



    struct fun_desc menu[] = { { "Toggle Debug Mode", debug }, { "Set File Name", file_name }, { "Set Unit Size", unit_size}, {"Load Into Memory", load_memory}, {"Toggle Display Mode", switch_hex}, {"Memory Display", memory_display}, {"Save Into File",write_units }, {"Memory Modify", memory_modify}, {"Quit",quit} ,{NULL,NULL}
    };
    int limit=0;
    while(menu[limit].name!=NULL){
        limit++;
    }

    do {
        if(dflag){
            printf("UnitSize \t\t FileName \t\t MemCount \t\t\n");
            printf("%d\t\t\t %s\t\t\t %d\t\t\t \n",s->unit_size, s->file_name, s->mem_count);
        }

        printf("Choose action:\n");
        //  printf("%d\n",limit);
        for (int i = 0; i < limit; i++) {
            printf("%d) %s \n", i, menu[i].name);
        }	    //    printf("\n");

        char c1;

        scanf("%d",&option);
        c1=fgetc(stdin);

        printf("Option: %d\n",option);

        if ((option >= 0) & (option < limit))
            bounds = 1;
        if (bounds) {
            printf("%s\n", "Within bounds");
            //   printf("\n");
        } else {
            printf("%s\n", "Not within bounds");
            exit(0);
        }

        menu[option].fun(s);
        printf("%s\n", "DONE.");
        printf("\n");
    }
    while (1);
}
