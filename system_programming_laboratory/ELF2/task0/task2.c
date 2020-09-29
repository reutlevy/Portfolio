#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

int dflag=0;
char open_file[128];
Elf32_Ehdr *memory;

void quit(){
    exit (0);
}


static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx) {
    Elf32_Shdr* header =(Elf32_Shdr *)((int)hdr + hdr->e_shoff);
	return &header[idx];
}
 
static inline Elf32_Phdr *elf_header(Elf32_Ehdr *hdr, int idx) {
     Elf32_Phdr* header =(Elf32_Shdr *)((int)hdr + hdr->e_shoff);
	return &header[idx];
}


void examine_elf(){
    printf("Enter the file name:\n");
    char buffer[128];
    scanf("%s",buffer);
    strcpy(open_file, buffer);
    if(dflag){
        printf("Debug: file name set to  %s\n",buffer);
    }
    int fd= open(buffer, O_RDONLY);
    if(fd==-1){
      perror("Coludn't open file");
      return;  
    }
    struct stat sd;
    if(fstat(fd,&sd) == -1){
        perror("Coludn't open file");
        return;
    }
    memory=mmap(NULL,sd.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    printf("%x\n",memory->e_entry);
    printf("magic number 1= %d\n",memory->e_ident[0]);
    printf("magic number 2= %d\n",memory->e_ident[1]);
    printf("magic number 3= %d\n",memory->e_ident[2]);
    printf("Section file off set= %d\n",memory->e_shoff);
    printf("The number of section header entries = %x\n",memory->e_shstrndx);
    for(int i=0;i<memory->e_shstrndx;i++){
        Elf32_Shdr *sh=elf_section(memory,i);
        printf("The size section header %d entry = %d\n",i, sh->sh_size);  
    }
    printf("The file offset in which the program header table resides = %x\n",memory->e_phoff);
    printf("The number of program header entries = %x\n",memory->e_phnum);
    for(int i=0;i<memory->e_phnum;i++){
        Elf32_Phdr *ph=elf_header(memory,i); 
        printf("The size section header %d entry = %d\n",i, ph->p_memsz);  
    }

}

struct fun_desc {
    char *name;
    void (*fun)();
};



static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset) {
    if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	char *strtab = (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
	return strtab + offset;
}

char* get_type(Elf32_Word* type){

    if((int)type==SHT_DYNAMIC)
        return "DYNAMIC"; 
    else if((int)type==SHT_DYNSYM)
        return "DYNSYM";
    else if((int)type==SHT_HIPROC)
        return "HIPROC";
    else if((int)type==SHT_HIUSER) 
        return "HIUSER";
    else if((int)type==SHT_PROGBITS) 
         return "PROGBITS";
    else if((int)type==SHT_FINI_ARRAY) 
         return "FINI_ARRAY";
    else if((int)type==SHT_INIT_ARRAY) 
         return "INIT_ARRAY";
    else if((int)type==SHT_NOBITS) 
         return "NOBITS";
    else if((int)type==SHT_GNU_versym) 
         return "VERSYM";
    else if((int)type==SHT_REL) 
         return "REL";
    else if((int)type==SHT_GNU_verdef) 
         return "VERDEF";
    else if((int)type==SHT_GNU_verneed) 
         return "VERNEED";
    else if((int)type==SHT_STRTAB)
         return "STRTAB";
    else if((int)type==SHT_SYMTAB)
         return "SYMTAB";
    else if((int)type==SHT_NOTE)
         return "NOTE";
    else
    {
         return "FNU_HASH";
    }
    
}


void headers(){
   for(int i=1;i<memory->e_shstrndx;i++){
       Elf32_Shdr* section=elf_section(memory,i);
       char* name=elf_lookup_string(memory,section->sh_name);
       char* type= get_type((Elf32_Word*)section->sh_type);
       printf ("%10s\t%10s\t%10s\t%10s\t%10s\t\n", "section_name", "section_address" , "section_offset","section_size", "section_type");
       printf ("%10s\t%10x\t%10x\t%10d\t%10s\t\n\n", name, section->sh_addr, section->sh_offset, section->sh_size, type);
       section = (Elf32_Shdr*)(section->sh_link);
   }
}

void print_symbols(){
    for(int i=1;i<memory->e_shstrndx;i++){
       Elf32_Shdr* section=elf_section(memory,i);
       Elf32_Shdr* section_link=elf_section(memory,section->sh_link);
       char* name=elf_lookup_string(memory,section->sh_name);
       if((section->sh_type==SHT_SYMTAB) || (section->sh_type==SHT_DYNSYM)){
        int symaddr = (int)memory + section->sh_offset;
        Elf32_Sym *symbol = &((Elf32_Sym *)symaddr)[0];
        int index=section->sh_size/section->sh_entsize;
        char* symbols= ((char*)memory+section_link->sh_offset);
        for(int j=0;j<index;j++){
          printf ("%10s\t%10s\t%10s\t%10s\t%10s\t\n", "index", "value" , "section_index","section_name", "symbol_name");
          printf ("%10d\t%08X\t%10x\t%10s\t%10s\t\n\n", j, symbol[j].st_value, i, name, symbol[j].st_name+symbols);
        }
       }
   }
  }


void debug(char * s){
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


int main(int argc, char **argv){

    int bounds=0;
    int option=0;
    char* name=calloc(255,1);


    struct fun_desc menu[] = { { "Toggle Debug Mode", debug }, { "Examine ELF File", examine_elf }, {"Print Section Names", headers}, {"PrintSymbols", print_symbols} ,{"Quit",quit} ,{NULL,NULL}
    };
    int limit=0;
    while(menu[limit].name!=NULL){
        limit++;
    }

    do {

        printf("Choose action:\n");
        for (int i = 0; i < limit; i++) {
            printf("%d) %s \n", i, menu[i].name);
        }	  

        char c1;

        scanf("%d",&option);
        c1=fgetc(stdin);

        printf("Option: %d\n",option);

        if ((option >= 0) & (option < limit))
            bounds = 1;
        if (bounds) {
            printf("%s\n", "Within bounds");
        } else {
            printf("%s\n", "Not within bounds");
            exit(0);
        }

        menu[option].fun(name);
        printf("%s\n", "DONE.");
        printf("\n");
    }
    while (1);
}
