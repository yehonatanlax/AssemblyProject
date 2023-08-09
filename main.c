#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"

#define LINE_LENGTH 80


enum boolean {FALSE, TRUE};
int ic; /*Instruction counter*/

 


int main(){
    /*
  void pass_2(FILE *, char *);
  FILE *fp;//our file-pointer //TODO DELETE?
  char * v = dec_to_32(100);
  printf("%s", v);
  */
    
    int r = bin_to_dec("000100");
    /*printf("%d\n",r);*/ 
    /*printf("%s", dec_to_b64(r));*/ 

    char a[] = "000110000000";
    
    char *full_sign = analize_bin(a);
    printf("%s", full_sign);
  
  /*pass_2(fp, *****.txt);//we need to change here*/
  
 

  return 0;
}


/*the execution of the second pass*/
/*
void pass_2(FILE *fp, char *file_name){
  char line[LINE_LENGTH];
  int line_number = 1;//starts from line 1
  ic = 0;
  
  printf("Second pass begins:");
  printf("The program is reading the file:");
  
  fp = fopen(*****.txt, "r");
  if(!fp)
    printf("ERROR with opening the file");
  fgets(line, LINE_LENGTH, fp);
  //test
  printf("%s", line);
  
  fclose(fp);
}
*/

