
#include <stdio.h>


int digit_cnt(char *str){
   size_t len = sizeof(str);
   int number=-1;
   int result=0;
   for (int i = 0; i < len; i++) {
      number=str[i]-'0';
      str[i]++;
      if((number>=0) && (number<=9)){
         result++;
      }
   }
   return result;
}

int main(int argc, char **argv){
    int val=digit_cnt(argv[1]);
    printf("the number of digits is %d\n", val);
}
