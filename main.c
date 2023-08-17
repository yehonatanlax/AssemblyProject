#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"

#define LINE_LENGTH 80




const int ic; /*Instruction counter*/


int main(int argc, char *argv[]){
  int i;
  char *input_filename;
  FILE *fp;
  char *myLine = "     abc";
  char temp_binary[12];

  for(i=1 ; i<argc ; i++){
    input_filename = create_file_name(argv[i], AM);
    printf("%s\n", input_filename);/*test - works!*/

    fp = fopen(input_filename, "r");

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(1);
   }
   printf("Starting the Assembly process with %s\n", input_filename);


/*Here we need to add the function*/
  sec_pass(fp, input_filename);



  printf("Assembly process with %s is finished\n", input_filename);
  free(input_filename);
  }



    /*
  void pass_2(FILE *, char *);
  FILE *fp;//our file-pointer //TODO DELETE?
  char * v = dec_to_32(100);
  printf("%s", v);
  */
    
    /*int r = bin_to_dec("000100");*/
    /*printf("%d\n",r);*/ 
    /*printf("%s", dec_to_b64(r));*/ 

    
   
  
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

