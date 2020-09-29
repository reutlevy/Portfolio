	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

	//task2a
	int number='D'-'A';

	char censor(char c) {
	    if(c == '!')
		return '.';
	    else
		return c;
	}

	char* map(char *array, int array_length, char (*f) (char)){
	    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
	    for(int i=0;i<array_length;i++){
		mapped_array[i]=(*f)(array[i]);
	    }
            free(array);
	    return mapped_array;
	}

	//task2b
	/* Gets a char c and returns its encrypted form by adding 3 to its value If c is not between 0x20 and 0x7E it is returned unchanged */
	char encrypt(char c)  {
	    int z=c;
	    //  int val = atoi(c)
	    if((z<0x20)|(c>0x7E)){
		return c;
	    }
	    else{
		char ans=c+number;
		return ans;
	    }
	}
	/* Gets a char c and returns its decrypted form by reducing 3 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
	char decrypt(char c) {
	    int z=c;
	    //  int val = atoi(c)
	    if((z<0x20)|(c>0x7E)){
		return c;
	    }
	    else{
		char ans=c-number;
		return ans;
	    }
	}
	/* dprt prints the value of c in a decimal representation followed by a new line, and returns c unchanged. */
	char dprt(char c) {
	    int val=c;
	    printf("%d\n", val);
	    return c;
	}

	/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed
		           by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns
		           the value of c unchanged. */
	char cprt(char c){

	    int val = c;
	    if((val>=0x20)&(val<=0x7E)){
		printf("%c\n" ,val);
	    }
	    else{
		printf("%c\n",'.');
	    }
	    return c;
	}

	/* Ignores c, reads and returns a character from stdin using fgetc. */

	char my_get(char c){
	    //  puts("hello world");
	    char i=fgetc(stdin);
	    return i;
	}

	char quit(char c){
	    if(c=='q')
		exit(0);
	    else
		return c;
	}

	struct fun_desc {
	    char *name;
	    char (*fun)(char);
	};


	int main(int argc, char **argv){

	    int bounds=0;
	    int option=0;
	    char *carray=(char *)malloc(5* sizeof(char));
	    struct fun_desc menu[] = { { "Censor", censor }, { "Encrypt", encrypt }, { "Decrypt", decrypt },{"Print dec",dprt},{"Print string",cprt},{ "Get String", my_get},{"Quit",quit}, {NULL,NULL}
	    };
	   int limit=0;
           while(menu[limit].name!=NULL){
               limit++;
           }
        //   printf("%d",limit);

	    do {

		printf("Please choose a function:\n");


	      //  printf("%d\n",limit);
		for (int i = 0; i < limit; i++) {
		    printf("%d) %s \n", i, menu[i].name);
		}
	    //    printf("\n");

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

		carray = map(carray, 5, menu[option].fun);
		printf("%s\n", "DONE.");
		printf("\n");
	    }
	    while (1);
	    free(carray);
	    free(menu);

	}
