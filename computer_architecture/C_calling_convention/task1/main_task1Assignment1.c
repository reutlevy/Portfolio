#include <stdio.h>

extern void assFunc(int x,int y);

int main(int argc, char** argv)
{
  int x,y;
 
  printf("%s\n", "Please enter 2 numbers in decimal base");
   
  scanf("%d %d", &x, &y);
  assFunc(x, y);		/* call your assembly function */

  return 0;
}

int c_checkValidity(int x, int y){
  if(x>=y)
     return 1;
  else
     return 0;   
}
